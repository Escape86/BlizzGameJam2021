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

void Audio::PlayAudio(AudioTracks track)
{
	Mix_PlayChannel(-1, Audio::audioTrackData[track], 0);
}
#pragma endregion

#pragma region Static Member Initialization
std::map<Audio::AudioTracks, Mix_Chunk*> Audio::audioTrackData;
#pragma endregion

std::map<Audio::AudioTracks, const char*> AudioTrackToFilePathMapping =
{

};