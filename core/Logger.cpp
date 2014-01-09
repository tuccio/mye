#include "Logger.h"

#include <iostream>
#include <fstream>

#include <ctime>

using namespace mye::core;

Logger::Logger(void) :
	m_eventStream(std::cout.rdbuf()),
	m_errorStream(std::cerr.rdbuf())
{
}


Logger::~Logger(void)
{
}

void Logger::OpenEventLogFile(const std::string &file)
{

	if (m_eventFile)
	{
		m_eventFile.close();
	}

	if (!file.empty())
	{
		m_eventFile.open(file);
	}	

	if (m_eventFile)
	{
		m_eventStream.set_rdbuf(m_eventFile.rdbuf());
	}
	else
	{
		m_eventStream.set_rdbuf(std::cout.rdbuf());
	}

	m_eventStream.clear();

}

void Logger::OpenErrorLogFile(const std::string &file)
{

	if (m_errorFile)
	{
		m_errorFile.close();
	}

	if (!file.empty())
	{
		m_errorFile.open(file);
	}

	if (m_errorFile)
	{
		m_errorStream.set_rdbuf(m_errorFile.rdbuf());
	}
	else
	{
		m_errorStream.set_rdbuf(std::cout.rdbuf());
	}

	m_errorStream.clear();

}

void Logger::LogEvent(const std::string &string)
{

	

	m_eventStream << GetTimestamp() << " " << string << std::endl;
}

void Logger::LogError(const std::string &string)
{
	time_t t = time(NULL);
	m_errorStream << GetTimestamp() << " " << string << std::endl;
}

const char* Logger::GetTimestamp(void)
{

	time_t t = time(NULL);
	tm *timeinfo = localtime(&t);

	static char output[20];
	strftime(output, 20, "[%d/%m/%y %H:%M:%S]", timeinfo);

	return output;

}