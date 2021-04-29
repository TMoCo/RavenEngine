#pragma once

#include "Logger/Console.h"

#include <memory>



// Pointer Types
template<class T>
using Ptr = std::shared_ptr<T>;

template<class T>
using WeakPtr = std::weak_ptr<T>;




#define RAVEN_ASSERT(condition, ...)   \
	{                                    \
		if(!(condition))                   \
		{                                  \
			LOGE("Assertion Failed : {0}", __VA_ARGS__);   \
			__debugbreak();       \
			exit(-2234);          \
		}                       \
	}





#define NOCOPYABLE(Type)								\
    Type(const Type&) = delete;							\
    Type& operator=(const Type&) = delete;




// Values Defs...
#define SMALL_NUM 1.e-6






// Use to archive enum using cearal archive functions.
template<class T>
struct EnumAsInt
{
	EnumAsInt(T& e)
	{
		value = &e;
	}

	// Serialization Save.
	template<typename Archive>
	void save(Archive& archive) const
	{
		int32_t tmp = static_cast<int32_t>(*value);
		archive(tmp)
	}

	// Serialization Load.
	template<typename Archive>
	void load(Archive& archive)
	{
		int32_t tmp;
		archive(tmp);

		*value = static_cast<T>();
	}

	T* value;
};


