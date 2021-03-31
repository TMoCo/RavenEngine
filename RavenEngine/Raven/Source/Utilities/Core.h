
#pragma once

#include <memory>

#include "Logger/Console.h"

template<class T>
using Ptr = std::shared_ptr<T>;

#define RAVEN_ASSERT(condition, ...)								\
	{																\
		if(!(condition))											\
		{															\
			LOGE("Assertion Failed : {0}", __VA_ARGS__);			\
			__debugbreak(); 										\
		}															\
	}

#define NOCOPYABLE(Type)								\
    Type(const Type&) = delete;							\
    Type& operator=(const Type&) = delete;