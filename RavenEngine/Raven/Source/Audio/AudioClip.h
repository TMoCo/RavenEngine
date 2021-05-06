#pragma once

#include "AudioData.h"
#include <string>
#include <LuaBridge/RefCountedPtr.h>

namespace Raven
{

	/**
	 * interface for different Audio Imp
	 * Now only have openal implementation
	 */

	class  AudioClip
	{
		friend class SoundManager;

	public:
		static AudioClip* Create(const std::string& name, const std::string& extension);
		static luabridge::RefCountedPtr<AudioClip> CreateLuaObj(const std::string& name);
		virtual ~AudioClip();

		inline auto GetData() const
		{
			return data.data;
		}
		int GetBitRate() const
		{
			return data.bitRate;
		}
		inline auto GetFrequency() const
		{
			return data.freqRate;
		}
		inline auto GetChannels() const
		{
			return data.channels;
		}
		inline auto GetSize() const
		{
			return data.size;
		}
		inline auto IsStreaming() const
		{
			return streaming;
		}
		double GetLength() const;

		virtual double StreamData(unsigned int buffer, double timeLeft){return 0.0f;}
		inline auto& GetFilePath() const { return filePath; }

	protected:
		AudioClip();
		bool streaming = false;
		std::string filePath;
		AudioData data;
	};
}
