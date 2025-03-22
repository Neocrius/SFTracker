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
:	m_rtc(-1),
    m_rtcFrequency(0),
    m_rtcPIE(false)
{
    bool success = false;

    m_rtc = open("/dev/rtc", O_RDONLY);

    if (m_rtc != -1)
    {
        m_rtcFrequency = MAXIMUM_RTC_FREQ;
        m_rtcPIE = 0;

        while (m_rtcFrequency >= MINIMUM_RTC_FREQ && !success)
        {
            if (ioctl(m_rtc, RTC_UIE_OFF, 0) == -1 || ioctl(m_rtc, RTC_IRQP_SET, rtcFrequency) == -1)
            {
                m_rtcFrequency /= 2;
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
	if (m_rtc > -1)
	{
		close(m_rtc);
	}
}

#endif

void Scheduler::start(struct timeval& startTime)
{
	m_next.tv_sec 	= startTime.tv_sec;
    m_next.tv_usec 	= startTime.tv_usec;
}

void Scheduler::waitForTick(int bpm)
{
    m_next.tv_sec 	+= (25 / bpm) / 10;
    m_next.tv_usec	+= ((2500000 / bpm) % 1000000);
    
    while (m_next.tv_usec > 1000000)
    {
        ++m_next.tv_sec;
        m_next.tv_usec -= 1000000;
    }

    struct timeval now;
    gettimeofday(&now, NULL);

#ifdef __linux
    if (m_rtc > -1)
    {
        if (!m_rtcPIE)
        {
            ioctl(m_rtc, RTC_PIE_ON, 0);
            m_rtcPIE = true;
        }

        while ((m_next.tv_sec - now.tv_sec) > 0 || ((m_next.tv_sec - now.tv_sec) == 0 && (m_next.tv_usec - now.tv_usec) > 1000000 / m_rtcFrequency))
        {
            unsigned long l;
            ssize_t n = read(rtc, &l, sizeof(unsigned long));
            (void)n;
            gettimeofday(&now, NULL);
        }

        while ((m_next.tv_sec - now.tv_sec) > 0 || ((m_next.tv_sec - now.tv_sec) == 0 && (m_next.tv_usec - now.tv_usec) > 0))
        {
            gettimeofday(&now, NULL);
        }

        return;
    }
#endif
    
    struct timespec req, rem;
    req.tv_sec 	= m_next.tv_sec - now.tv_sec;
    req.tv_nsec = (m_next.tv_usec - now.tv_usec) * 1000;
    
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
    if (m_rtcPIE && m_rtc > -1)
    {
        ioctl(m_rtc, RTC_PIE_OFF, 0);
        m_rtcPIE = false;
    }
#endif
}

}	//namespace sft
