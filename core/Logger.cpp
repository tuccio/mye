#include "Logger.h"

#include <iostream>
#include <fstream>

#include <ctime>

using namespace mye::core;

Logger::Logger(void) :
	m_errorFile(StandardOutput),
	m_eventFile(StandardOutput)
{
}

Logger::Logger(const char *events, const char *errors) :
	m_errorFile(errors),
	m_eventFile(events)
{

}


Logger::~Logger(void)
{
}

void Logger::OpenEventLogFile(const String &file)
{

	if (m_eventFile)
	{
		m_eventFile.Close();
	}

	if (!file.IsEmpty())
	{
		m_eventFile = FileOutputStream(file);
	}

}

void Logger::OpenErrorLogFile(const String &file)
{

	if (m_errorFile)
	{
		m_errorFile.Close();
	}

	if (!file.IsEmpty())
	{
		m_errorFile = FileOutputStream(file);
	}

}

void Logger::LogEvent(const String & tag, const String & message)
{
	m_eventFile << GetTimestamp() << " " << tag << ": " << message << NewLine;
	m_lastEvent = message;
}

void Logger::LogError(const String & tag, const String & message)
{
	m_errorFile << GetTimestamp() << " " << tag << ": " << message << NewLine;
	m_lastError = message;
}

const char * Logger::GetTimestamp(void)
{

	time_t t = time(nullptr);
	tm * timeinfo = localtime(&t);

	static char output[20];
	strftime(output, 20, "[%d/%m/%y %H:%M:%S]", timeinfo);

	return output;

}

String Logger::GetLastEvent(void)
{
	return m_lastEvent;
}

String Logger::GetLastError(void)
{
	return m_lastError;
}

bool Logger::LogEventOptional(const String & tag, const String & message)
{

	Logger * logger = GetSingletonPointer();

	if (logger)
	{
		logger->LogEvent(tag, message);
		return true;
	}

	return false;

}

bool Logger::LogErrorOptional(const String & tag, const String & message)
{

	Logger * logger = GetSingletonPointer();

	if (logger)
	{
		logger->LogError(tag, message);
		return true;
	}

	return false;

}