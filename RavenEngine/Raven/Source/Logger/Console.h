

#pragma once
#include <iostream>
#include <string>
#include <stdarg.h>
#include <spdlog/spdlog.h>

namespace Raven 
{
	class Console
	{
	public:
		static auto Init() -> void;
		static auto & GetLogger() { return logger; }
	private:
		static std::shared_ptr<spdlog::logger> logger;
	};
};


#define PRINT_FUNC()   Raven::Console::GetLogger()->info("File {0} : Line : {1}",__FUNCTION__,__LINE__)	
#define LOGV(...)      Raven::Console::GetLogger()->trace(__VA_ARGS__)
#define LOGI(...)      Raven::Console::GetLogger()->info(__VA_ARGS__)
#define LOGW(...)      Raven::Console::GetLogger()->warn(__VA_ARGS__)
#define LOGE(...)      Raven::Console::GetLogger()->error(__VA_ARGS__)
#define LOGC(...)      Raven::Console::GetLogger()->critical(__VA_ARGS__)



