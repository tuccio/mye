#pragma once

#include "String.h"

namespace mye
{

	namespace core
	{

		class FileInfo
		{

		public:

			FileInfo(const String &filepath);
			~FileInfo(void);

			String GetFilePath(void) const;

			String GetFileName(void) const;
			String GetFileDirectory(void) const;

			size_t GetFileSize(void) const;

		private:

			String m_file;

		};

	}

}
