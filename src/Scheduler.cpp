#include "Scheduler.h"

#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#ifdef __linux
#include <linux/rtc.h>
#endif

#define MAXIMUM_RTC_FREQ 2048
#define MINIMUM_RTC_FREQ 512

namespace sft
{

Scheduler& Scheduler::singleton()
{
	static Scheduler s;
	return s;
}

#ifdef __linux

Scheduler::Scheduler()
:	_rtc(-1),
    _rtcFrequency(0),
    _rtcPIE(false)
{
    bool success = false;

    _rtc = open("/dev/rtc", O_RDONLY);

    if (_rtc != -1)
    {
        _rtcFrequency = MAXIMUM_RTC_FREQ;
        _rtcPIE = 0;

        while (_rtcFrequency >= MINIMUM_RTC_FREQ && !success)
        {
            if (ioctl(_rtc, RTC_UIE_OFF, 0) == -1 || ioctl(_rtc, RTC_IRQP_SET, rtcFrequency) == -1)
            {
                _rtcFrequency /= 2;
            }
            else
            {
                success = true;
            }
        }

        if (!success)
        {
            close(rtc);
            rtc = -1;
        }
    }
}

Scheduler::~Scheduler()
{
	if (_rtc > -1)
	{
		close(_rtc);
	}
}

#endif

void Scheduler::start(struct timeval& startTime)
{
	_next.tv_sec 	= startTime.tv_sec;
    _next.tv_usec 	= startTime.tv_usec;
}

void Scheduler::waitForTick(int rate)
{
    _next.tv_sec 	+= (60 / rate);
    _next.tv_usec	+= ((60000000 / rate) % 1000000);
    
    while (_next.tv_usec > 1000000)
    {
        ++_next.tv_sec;
        _next.tv_usec -= 1000000;
    }

    struct timeval now;
    gettimeofday(&now, NULL);

#ifdef __linux
    if (_rtc > -1)
    {
        if (!_rtcPIE)
        {
            ioctl(_rtc, RTC_PIE_ON, 0);
            _rtcPIE = true;
        }

        while ((_next.tv_sec - now.tv_sec) > 0 || ((_next.tv_sec - now.tv_sec) == 0 && (_next.tv_usec - now.tv_usec) > 1000000 / _rtcFrequency))
        {
            unsigned long l;
            ssize_t n = read(rtc, &l, sizeof(unsigned long));
            (void)n;
            gettimeofday(&now, NULL);
        }

        while ((_next.tv_sec - now.tv_sec) > 0 || ((_next.tv_sec - now.tv_sec) == 0 && (_next.tv_usec - now.tv_usec) > 0))
        {
            gettimeofday(&now, NULL);
        }

        return;
    }
#endif
    
    struct timespec req, rem;
    req.tv_sec 	= _next.tv_sec - now.tv_sec;
    req.tv_nsec = (_next.tv_usec - now.tv_usec) * 1000;
    
    while (req.tv_nsec < 0)
    {
        --req.tv_sec;
        req.tv_nsec += 1000000000;
    }

    if (req.tv_sec >= 0)
    {
        while (nanosleep(&req, &rem) == -1) { };
    }
}

void Scheduler::stop()
{
#ifdef __linux
    if (_rtcPIE && _rtc > -1)
    {
        ioctl(_rtc, RTC_PIE_OFF, 0);
        _rtcPIE = false;
    }
#endif
}

}	//namespace sft
