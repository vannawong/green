/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Game.cpp

	See Game.h for a class description.
*/
#include "sssf_VS\stdafx.h"
#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Button.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"
#include "sssf\text\TextFileWriter.h"
#include "sssf\text\TextGenerator.h"
#include "sssf\timer\GameTimer.h"
#include "fmod.h"

/*
	Game  - Constructor, this method begins the 
	construction and loading of all major game objects. 
	This method only needs to be called once, when the 
	application is first started.

	NOTE: This constructor will not initialize the custom
	game objects: graphics, input, os, & timer.
	These must be constructed separately and fed to this
	object using the init method. Doing it this way allows for
	more platform independence.
*/
Game::Game()
{
	// NOTE THAT graphics, input, os, & timer
	// ARE CUSTOM GAME OBJECTS. DEPENDING ON WHAT TECHNOLOGY 
	// IS TO BE USED THESE OBJECT SHOULD BE CONSTRUCTED
	// AND THEN FED TO THIS Game USING THE init METHOD
	initMusic();
	gsm = new GameStateManager();
	gui = new GameGUI();
	text = new GameText();
}

/*
	~Game - Desctructor, it recovers memory allocated for
	the game objects. If Game is deleted, the application
	is closing, so everything should be cleaned up. Note that
	this 
*/
Game::~Game() 
{
	// THESE WERE CREATED BY Game, AND SO SHOULD BE
	// DELETED BY GAME
	delete gsm;
	delete gui;
	delete text;

	// THE GAME AND PLATFORM STUFF SHOULD BE DELETED
	// BY WHOEVER CREATED THEM
}

/*
	initplatforms - This method is to be used to feed this Game
	the constructed, technology-specific, objects. 
*/
void Game::initplatforms(		GameGraphics *initGraphics,
									GameInput *initInput,
									GameOS *initOS,									
									GameTimer *initTimer)
{
	// INITIALIZE ALL OF THE GAME'S CUSTOM OBJECTS
	graphics = initGraphics;
	input = initInput;
	os = initOS;
	timer = initTimer;
}

/*
	reloadAllDevices - Windows applications must cooperate
	with other running applications, so when someone hits
	ALT-TAB and switches from a full-screen game, it might
	lose ownership of the graphics card. This method can
	be called when a full-screen application retains ownership
	of all necessary resources such that all necessary
	data (like textures, sound, music, etc.) can be reloaded.
*/
void Game::reloadAllDevices()
{
	graphics->reloadGraphics();

	// WE MIGHT ADD MORE LATER
}
	
/*
	runGameLoop - This is the game loop management method.
	It runs continuously while the game is active. Once per
	frame it instructs the major game objects to get
	user input, record user input, update the GUI state, 
	update the sprites' states using AI and input, perform
	collision detection and resolution (physics), render
	the screen, etc.

	This loop is timed such that everything is kept to a 
	consistent framerate, thus the game should run 
	consistently on all machines.
*/
void Game::runGameLoop()
{
	// FIRST PROFILE?
	bool firstTimeThroughLoop = true;

	// LET'S START THE TIMER FROM SCRATCH
	timer->resetTimer();
	curchan = playMusic ("data\\music\\planetarium.mp3");

	// KEEP RENDERING UNTIL SOMEONE PULLS THE PLUG
	while(gsm->isAppActive())
	{
		// MOVE ALONG WINDOWS MESSAGES, THIS ALLOWS
		// US TO GET USER INPUT
		os->processOSMessages();

		// GET USER INPUT AND UPDATE GAME, GUI, OR PLAYER
		// STATE OR WHATEVER IS NECESSARY
		input->processInput(this);

		// IT IS POSSIBLE THE USER REQUESTED A SHUTDOWN
		// OF THE APP, SO WE NEED TO CHECK AGAIN
		if (gsm->isAppActive())
		{
			// USE THE INPUT TO UPDATE THE GAME
			processGameData();

			// AND RENDER THE GAME
			graphics->renderGame(this);
		}
	}
}

/*
	processGameData - This method directs game logic to be
	executed or not, depending on the game state, it addition
	it triggers the building or render lists and game loop
	timing each frame.
*/
void Game::processGameData()
{
	// WE ONLY PERFORM GAME LOGIC IF THE GAME
	// IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		gsm->update(this);
		FMOD_System_Update (system);
	}
	else if (gsm->isGameLevelLoading())
	{
		gsm->goToGame();
	}

	// UPDATE TEXT
	TextGenerator *tg = text->getTextGenerator();
	tg->updateText(this);

	// BUILD THE RENDER LISTS
	graphics->fillRenderLists(this);

	// KEEP THE FRAME RATE CONSISTENT
	timer->timeGameLoop();
}

/*
	quitGame - When called, this method will force the unloading of all game data
	loaded for the current level and then return the user to the main menu.
*/
void Game::quitGame()
{
	// CLEAN UP ALL THE WORLD TEXTURES
	graphics->clearWorldTextures();
	gsm->getSpriteManager->clearSprites();
	gsm->getWorld()->unloadWorld();

	// WE'RE GOING BACK TO THE MAIN MENU
	gsm->goToMainMenu();
}

/*
	shutdown - This method starts the termination of the game application, 
	cascading shutdown proceedings through all the necessary classes, resulting
	in the release of all game resources.
*/
void Game::shutdown()
{
	// MAKE SURE THE GAME LOOP ENDS AND THAT THE GSM CLEANS
	// UP ALL THE MEMORY IT IS USING
	gsm->shutdown();

	// CLEAR OUT AND RELEASE THE GPU
	graphics->shutdown();

	// RELEASE GAME CONTROLLERS IF NECESSARY
	input->shutdown();

	// WE MAY SHUTDOWN OTHER THINGS HERE LIKE SOUND & MUSIC
	// RESOURCES IN THE FUTURE
	// AND KILL THE WINDOW
	os->shutdown();
}

/*
	startGame - When called, this method will load the first level of the
	game and start play. Note that one must make sure the currentLeve
*/
void Game::startGame()
{
	// AND UPDATE THE GAME STATE OF COURSE
	gsm->goToLoadLevel();	// NOTE THAT CURRENTLY THERE IS NO LEVEL FILE,
	
	// THAT'S ONE THING YOU'LL BE DOING
	dataLoader->loadWorld(this, currentLevelFileName);
}

//MUSIC METHODS

void Game::initMusic() {
	FMOD_System_Create (&system);
	FMOD_System_Init (system, 100, FMOD_INIT_NORMAL, 0);
	FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_NOSOUND);
	songMusic = NULL;
	soundMusic = NULL;
	curchan = NULL;
	cursong = NULL;
	
	/*	unsigned int version;
	int numDrivers = 0;

	FMOD_System_GetVersion(system, &version);
 
	FMOD_System_GetNumDrivers (system, &numDrivers);
	if (numDrivers == 0)
	{
		FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_NOSOUND);
	}
	*/
}

FMOD_CHANNEL* Game::playMusic (const char* songName) {
	FMOD_SOUND *audiostream;
	songchan = NULL;

	FMOD_System_CreateStream (system, songName, FMOD_DEFAULT, 0, &audiostream);
	FMOD_Channel_SetChannelGroup (songchan, songMusic);
	FMOD_System_PlaySound(system,audiostream,songMusic,false,&songchan);
	cursong = audiostream;
	return songchan;
}

/*void Game::playMusic(const char* songName) {
	//INIT THE MUSIC
	FMOD_SYSTEM *system;
	FMOD_SOUND *audiostream;
	unsigned int version;
	FMOD_RESULT res;
	int numDrivers = 0;

	printf ("Starting fmod");

	res = FMOD_System_Create (&system);

	// Check version
	res= FMOD_System_GetVersion(system, &version);
 
	FMOD_System_GetNumDrivers (system, &numDrivers);
	// No sound cards (disable sound)
	if (numDrivers == 0)
	{
		res = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_NOSOUND);
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
	res = FMOD_System_CreateStream (system, songName, FMOD_DEFAULT, 0, &audiostream);
	FMOD_Channel_SetChannelGroup (songchan, channelMusic);
	FMOD_System_PlaySound(system,audiostream,channelMusic,false,&songchan);

	/*FMOD_CHANNELGROUP *channelMusic;
	//FMOD_CHANNELGROUP *channelEffects;
	FMOD_CHANNEL *songchan;
	
	FMOD_System_PlaySound(system,audiostream,channelMusic,false,&songchan);
	FMOD_Channel_SetChannelGroup(songchan, channelMusic);
}*/

void Game::stopMusic (FMOD_CHANNEL* channel) {
	FMOD_Channel_Stop (channel);
}

void Game::playSound (const char* soundName) {
	FMOD_SOUND *audiostream;
	FMOD_CHANNEL *soundchan = NULL;

	FMOD_System_CreateStream (system, soundName, FMOD_DEFAULT, 0, &audiostream);
	FMOD_Channel_SetChannelGroup (soundchan, soundMusic);
	FMOD_System_PlaySound(system,audiostream,soundMusic,false,&soundchan);
}