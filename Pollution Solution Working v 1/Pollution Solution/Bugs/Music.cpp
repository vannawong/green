#include "fmod.h"
#include "bugs_VS\stdafx.h"
#include "bugs\Music.h"

Music::Music () {};

void Music::playMusic (const char* songName) {
	//INIT THE MUSIC
	FMOD_SYSTEM *system;
	FMOD_SOUND *audiostream;
	unsigned int version;
	FMOD_RESULT res;
	int numDrivers = 0;

	printf ("Starting fmod");

	res = FMOD_System_Create (&system);
	if (res != FMOD_OK)
		printf ("system create failed");

	// Check version
	res= FMOD_System_GetVersion(system, &version);
 
	FMOD_System_GetNumDrivers (system, &numDrivers);
	// No sound cards (disable sound)
	if (numDrivers == 0)
	{
		res = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_NOSOUND);
		if (res != FMOD_OK)
			printf ("no sound card");
	}

	// Get the capabilities of the default (0) sound card
    //res = FMOD_System_GetDriver(system, 0, &caps, 0, &speakerMode);
 
    // Set the speaker mode to match that in Control Panel
    //res = FMOD_System_SetSpeakerPosition(speakerMode);

	FMOD_CHANNELGROUP *channelMusic = NULL;
	FMOD_CHANNEL *songchan = NULL;

	const char* path = "data\\music\\";
	//path + songName;

	res = FMOD_System_Init (system, 100, FMOD_INIT_NORMAL, 0);
	if (res != FMOD_OK)
		printf ("system init failed");
	//FMOD_System_CreateSound (system, ".mp3", FMOD_DEFAULT, 0, &audio);
	res = FMOD_System_CreateStream (system, songName, FMOD_DEFAULT, 0, &audiostream);
	if (res != FMOD_OK)
		printf ("system createStream failed");
	//FMOD_System_PlaySound(system,audiostream,0,false,0);
	FMOD_Channel_SetChannelGroup (songchan, channelMusic);
	FMOD_System_PlaySound(system,audiostream,channelMusic,false,&songchan);

	/*FMOD_CHANNELGROUP *channelMusic;
	//FMOD_CHANNELGROUP *channelEffects;
	FMOD_CHANNEL *songchan;
	
	FMOD_System_PlaySound(system,audiostream,channelMusic,false,&songchan);
	FMOD_Channel_SetChannelGroup(songchan, channelMusic);*/
}