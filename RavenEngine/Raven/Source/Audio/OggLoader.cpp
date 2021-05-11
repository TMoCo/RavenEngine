
#include "OggLoader.h"

#include <stb/stb_vorbis.h>
#include "Logger/Console.h"


namespace Raven
{
	AudioData LoadOgg(const std::string& fileName)
	{
		AudioData data = AudioData();

		const auto fileHandle = fopen(fileName.c_str(), "rb");

		if(!fileHandle)
		{
			LOGC("Failed to load OGG file : {0}", fileName);
		}
        int32_t error;
		auto streamHandle = stb_vorbis_open_filename(fileName.c_str(), &error, nullptr);

		if(!streamHandle)
		{
			LOGC("Failed to load OGG file : {0} , Error {1}", fileName, error);
            return data;
		}

		// Get file info
		const stb_vorbis_info m_VorbisInfo = stb_vorbis_get_info(streamHandle);
		data.channels = m_VorbisInfo.channels;
		data.bitRate = 16;
		data.freqRate = static_cast<float>(m_VorbisInfo.sample_rate);

		const uint32_t dataSize = stb_vorbis_stream_length_in_samples(streamHandle) * m_VorbisInfo.channels * sizeof(int16_t);
		auto* buffer = static_cast<int16_t*>(malloc(dataSize * sizeof(uint16_t)));
		stb_vorbis_get_samples_short_interleaved(streamHandle, m_VorbisInfo.channels, static_cast<short*>(buffer), dataSize);
		data.data = reinterpret_cast<unsigned char*>(buffer);
		data.size = dataSize;
		data.length = stb_vorbis_stream_length_in_seconds(streamHandle) * 1000.0f;
		stb_vorbis_close(streamHandle);
		fclose(fileHandle);
		return data;
	}
}
