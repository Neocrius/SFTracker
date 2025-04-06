#include "Player.h"
#include "Module.h"

namespace sft
{

static Player &Player::singleton()
{
	static Player p;
	return p;
}

void Player::threadFn()
{
	Scheduler::singleton().start(_startTime);

	Position currentPos;
	int tickOfLine = 0;

	while (_run)
	{
		if (!tickOfLine)
		{
			currentPos = advancePosition();

			//	for all tracks:
			//		update instruments
			//		for all note columns:
			//			send note on/offs
		}

		//for all tracks:
		//	for all fx columns:
		//		handle fx

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
	WRITE_LOCK;

	Position current = _nextPos;

	++_nextPos.line;

	Module::P module = Module::current();

	module->readLock();

	if (_nextPos.pattern->length() <= _nextPos.line)
	{
		_next.line = 0;

		if (!_loop)
		{
			++_nextPos.order;

			if (module->orderLength() <= _next.order)
			{
				_nextPos.order = 0;
			}
		}
	}

	_nextPos._pattern 	= module->orderPattern(_nextPos.order);
	_nextPos._trackMask = module->trackMask(_nextPos.order);

	module->readUnlock();

	return current;
}

}	//namespace sft
