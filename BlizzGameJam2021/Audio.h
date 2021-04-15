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
		BG_MUSIC,
		PLAYER_HIT
	};

	static bool Initialize();
	static bool ShutDown();

	static void PlayAudio(AudioTracks track, bool loop);

	Audio() = delete;

private:
	static std::map<Audio::AudioTracks, Mix_Chunk*> audioTrackData;
	static std::map<Audio::AudioTracks, int> audioTrackToChannelMap;
};

extern std::map<Audio::AudioTracks, const char*> AudioTrackToFilePathMapping;