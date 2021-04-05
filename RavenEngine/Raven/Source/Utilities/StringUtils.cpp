#include <cstdlib>
#include <algorithm>

#include "Utilities/Core.h"
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

			pos = filePath.find_last_of('\\');
			if (pos != std::string::npos)
				return filePath.substr(pos + 1);

			return filePath;
		}

		void Replace(std::string& src, const std::string& origin, const std::string& des) 
		{
			std::string::size_type pos = 0;
			std::string::size_type srcLen = origin.size();
			std::string::size_type desLen = des.size();
			pos = src.find(origin, pos);
			while ((pos != std::string::npos))
			{
				src.replace(pos, srcLen, des);
				pos = src.find(origin, (pos + desLen));
			}
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

		bool IsHiddenFile(const std::string& path)
		{
			if (path != ".." &&
				path != "." &&
				path[0] == '.')
			{
				return true;
			}
			return false;
		}


		bool IsTextFile(const std::string& filePath)
		{
			std::string extension = GetExtension(filePath);
			Trim(extension);
			if (extension == "txt" || extension == "glsl" || extension == "shader" || extension == "vert"
				|| extension == "frag" || extension == "lua" || extension == "Lua")
				return true;

			return false;
		}

		bool IsLuaFile(const std::string& filePath)
		{
			std::string extension = GetExtension(filePath);
			Trim(extension);
			return extension == "lua" || extension == "Lua";
		}

		bool IsAudioFile(const std::string& filePath)
		{
			std::string extension = GetExtension(filePath);
			Trim(extension);
			return extension == "ogg" || extension == "wav" || extension == "mp3";
		}

		bool IsSceneFile(const std::string& filePath)
		{
			std::string extension = GetExtension(filePath);
			Trim(extension);
			return extension == "raven";
			
		}

		bool IsModelFile(const std::string& filePath)
		{
			std::string extension = GetExtension(filePath);
			Trim(extension);
			return extension == "obj" || extension == "gltf" || extension == "glb" || extension == "fbx" || extension == "FBX";
		}

		bool IsTextureFile(const std::string& filePath)
		{
			std::string extension = GetExtension(filePath);
			Trim(extension);
			return extension == "png" || extension == "tga" || extension == "jpg";
		}

		void Trim(std::string& str)
		{
			if (!str.empty())
			{
				str.erase(0, str.find_first_not_of(" "));
				str.erase(str.find_last_not_of(" ") + 1);
			}
		}

		char* IntToString(int num, char* buffer, EBase base)
		{
			int i = 0;
			// check for 0 case
			if (num == 0)
			{
				buffer[i++] = '0';
				buffer[i]   = '\0';
				return buffer;
			}
			// detect negative numbers for base 10
			bool negative = false;
			if (num < 0 && base == 10)
			{
				num = -num;
				negative = true;
			}
			// loop till all digits have been processed
			while (num != 0)
			{
				int remainder = num % base; // determine digit
				buffer[i++] = remainder > 9 ? remainder - 10 + 'a' : remainder + '0';
				num /= base; // integer division by base changes to next digit
			}
			if (negative)
			{
				buffer[i++] = '-';
			}
			buffer[i] = '\0';
			std::reverse(buffer, buffer + strlen(buffer));
			return buffer;
		}
	};
};
