
#include "FileSystem.h"
#include <sys/stat.h>
#include <fstream>
namespace Raven 
{

	std::unique_ptr<uint8_t[]> FileSystem::ReadFile(const std::string& path, int64_t& size)
	{
		auto filePtr = fopen(path.c_str(), "rb");
		
		if (filePtr != nullptr)
		{
			fseek(filePtr, 0, SEEK_END);
			size = ftell(filePtr);
			fseek(filePtr, 0, SEEK_SET);
			std::unique_ptr<uint8_t[]> array(new uint8_t[size]);
			memset(array.get(), 0, size);
			fread(array.get(), sizeof(int8_t) * size, 1, filePtr);
			fclose(filePtr);
			return array;
		}
		size = -1;
		return nullptr;
	}


};
