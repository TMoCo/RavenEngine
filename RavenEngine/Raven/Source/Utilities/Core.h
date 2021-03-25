
#pragma once
#include "Logger/Console.h"

// rename cereal function names to follow naming convention
#define CEREAL_LOAD_FUNCTION_NAME LoadSerialized
#define CEREAL_SAVE_FUNCTION_NAME SaveSerialized

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