#include "Player.h"
#include "Scheduler.h"
#include "FluidSynthWrapper.h"
#include "Module.h"
#include "NoteNames.h"

namespace sft
{

Player &Player::singleton()
{
	static Player p;
	return p;
}

Player::Player()
{
	memset(_lastInstrument, 0xFF, sizeof(_lastInstrument));
	memset(_lastNote, 0xFF, sizeof(_lastNote));
}

Player::~Player()
{
	//stop, join...
}

void Player::threadFn()
{
	Scheduler::singleton().start(_startTime);

	Position currentPos;
	int tickOfLine = 0;

	while (_run)
	{
		_mutex.lock_shared();

		if (!tickOfLine)
		{
			currentPos = advancePosition();

			for (int track = 0, trackBit = 1; track < TRACK_COUNT; ++track, trackBit <<= 1)
			{
				if (currentPos.trackMask & trackBit)
				{
					int inst 		= currentPos.pattern->instrument(track, currentPos.line);
					int defVelocity = _lastDefVelocity[track];
					int noteColumns = 0;

					if (inst != _lastInstrument[track])
					{
						Module::current()->readLock();

						if (inst < Module::current()->instruments())
						{
							if (inst >= 0)
							{
								Module::current()->instrument(inst).programSelect(track);
								defVelocity = Module::current()->instrument(inst).velocity();
							}

							noteColumns = Module::current()->noteColumns(track);
						}
						else
						{
							inst = -1;
						}

						Module::current()->readUnlock();
						
						_lastInstrument[track] = inst;
						_lastDefVelocity[track] = defVelocity;
					}

					for (int nc = 0; nc < MAX_NOTE_COLUMNS; ++nc)
					{
						int lastNote = _lastNote[track * MAX_NOTE_COLUMNS + nc];
						
						if (nc < noteColumns)
						{
							//active note column--send note on
							int velocity = currentPos.pattern->velocity(track, nc, currentPos.line);
							if (velocity < 0)
							{
								velocity = defVelocity;
							}
							int note = currentPos.pattern->note(track, nc, currentPos.line);

							if (note >= NOTE_OFF)
							{
								const int ln = track * MAX_NOTE_COLUMNS + nc;

								if (lastNote >= FIRST_VALID_NOTE && note != _lastNote[ln])
								{
									FluidSynth::singleton().noteOff(track, lastNote);								
								}

								if (note >= FIRST_VALID_NOTE && velocity > 0)
								{
									FluidSynth::singleton().noteOn(track, note, velocity);
									_lastNote[ln] = note;
								}
								else
								{
									_lastNote[ln] = -1;
								}
							}
						}
						else if (lastNote >= FIRST_VALID_NOTE)
						{
							FluidSynth::singleton().noteOff(track, lastNote);								
						}
					}
				}
			}
		}

		for (int track = 0, trackBit = 1; track < TRACK_COUNT; ++track, trackBit <<= 1)
		{
			if (currentPos.trackMask & trackBit)
			{
			//for all fx columns:
			//	handle fx
			}
		}

		_mutex.unlock_shared();

		Scheduler::singleton().waitForTick(Module::current()->tickRate());

		++tickOfLine;

		if (Module::current()->tpl() <= tickOfLine)
		{
			tickOfLine = 0;
		}
	}
}

Player::Position Player::advancePosition()
{
	Position current = _nextPos;

	++_nextPos.line;

	Module::P module = Module::current();

	module->readLock();

	if (_nextPos.pattern->length() <= _nextPos.line)
	{
		_nextPos.line = 0;

		if (!_loop)
		{
			++_nextPos.order;

			if (module->orderLength() <= _nextPos.order)
			{
				_nextPos.order = 0;
			}
		}
	}

	_nextPos.pattern 	= module->orderPattern(_nextPos.order);
	_nextPos.trackMask = module->trackMask(_nextPos.order);

	module->readUnlock();

	return current;
}

}	//namespace sft
