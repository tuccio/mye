#pragma once

#include "Singleton.h"

#include <string>
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

			void OpenEventLogFile(const std::string &file);
			void OpenErrorLogFile(const std::string &file);

			void LogEvent(const std::string &string);
			void LogError(const std::string &string);

		private:

			const char* GetTimestamp(void);

			std::ostream m_eventStream;
			std::ostream m_errorStream;

			std::ofstream m_eventFile, m_errorFile;

		};

	}

}


