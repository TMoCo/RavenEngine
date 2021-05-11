#include "ALAudioClip.h"
#include "Audio/OggLoader.h"

namespace Raven
{
	ALAudioClip::ALAudioClip(const std::string& fileName, const std::string& format)
		: format(0)
	{
		filePath = fileName;
		if(format == "ogg")
			data = LoadOgg(fileName);

		alGenBuffers(1, &buffer);
		alBufferData(buffer, GetOALFormat(data.bitRate, data.channels), data.data, data.size, static_cast<ALsizei>(data.freqRate));
	}

	ALAudioClip::~ALAudioClip()
	{
		alDeleteBuffers(1, &buffer);
	}

	ALenum ALAudioClip::GetOALFormat(uint32_t bitRate, uint32_t channels)
	{
		if(bitRate == 16)
		{
			return channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		}
		else if(bitRate == 8)
		{
			return channels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		}
		return AL_FORMAT_MONO8;
	}
}
