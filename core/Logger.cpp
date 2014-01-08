#include "Logger.h"

#include <iostream>
#include <fstream>

using namespace mye::core;

Logger::Logger(void) :
	_event(std::cout.rdbuf()),
	_error(std::cerr.rdbuf())
{
}


Logger::~Logger(void)
{
}

void Logger::OpenEventLogFile(const std::string &file)
{

	if (_eventFile)
	{
		_eventFile.close();
	}

	if (!file.empty())
	{
		_eventFile.open(file);
	}	

	if (_eventFile)
	{
		_event.set_rdbuf(_eventFile.rdbuf());
	}
	else
	{
		_event.set_rdbuf(std::cout.rdbuf());
	}

}

void Logger::OpenErrorLogFile(const std::string &file)
{

	if (_errorFile)
	{
		_errorFile.close();
	}

	if (!file.empty())
	{
		_errorFile.open(file);
	}

	if (_errorFile)
	{
		_error.set_rdbuf(_errorFile.rdbuf());
	}
	else
	{
		_error.set_rdbuf(std::cout.rdbuf());
	}

}

void Logger::LogEvent(const std::string &string)
{
	_event << string;
}

void Logger::LogError(const std::string &string)
{
	_error << string;
}