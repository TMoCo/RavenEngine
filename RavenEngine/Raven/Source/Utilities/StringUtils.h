#pragma once
#include <string>

namespace Raven
{
	namespace StringUtils 
	{
		// later we should use enums to identify formats for faster comparison
		enum EFileExtensions : uint32_t
		{
			Txt,
			Obj,
			Fbx
		};

		enum EBase : uint32_t
		{
			Decimal		= 10,
			Hexadecimal = 16,
			Binary		= 2
		};

		std::string GetExtension(const std::string& fileName);
		std::string RemoveExtension(const std::string& fileName);
		std::string GetFileName(const std::string& filePath);
		std::string GetCurrentWorkingDirectory();
		void Replace(std::string& src, const std::string& origin, const std::string& des);
		bool IsHiddenFile(const std::string& path);
		bool IsTextFile(const std::string& filePath);
		bool IsLuaFile(const std::string& filePath);
		bool IsAudioFile(const std::string& filePath);
		bool IsSceneFile(const std::string& filePath);
		bool IsModelFile(const std::string& filePath);
		bool IsTextureFile(const std::string& filePath);
		void Trim(std::string& str);

		char* IntToString(int num, char* buffer, EBase base); // itoa
	};

};
