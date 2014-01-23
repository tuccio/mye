#ifdef _WIN32
#define __MYE_GET_MS_TIME() (static_cast<unsigned int>(GetTickCount()))
#endif

namespace mye
{

	namespace core
	{

		/* Stop watch */

		void StopWatch::Start(void)
		{
			m_startTime = __MYE_GET_MS_TIME();
			m_active = true;
		}

		Milliseconds StopWatch::Stop(void)
		{
			Milliseconds elapsed = GetElapsedTime();
			m_active = false;
			return elapsed;
		}

		bool StopWatch::IsRunning(void) const
		{
			return m_active;
		}

		Milliseconds StopWatch::GetElapsedTime(void) const
		{
			Milliseconds current(__MYE_GET_MS_TIME());
			return current - m_startTime;
		}

		void LapStopWatch::Start(void)
		{
			m_startTime = __MYE_GET_MS_TIME();
			m_lastLap = m_startTime;
			m_active = true;
		}

		Milliseconds LapStopWatch::Stop(void)
		{
			Milliseconds elapsed = GetElapsedTime();
			m_active = false;
			return elapsed;
		}

		bool LapStopWatch::IsRunning(void) const
		{
			return m_active;
		}

		Milliseconds LapStopWatch::GetElapsedTime(void) const
		{
			Milliseconds current(__MYE_GET_MS_TIME());
			return current - m_startTime;
		}

		Milliseconds LapStopWatch::Lap(void)
		{
			Milliseconds current(__MYE_GET_MS_TIME());
			Milliseconds d = current - m_lastLap;
			m_lastLap = current;
			return d;
		}

		/* Float seconds */
		
		FloatSeconds FloatSeconds::operator+ (const FloatSeconds &s) const
		{
			return FloatSeconds(m_seconds + s.m_seconds);
		}

		FloatSeconds FloatSeconds::operator- (const FloatSeconds &s) const
		{
			return FloatSeconds(m_seconds - s.m_seconds);
		}

		FloatSeconds::operator float (void) const
		{
			return m_seconds;
		}

		/* Seconds */

		Seconds Seconds::operator+ (const Seconds &s) const
		{
			return Seconds(m_seconds + s.m_seconds);
		}

		Seconds Seconds::operator- (const Seconds &s) const
		{
			return Seconds(m_seconds - s.m_seconds);
		}

		Seconds::operator unsigned int (void) const
		{
			return m_seconds;
		}

		/* Milliseconds */

		Milliseconds Milliseconds::operator+ (const Milliseconds &ms) const
		{
			return Milliseconds(m_milliseconds + ms.m_milliseconds);
		}

		Milliseconds Milliseconds::operator- (const Milliseconds &ms) const
		{
			return Milliseconds(m_milliseconds - ms.m_milliseconds);
		}

		Milliseconds::operator unsigned int (void) const
		{
			return m_milliseconds;
		}

	}

}

#undef __MYE_GET_MS_TIME