#include "bugs_VS\stdafx.h"

// Bugs GAME INCLUDES
#include "bugs\BugsButtonEventHandler.h"
#include "bugs\BugsDataLoader.h"
#include "bugs\BugsGame.h"
#include "bugs\BugsKeyEventHandler.h"
#include "bugs\BugsTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\ai\bots\GarbageMon.h"
#include "sssf\gsm\ai\bots\NPC.h"
#include "sssf\gsm\sprite\TopDownSprite.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"
#include "sssf\gui\Viewport.h"

// WINDOWS PLATFORM INCLUDES
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

// DIRECTX INCLUDES
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"

// TMX MAP LOADING
#include "tmxmi\TMXMapImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "psti\PoseurSpriteTypesImporter.h"

#include "fmod.h"

#include "sssf\gsm\ai\BotRecycler.h"

#include "LuaPlusFramework\LuaPlus.h"
using namespace LuaPlus;
#include <locale>
#include <codecvt>
#include <string>

#include "Box2D\Box2D.h"

/*
	loadGame - This method loads the setup game data into the game and
	constructs all the needed objects for the game to work.
*/
void BugsDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[W_TITLE];
	wstring useFullscreenProp = (*properties)[W_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	//BotRecycler *recylcler = new BotRecycler();

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *BugsOS = new WindowsOS(	hInstance, 
										nCmdShow,
										useFullscreen,
										titleProp,
										screenWidth, screenHeight,
										game);
	
	int textFontSize;
	wstring textFontSizeProp = (*properties)[W_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *BugsGraphics = new DirectXGraphics(game);
	BugsGraphics->init(screenWidth, screenHeight);
	BugsGraphics->initGraphics(BugsOS, useFullscreen);
	BugsGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(BugsGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *BugsInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *BugsTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initplatforms(	(GameGraphics*)BugsGraphics,
								(GameInput*)BugsInput,
								(GameOS*)BugsOS,
								(GameTimer*)BugsTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	BugsTextGenerator *bugsTextGenerator = new BugsTextGenerator();
	bugsTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(W_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)bugsTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);	

	//FMOD_CHANNEL* ch = NULL;
	//game->playMusic ("data\\music\\planetarium.mp3");

	//INIT THE MUSIC
	//FMOD_SOUND *s;
	//FMOD_SYSTEM *system;
	//FMOD_SOUND *audio;
	/*FMOD_SOUND *audiostream;
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

	res = FMOD_System_Init (system, 100, FMOD_INIT_NORMAL, 0);
	if (res != FMOD_OK)
		printf ("system init failed");
	//FMOD_System_CreateSound (system, ".mp3", FMOD_DEFAULT, 0, &audio);
	res = FMOD_System_CreateStream (system, "data\\music\\planetarium.mp3", FMOD_DEFAULT, 0, &audiostream);
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

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;
}

/*
	initColors - this helper method loads the color key, used for loading
	images, and the font color, used for rendering text.
*/
void BugsDataLoader::initColors(	GameGraphics *graphics,
									map<wstring,wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[W_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[W_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[W_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[W_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[W_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[W_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
	loadGUI - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
*/
void BugsDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	// WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
	hardCodedLoadGUIExample(game);
}

/*
	loadLevel - This method should load the data the level described by the
	levelInitFile argument in to the Game's game state manager.
*/
void BugsDataLoader::loadWorld(Game *game, wstring levelInitFile)	
{

	string luaFile(levelInitFile.begin(), levelInitFile.end());
	luaFile.assign(levelInitFile.begin(), levelInitFile.end());
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	LuaState* luaPState = LuaState::Create();
	luaPState->DoFile(luaFile.c_str());

	string hi(luaPState->GetGlobal("W_LEVEL_DIR").GetString());
	std::wstring W_LEVEL_DIR = converter.from_bytes(hi);

	hi = luaPState->GetGlobal("W_LEVEL_NAME").GetString();
	std::wstring W_LEVEL_NAME = converter.from_bytes(hi);

	float PLAYER_INIT_X = (float) luaPState->GetGlobal("PLAYER_INIT_X").GetInteger();
	float PLAYER_INIT_Y = (float) luaPState->GetGlobal("PLAYER_INIT_Y").GetInteger();

	float BOT_1_INIT_X = (float) luaPState->GetGlobal("BOT_1_INIT_X").GetInteger();
	float BOT_1_INIT_Y = (float) luaPState->GetGlobal("BOT_1_INIT_Y").GetInteger();

	float BOT_2_INIT_X = (float) luaPState->GetGlobal("BOT_2_INIT_X").GetInteger();
	float BOT_2_INIT_Y = (float) luaPState->GetGlobal("BOT_2_INIT_Y").GetInteger();

	LuaState::Destroy(luaPState);

	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, W_LEVEL_DIR, W_LEVEL_NAME);

	// SPECIFY WHO WILL DO THE PATHFINDING
	GameStateManager *gsm = game->getGSM();
	World *world = gsm->getWorld();
	SpriteManager *spriteManager = gsm->getSpriteManager();
	
	// LOAD THE LEVEL'S SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LET'S MAKE A PLAYER SPRITE
	// @TODO - LATER WE'LL LOAD ALL LEVEL DATA FROM A FILE
	Physics *physics = gsm->getPhysics();
	TopDownSprite *player = spriteManager->getPlayer();
	physics->addCollidableObject(player);
	player->setRotationInRadians(0.0f);

	b2World* bworld = game->getbworld();

	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position.Set(PLAYER_INIT_X, PLAYER_INIT_Y);
	b2Body* body = bworld->CreateBody(&bdef);
	body->SetAngularDamping (1.0f);
	body->SetFixedRotation (true);

	//body->SetLinearVelocity(b2Vec2 (0.0f, 0.0f));

	b2PolygonShape dynamicBox; 
	dynamicBox.SetAsBox(1.0f, 1.0f); 

	b2FixtureDef fixtureDef; 
	fixtureDef.shape = &dynamicBox; 
	fixtureDef.density = 18.0f; 
	fixtureDef.friction = 200.0f;

	body->CreateFixture(&fixtureDef);
	Physics* p = gsm->getPhysics();

	player->setBody(body);
	p->addCO(player);

	//FooDraw drawer;
	//bworld->SetDebugDraw( &drawer );
	//drawer.SetFlags( b2Draw::e_shapeBit );

	// NOTE THAT RED BOX MAN IS SPRITE ID 1
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(0);
	player->setSpriteType(playerSpriteType);
	player->setAlpha(255);
	player->setCurrentState(IDLE);
	PhysicalProperties *playerProps = player->getPhysicalProperties();
	playerProps->setX(PLAYER_INIT_X);
	playerProps->setY(PLAYER_INIT_Y);
	playerProps->setVelocity(0.0f, 0.0f);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);
	player->setOnTileThisFrame(false);
	player->setOnTileLastFrame(false);
	player->affixTightAABBBoundingVolume();

	// MAKING A HEALTH BAR
	AnimatedSpriteType *healthBarSpriteType = spriteManager->getSpriteType(4);
	AnimatedSprite *healthBar = spriteManager->getHealthBar();
	healthBar->setSpriteType(healthBarSpriteType);
	healthBar->setAlpha(255);
	healthBar->setCurrentState(L"100");
	PhysicalProperties *healthprops = healthBar->getPhysicalProperties();
	healthprops->setX(HEALTH_INIT_X);
	healthprops->setY(HEALTH_INIT_Y);

	//NPC
	AnimatedSpriteType *npcSpriteType = spriteManager->getSpriteType(1);
	makeNPC(game, npcSpriteType, 300, 500);

	AnimatedSpriteType *botSpriteType = spriteManager->getSpriteType(2);


	// AND LET'S ADD A BUNCH OF RANDOM JUMPING BOTS, FIRST ALONG
	// A LINE NEAR THE TOP

// UNCOMMENT THE FOLLOWING CODE BLOCK WHEN YOU ARE READY TO ADD SOME BOTS
/*	for (int i = 2; i <= 26; i++)
	{
		float botX = 400.0f + (i * 100.0f);
		float botY = 100.0f;
		makeRandomJumpingBot(game, botSpriteType, botX, botY);
	}*/

	// AND THEN STRATEGICALLY PLACED AROUND THE LEVEL
	makeGarbageMon(game, botSpriteType, BOT_1_INIT_X, BOT_1_INIT_Y);
	makeGarbageMon(game, botSpriteType, BOT_2_INIT_X, BOT_2_INIT_Y);
/*	makeRandomJumpingBot(game, botSpriteType, 400, 400);
	makeRandomJumpingBot(game, botSpriteType, 800, 700);
	makeRandomJumpingBot(game, botSpriteType, 900, 700);
	makeRandomJumpingBot(game, botSpriteType, 1000, 700);
	makeRandomJumpingBot(game, botSpriteType, 100, 1000);
	makeRandomJumpingBot(game, botSpriteType, 300, 1000);	
	makeRandomJumpingBot(game, botSpriteType, 500, 1000);
	makeRandomJumpingBot(game, botSpriteType, 100, 1400);
	makeRandomJumpingBot(game, botSpriteType, 400, 1400);	
	makeRandomJumpingBot(game, botSpriteType, 700, 1400);

	// AND THEN A BUNCH LINED UP NEAR THE LEVEL EXIT
	for (int i = 0; i < 14; i++)
		makeRandomJumpingBot(game, botSpriteType, 1700.0f + (i*100.0f), 1300.0f);
*/		
}

void BugsDataLoader::makeNPC(Game *game, AnimatedSpriteType *npcSpriteType, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();
	NPC *npc = new NPC(physics, 0, 0, 0);
	physics->addCollidableObject(npc);
	PhysicalProperties *pp = npc->getPhysicalProperties();
	pp->setPosition(initX, initY);
	npc->setSpriteType(npcSpriteType);
	npc->setCurrentState(IDLE);
	npc->setAlpha(255);
	spriteManager->addBot(npc);
	npc->affixTightAABBBoundingVolume();
}


void BugsDataLoader::makeGarbageMon(Game *game, AnimatedSpriteType *garbageMonType, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();
	GarbageMon *bot = new GarbageMon(physics, 30, 120, 40);
	physics->addCollidableObject(bot);
	PhysicalProperties *pp = bot->getPhysicalProperties();
	pp->setPosition(initX, initY);
	bot->setSpriteType(garbageMonType);
	pp->setVelocity(0, 0);
	bot->setCurrentState(IDLE);
	bot->setAlpha(255);
	spriteManager->addBot(bot);
	bot->affixTightAABBBoundingVolume();
	//recycler->registerBotType(L"garbageMon", bot); 
	
	b2BodyDef bdef;
	bdef.type = b2_kinematicBody;
	bdef.position.Set (initX, initY);
	b2World* bworld = game->getbworld();
	b2Body* body = bworld->CreateBody (&bdef);

	b2PolygonShape dynamicBox; 
	dynamicBox.SetAsBox(1.0f, 1.0f); 

	b2FixtureDef fixtureDef; 
	fixtureDef.shape = &dynamicBox; 
	fixtureDef.density = 1.0f; 
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.0f;

	body->CreateFixture (&fixtureDef);
	bot->setBody(body);
	game->getGSM()->getPhysics()->addCO(bot);
}


/*
	initBugsGUI - This method builds a GUI for the Bugs Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void BugsDataLoader::hardCodedLoadGUIExample(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
	initQuestsScreen(gui, guiTextureManager);
}

/*
	initCursor - initializes a simple little cursor for the gui.
*/
void BugsDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(W_TRASH_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	imageID = guiTextureManager->loadTexture(W_RED_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);
}

/*
	initSplashScreen - initializes the game's splash screen gui.
*/
void BugsDataLoader::initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							game->getGraphics()->getScreenWidth(),
							game->getGraphics()->getScreenHeight(),
							false,
							W_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
	initMainMenu - initializes the game's main menu gui.
*/
void BugsDataLoader::initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 1366;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);
	
	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							1000,
							200,
							0,
							255,
							200,
							100,
							false,
							W_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_START_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_START_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							150,
							200,
							0,
							255,
							200,
							100,
							false,
							W_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture(W_CHEAT_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_CHEAT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							150,
							500,
							0,
							255,
							200,
							100,
							false,
							W_CHEAT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU,		mainMenuGUI);
}

/*
	initInGameGUI - initializes the game's in-game gui.
*/
void BugsDataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							200,
							100,
							false,
							W_QUIT_COMMAND);
	inGameGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS,	inGameGUI);
}

void BugsDataLoader::initQuestsScreen(GameGUI *gui, DirectXTextureManager *guiTextureManager){
		// NOW ADD THE IN-GAME GUI
	ScreenGUI *questsScreen = new ScreenGUI();

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUEST_SCREEN_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 100	;
	imageToAdd->y = 200;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 1123;
	imageToAdd->height = 517;
	imageToAdd->imageID = normalTextureID;
	questsScreen->addOverlayImage(imageToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_QUEST_SCREEN,	questsScreen);
}

/*
	initViewport - initializes the game's viewport.
*/
void BugsDataLoader::initViewport(GameGUI *gui, map<wstring,wstring> *properties)
{
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, toggleOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[W_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[W_VIEWPORT_OFFSET_Y];
	wstring toggleOffsetYProp = (*properties)[W_TOGGLE_OFFSET_Y];
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(toggleOffsetYProp) >> toggleOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
	viewport->setToggleOffsetY(toggleOffsetY);
}