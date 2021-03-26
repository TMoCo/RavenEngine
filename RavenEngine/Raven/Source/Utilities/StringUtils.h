#pragma once
#include <string>

namespace Raven
{
	namespace StringUtils 
	{
		std::string GetExtension(const std::string& fileName);
		std::string RemoveExtension(const std::string& fileName);
		std::string GetFileName(const std::string& filePath);
		bool IsHiddenFile(const std::string& path);

		bool IsTextFile(const std::string& filePath);
		bool IsAudioFile(const std::string& filePath);
		bool IsSceneFile(const std::string& filePath);
		bool IsModelFile(const std::string& filePath);
		bool IsTextureFile(const std::string& filePath);


	};

};
