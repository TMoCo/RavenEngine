#include "StringUtils.h"
#ifdef _WIN32
#include <direct.h>
#define GetCwd _getcwd
#else
#include <unistd.h>
#define GetCwd getcwd
#endif


namespace Raven 
{
	namespace StringUtils
	{
		std::string GetExtension(const std::string& fileName)
		{
			auto pos = fileName.find_last_of('.');
			if (pos != std::string::npos)
				return fileName.substr(pos + 1);
			return "";
		}

		std::string RemoveExtension(const std::string& fileName)
		{
			auto pos = fileName.find_last_of('.');
			if (pos != std::string::npos)
				return fileName.substr(0, pos);
			return fileName;
		}

		std::string GetFileName(const std::string& filePath)
		{
			auto pos = filePath.find_last_of('/');
			if (pos != std::string::npos)
				return filePath.substr(pos + 1);
			return filePath;
		}

		std::string GetCurrentWorkingDirectory()
		{
			char currentPath[FILENAME_MAX];
			if (!GetCwd(currentPath, sizeof(currentPath)))
			{
				return std::string(); // empty string
			}
			currentPath[sizeof(currentPath) - 1] = '\0'; // terminate the string
			return std::string(currentPath);
		}
	};
};
