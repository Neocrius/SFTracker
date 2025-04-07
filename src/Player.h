#pragma once

#include "Pattern.h"
#include <thread>
#include <sys/time.h>

namespace sft
{

class Player
{
public:
	static Player &singleton();

private:
	Player();
	
	~Player();

	void threadFn();

	std::thread 		_thread;
	std::shared_mutex	_mutex;

	struct timeval 		_startTime;
	volatile bool		_run;
	bool				_loop;

	struct Position
	{
		int 			order;
		int 			line;
		Pattern::P 		pattern;
		int				trackMask;
	};

	Position			_nextPos;

	Position advancePosition();

	int					_lastInstrument[TRACK_COUNT];
	int					_lastDefVelocity[TRACK_COUNT];
	int					_lastNote[TRACK_COUNT * MAX_NOTE_COLUMNS];
};

}	//namespace sft
