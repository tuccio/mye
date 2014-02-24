#pragma once

#include "Singleton.h"
#include "String.h"
#include "Stream.h"

#include <ostream>
#include <fstream>

namespace mye
{

	namespace core
	{

		class Logger :
			public Singleton<Logger>
		{

		public:

			Logger(void);
			~Logger(void);

			void OpenEventLogFile(const String &file);
			void OpenErrorLogFile(const String &file);

			void LogEvent(const String &string);
			void LogError(const String &string);

			static bool LogEventOptional(const String &string);
			static bool LogErrorOptional(const String &string);

			String GetLastEvent(void);
			String GetLastError(void);

		private:

			const char* GetTimestamp(void);
 
// 			std::ostream m_eventStream;
// 			std::ostream m_errorStream;

			FileOutputStream m_eventFile;
			FileOutputStream m_errorFile;

//			std::ofstream m_eventFile, m_errorFile;

			String m_lastEvent;
			String m_lastError;

		};

	}

}


