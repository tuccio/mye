#pragma once

#include <string>

namespace mye
{

	namespace core
	{

		class FileInfo
		{

		public:

			FileInfo(const std::string &filepath);
			~FileInfo(void);

			std::string GetFilePath(void) const;

			std::string GetFileName(void) const;
			std::string GetFileDirectory(void) const;

		private:

			std::string m_file;

		};

	}

}
