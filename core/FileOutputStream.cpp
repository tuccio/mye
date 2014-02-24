#include "FileOutputStream.h"
#include "DataEncoding.h"

using namespace mye::core;

FileOutputStream mye::core::StandardOutput(stdout);

FileOutputStream::FileOutputStream(void) :
	m_file(nullptr)
{
}

FileOutputStream::FileOutputStream(FILE *file) :
	m_file(file)
{
}

FileOutputStream::FileOutputStream(const String &file)
{
	m_file = fopen(file.CString(), "wb");
	
}

FileOutputStream::~FileOutputStream(void)
{
}

size_t FileOutputStream::Write(const void *data, size_t size)
{
	return fwrite(data, 1, size, m_file);
}