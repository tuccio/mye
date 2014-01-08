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

			std::ostream _event;
			std::ostream _error;

			std::ofstream _eventFile, _errorFile;

		};

	}

}


