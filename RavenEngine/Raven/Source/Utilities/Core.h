#pragma once

#include "Logger/Console.h"

#include <memory>



// Pointer Types
template<class T>
using Ptr = std::shared_ptr<T>;

template<class T>
using WeakPtr = std::weak_ptr<T>;




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




// Values Defs...
#define SMALL_NUM 1.e-6

