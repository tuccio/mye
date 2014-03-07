#include "Time.h"

#include <cmath>

using namespace mye::core;

StopWatch::StopWatch(void)
{
	m_running = false;
}

LapStopWatch::LapStopWatch(void)
{
	m_running = false;
}

/* Float seconds */

FloatSeconds::FloatSeconds(void)
{
}

FloatSeconds::FloatSeconds(float s)
{
	m_seconds = s;
}

FloatSeconds::FloatSeconds(const Seconds &s)
{
	m_seconds = ((unsigned int) s);
}

FloatSeconds::FloatSeconds(const Milliseconds &ms)
{
	m_seconds = ((unsigned int) ms) * 0.001f;
}

/* Seconds */

Seconds::Seconds(void)
{
}

Seconds::Seconds(unsigned int s)
{
	m_seconds = s;
}

Seconds::Seconds(Milliseconds ms)
{
	m_seconds = ((unsigned int) ms) / 1000;
}

Seconds::Seconds(FloatSeconds s)
{
	m_seconds = (unsigned int) ((float) s);
}

/* Milliseconds */

Milliseconds::Milliseconds(void)
{
}

Milliseconds::Milliseconds(unsigned int ms)
{
	m_milliseconds = ms;
}

Milliseconds::Milliseconds(Seconds ms)
{
	m_milliseconds = ((unsigned int) ms) * 1000;
}

Milliseconds::Milliseconds(FloatSeconds s)
{
	m_milliseconds = (unsigned int) (s * 1000.0f);
}