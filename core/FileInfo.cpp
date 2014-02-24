#include "FileInfo.h"

#include <regex>

using namespace mye::core;

FileInfo::FileInfo(const String &file) :
	m_file(file)
{
}


FileInfo::~FileInfo(void)
{
}

String FileInfo::GetFilePath(void) const
{
	return m_file;
}

String FileInfo::GetFileName(void) const
{

	std::smatch sm;

	std::regex regex("^(.*[/\\\\])?([^/\\\\]+)$");
	std::string file(m_file.CString());

	if (std::regex_match(file, sm, regex))
	{
		return sm[2].str().c_str();
	}

	return String();

}

String FileInfo::GetFileDirectory(void) const
{

	std::smatch sm;

	std::regex regex("^(.*[/\\\\])?([^/\\\\]+)$");
	std::string file(m_file.CString());

	if (std::regex_match(file, sm, regex))
	{
		return sm[1].str().c_str();
	}

	return String(".");

}