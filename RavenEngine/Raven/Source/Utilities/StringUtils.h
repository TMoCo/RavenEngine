#pragma once
#include <string>

namespace Raven
{
	namespace StringUtils 
	{
		std::string GetExtension(const std::string& fileName);
		std::string RemoveExtension(const std::string& fileName);
		std::string GetFileName(const std::string& filePath);
		std::string GetCurrentWorkingDirectory();
	};

};
