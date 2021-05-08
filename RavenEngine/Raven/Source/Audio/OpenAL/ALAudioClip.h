#pragma once



#include "Audio/AudioClip.h"

#include <AL/al.h>

namespace Raven
{
	class ALAudioClip : public AudioClip
	{
	public:
		ALAudioClip(const std::string& fileName, const std::string& format);
		virtual ~ALAudioClip();
		inline auto GetBuffer() const { return buffer;}
	private:
		static ALenum GetOALFormat(uint32_t bitRate, uint32_t channels);
		uint32_t buffer = 0;
		int32_t format = 0;
	};
}
