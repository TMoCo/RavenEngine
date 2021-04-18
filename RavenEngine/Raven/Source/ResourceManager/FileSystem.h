#pragma once
#include <string>
#include <cstdint>
#include <memory>
namespace Raven
{
	class FileSystem
	{
	public:
		static std::unique_ptr<uint8_t[]> ReadFile(const std::string& path,int64_t & size);
	};
};