#include "Audio.h"
#include "Constants.h"

#include <SDL_mixer.h>
#include <iostream>

#pragma region Public Methods
bool Audio::Initialize()
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	//load the tracks
	for (const std::pair<Audio::AudioTracks, const char*>& track : AudioTrackToFilePathMapping)
	{
		Audio::audioTrackData[track.first] = Mix_LoadWAV(track.second);
	}

	return true;
}

bool Audio::ShutDown()
{
	//cleanup any loaded tracks
	for (const std::pair<Audio::AudioTracks, Mix_Chunk*>& track : Audio::audioTrackData)
	{
		Mix_FreeChunk(track.second);
	}
	Audio::audioTrackData.clear();

	return true;
}

void Audio::PlayAudio(AudioTracks track, bool loop)
{
	int channel = Mix_PlayChannel(-1, Audio::audioTrackData[track], loop ? -1 : 0);

	//update volume for this channel based on the track
	Mix_Volume(channel, Audio::audioTrackToChannelMap[track]);
}
#pragma endregion

#pragma region Static Member Initialization
std::map<Audio::AudioTracks, Mix_Chunk*> Audio::audioTrackData;
std::map<Audio::AudioTracks, int> Audio::audioTrackToChannelMap
{
	{ Audio::AudioTracks::BG_MUSIC, BG_MUSIC_VOLUME },
	{ Audio::AudioTracks::PLAYER_HIT, PLAYER_HIT_SOUND_VOLUME }
};

std::map<Audio::AudioTracks, const char*> AudioTrackToFilePathMapping =
{
	{ Audio::AudioTracks::BG_MUSIC, BG_MUSIC_AUDIO_FILEPATH },
	{ Audio::AudioTracks::PLAYER_HIT, PLAYER_HIT_AUDIO_FILEPATH }
};
#pragma endregion