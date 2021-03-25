#pragma once

#include <map>

#pragma region Forward Declarations
struct Mix_Chunk;
#pragma endregion

class Audio
{
public:
	enum AudioTracks
	{
		//add entry to file path mapping when adding track to enum
	};

	static bool Initialize();
	static bool ShutDown();

	static void PlayAudio(AudioTracks track);

	Audio() = delete;

private:
	static std::map<Audio::AudioTracks, Mix_Chunk*> audioTrackData;
};

extern std::map<Audio::AudioTracks, const char*> AudioTrackToFilePathMapping;