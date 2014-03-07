#include "FileInfo.h"

#include <regex>
#include <Windows.h>

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

size_t FileInfo::GetFileSize(void) const
{

	HANDLE hFile = CreateFile(m_file.CString(),
		FILE_READ_ONLY,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return (hFile != INVALID_HANDLE_VALUE ? ::GetFileSize(hFile, nullptr) : 0);

}