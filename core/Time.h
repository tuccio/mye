#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include <string>

namespace mye
{

	namespace core
	{

		class FloatSeconds;
		class Seconds;
		class Milliseconds;

		inline Seconds TimeSinceEpoch(void);
		inline std::string FormatEpochTime(std::string format);

		class FloatSeconds
		{

		public:

			FloatSeconds(void);
			FloatSeconds(float s);
			FloatSeconds(const Seconds &s);
			FloatSeconds(const Milliseconds &ms);

			inline FloatSeconds operator+ (const FloatSeconds &s) const;
			inline FloatSeconds operator- (const FloatSeconds &s) const;

			inline operator float (void) const;

		private:

			float m_seconds;

		};

		class Seconds
		{

		public:

			Seconds(void);
			Seconds(unsigned int s);
			Seconds(Milliseconds ms);
			Seconds(FloatSeconds s);

			inline Seconds operator+ (const Seconds &s) const;
			inline Seconds operator- (const Seconds &s) const;

			inline operator unsigned int (void) const;

		private:

			unsigned int m_seconds;

		};

		class Milliseconds
		{

		public:

			Milliseconds(void);
			Milliseconds(unsigned int ms);
			Milliseconds(Seconds ms);
			Milliseconds(FloatSeconds s);

			inline Milliseconds operator+ (const Milliseconds &ms) const;
			inline Milliseconds operator- (const Milliseconds &ms) const;

			inline operator unsigned int (void) const;

		private:

			unsigned int m_milliseconds;

		};

		class StopWatch
		{

		public:

			StopWatch(void);

			inline void Start(void);
			inline Milliseconds Stop(void);

			inline bool IsRunning(void) const;

			inline Milliseconds GetElapsedTime(void) const;

		private:

			Milliseconds m_startTime;
			bool m_running;

		};

		class LapStopWatch
		{

		public:

			LapStopWatch(void);

			inline void Start(void);
			inline Milliseconds Stop(void);

			inline Milliseconds Lap(void);

			inline bool IsRunning(void) const;

			inline Milliseconds GetElapsedTime(void) const;

		private:

			Milliseconds m_startTime;
			Milliseconds m_lastLap;
			bool m_running;

		};

	}

}

#include "Time.inl"