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
#include "sssf\gsm\ai\bots\RandomJumpingBot.h"
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

// BOX2D LOADING
#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Dynamics\b2World.h"
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
	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_1_NAME);

	// SPECIFY WHO WILL DO THE PATHFINDING
	GameStateManager *gsm = game->getGSM();
	World *world = gsm->getWorld();
	SpriteManager *spriteManager = gsm->getSpriteManager();
	
	// LOAD THE LEVEL'S SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LET'S MAKE A PLAYER SPRITE
	TopDownSprite *player = spriteManager->getPlayer();
	player->setRotationInRadians(0.0f);

	// PLAYER SPRITE TYPE ID IS 0
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(0);
	player->setSpriteType(playerSpriteType);
	player->setAlpha(255);
	player->setCurrentState(IDLE_DOWN);
	
	b2World *bworld = gsm->getBWorld();
	
	b2BodyDef pDef;
	pDef.position.Set(PLAYER_INIT_X, PLAYER_INIT_Y);
	pDef.type = b2_dynamicBody;
	player->setBody(bworld->CreateBody(&pDef));
	b2PolygonShape box;

	b2FixtureDef fd;
	fd.shape = &box;
	fd.density = 1.0f;
	fd.restitution = 0.0f;
	fd.friction = 0.0f;
	box.SetAsBox(player->getSpriteType()->getTextureWidth() / 2.0, player->getSpriteType()->getTextureHeight() / 2.0);

	player->getBody()->CreateFixture(&fd);

	b2Vec2 v;
	v.Set(0,0);
	player->getBody()->SetLinearVelocity(v);
	player->getBody()->SetLinearDamping(0.0f);
	player->getBody()->SetAwake(true);

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
	makeRandomJumpingBot(game, botSpriteType, 400, 100);
	makeRandomJumpingBot(game, botSpriteType, 200, 400);
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
	/*
	b2World *bworld = game->getGSM()->getBWorld();
	b2BodyDef pDef;
	pDef.position.Set(initX, initY);
	pDef.type = b2_dynamicBody;
	b2Body* b = bworld->CreateBody(&pDef);
	*/
	NPC *npc = new NPC(NULL, 0, 0, 0);
	//npc->setBody(bworld->CreateBody(&pDef));
	npc->setCurrentState(IDLE_DOWN);
	npc->setSpriteType(npcSpriteType);
	npc->setAlpha(255);
	npc->setCurrentState(IDLE_DOWN);
	/*
	npc->getBody()->SetAwake(true);

	b2Vec2 v;
	v.Set(0,0);
	npc->getBody()->SetLinearVelocity(v);
	npc->getBody()->SetLinearDamping(0.0f);
	b2PolygonShape box;
	box.SetAsBox(npc->getSpriteType()->getTextureWidth() / 2.0, npc->getSpriteType()->getTextureHeight() / 2.0);

	b2FixtureDef fd;
	fd.shape = &box;
	fd.density = 1.0f;
	fd.restitution = 0.0f;
	fd.friction = 0.0f;
	
	npc->getBody()->CreateFixture(&fd);
	*/
	spriteManager->addBot(npc);
}

void BugsDataLoader::makeRandomJumpingBot(Game *game, AnimatedSpriteType *randomJumpingBotType, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	RandomJumpingBot *bot = new RandomJumpingBot(NULL, initX, initY, JUMPING_BOT_MAX_VELOCITY);
	
	/*
	b2World *bworld = game->getGSM()->getBWorld();
	b2BodyDef pDef;
	pDef.position.Set(initX, initY);
	pDef.type = b2_dynamicBody;
	b2Body *body = bworld->CreateBody(&pDef);
	*/
	bot->setSpriteType(randomJumpingBotType);
	bot->setAlpha(255);
	bot->setCurrentState(IDLE_DOWN);
	/*
	bot->setBody(body);
	bot->getBody()->SetAwake(true);

	b2Vec2 v;
	v.Set(0,0);
	bot->getBody()->SetLinearVelocity(v);
	bot->getBody()->SetLinearDamping(0.0f);
	b2PolygonShape box;
	box.SetAsBox(bot->getSpriteType()->getTextureWidth() / 2.0, bot->getSpriteType()->getTextureHeight() / 2.0);

	b2FixtureDef fd;
	fd.shape = &box;
	fd.density = 1.0f;
	fd.restitution = 0.0f;
	fd.friction = 0.0f;
	
	bot->getBody()->CreateFixture(&fd);
	*/
	spriteManager->addBot(bot);
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