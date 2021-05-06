

#include "AudioClip.h"
#include "OpenAL/ALAudioClip.h"
#include "Utilities/StringUtils.h"
#include "Logger/Console.h"
namespace Raven
{
	AudioClip::AudioClip()
	{
	}

	AudioClip::~AudioClip()
	{
		delete[] data.data;
	}

	AudioClip* AudioClip::Create(const std::string& name, const std::string& extension)
	{
		return new ALAudioClip(name, extension);
	}

	luabridge::RefCountedPtr<AudioClip> AudioClip::CreateLuaObj(const std::string& name)
	{
		return new ALAudioClip(name, StringUtils::GetExtension(name));
	}

	double AudioClip::GetLength() const
	{
		return data.length;
	}
}
