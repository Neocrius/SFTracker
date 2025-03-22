#pragma once

#include <sys/time.h>

namespace sft
{

//adapted from Tutka tracker by Vesa Halttunen
//uses RTC on Linux, nanosleep otherwise

class Scheduler
{
public:
	Scheduler& singleton();

	void start(struct timeval& startTime);

	void waitForTick(int bpm);

	void stop();
	
private:
    struct timeval 	m_next;

#ifdef __linux
	Scheduler();

	~Scheduler();

    int 			m_rtc;
    int 			m_rtcFrequency;
    bool 			m_rtcPIE;
#endif
};

}	//namespace sft
