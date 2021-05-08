#pragma once

#include <cstdint>

namespace Raven
{
	struct AudioData
	{
		uint8_t * data = nullptr;
		float	freqRate = 0.0f;
		double	length = 0.0;
		uint32_t bitRate = 0;
		uint32_t size = 0;
		uint32_t channels = 0;
	};
}
