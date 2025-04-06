#pragma once

namespace sft
{

class Player
{
public:
	static Player &singleton();

private:
	~Player();

	void threadFn();

	Position advancePosition();

	std::thread 		_thread;
	std::shared_mutex	_mutex;

	struct timevalue_t 	_startTime;
	volatile bool		_run;
	bool				_loop;

	struct Position
	{
		int 			_order;
		int 			_line;
		Pattern:P 		_pattern;
		int				_trackMask;
	};

	Position			_nextPos;

	int					_lastInstrument[TRACK_COUNT];
	int					_lastDefVelocity[TRACK_COUNT];
	int					_lastNote[TRACK_COUNT * MAX_NOTE_COLUMNS]
}

}	//namespace sft
