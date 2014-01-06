#include "FileInfo.h"

#include <regex>

using namespace mye::core;

FileInfo::FileInfo(const std::string &file) :
	_file(file)
{
}


FileInfo::~FileInfo(void)
{
}

std::string FileInfo::GetFilePath(void) const
{
	return _file;
}

std::string FileInfo::GetFileName(void) const
{

	std::smatch sm;

	std::regex regex("^(.*[/\\\\])?([^/\\\\]+)$");

	if (std::regex_match(_file, sm, regex))
	{
		return sm[2];
	}

	return std::string();

}

std::string FileInfo::GetFileDirectory(void) const
{

	std::smatch sm;
	std::regex regex("^(.*[/\\\\])?([^/\\\\]+)$");

	if (std::regex_match(_file, sm, regex))
	{
		return sm[1];
	}

	return std::string(".");

}