#pragma once

#include <sys/time.h>

namespace sft
{

//adapted from Tutka tracker by Vesa Halttunen
//uses RTC on Linux, nanosleep otherwise

class Scheduler
{
public:
	static Scheduler& singleton();

	void start(struct timeval& startTime);

	void waitForTick(int rate);	//rate = ticks / minute = bpm * lpb * tpl

	void stop();
	
private:
    struct timeval 	_next;

#ifdef __linux
	Scheduler();

	~Scheduler();

    int 			_rtc;
    int 			_rtcFrequency;
    bool 			_rtcPIE;
#endif
};

}	//namespace sft
