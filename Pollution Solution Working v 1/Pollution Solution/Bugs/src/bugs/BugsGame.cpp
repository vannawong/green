/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BugsGame.cpp

	This is a test game application, a game that demonstrates use of the 
	SideScrollerFramework to make a little scrolling, interactive demo. It
	demonstrates the rendering of images and text as well as responding to
	key presses and button cicks. Students should make their own named game
	applictions using a similar pattern, gradually adding other components,
	like additional gui controls, tiles, sprites, ai, and physics.
 */

#include "bugs_VS\stdafx.h"

// Bugs GAME INCLUDES
#include "bugs\BugsButtonEventHandler.h"
#include "bugs\BugsCollisionListener.h"
#include "bugs\BugsDataLoader.h"
#include "bugs\BugsGame.h"
#include "bugs\BugsKeyEventHandler.h"
#include "bugs\BugsMouseEventHandler.h"
#include "bugs\BugsTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\state\GameState.h"
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

#include "LuaPlusFramework\LuaPlus.h"
using namespace LuaPlus;
#include "stdio.h"
using namespace std;
#include <locale>
#include <codecvt>
#include <string>

#include <cstdio>
#include <iostream>
#include <cmath>
#include <limits> 

/*
	WinMain - This is the application's starting point. In this method we will construct a Game object, 
	then initialize all the platform-dependent technologies, then construct all the custom data for our 
	game, and then initialize the Game with	our custom data. We'll then start the game loop.
*/

struct Nums
{
	int num1;
	int num2;
};

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	// CREATE THE GAME
	Game *bugsGame = new Game();

/*	wstring luaFile = L"luaConstants.lua";
	string s(luaFile.begin(), luaFile.end());
	s.assign(luaFile.begin(), luaFile.end());

	//string luaFile("luaConstants.lua");
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	LuaState* luaPState = LuaState::Create();
	luaPState->DoFile(s.c_str());
		//OutputDebugStringA(luaPState->StackTop().GetString());

		//OutputDebugStringA("HI");

	string hi(luaPState->GetGlobal("W_INIT_FILE").GetString());
	std::wstring W_INIT_FILE = converter.from_bytes(hi);
	
	hi = luaPState->GetGlobal("W_GUI_INIT_FILE").GetString();
	std::wstring W_GUI_INIT_FILE = converter.from_bytes(hi);

		LuaState::Destroy(luaPState);
		*/
	// FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ
	// IN DATA TO SETUP OTHER STUFF
	BugsDataLoader *bugsDataLoader = new BugsDataLoader();
	bugsDataLoader->initWinHandle(hInstance, nCmdShow);
	bugsGame->setDataLoader(bugsDataLoader);
	bugsDataLoader->loadGame(bugsGame, W_INIT_FILE);
	
	// WHAT WE SHOULD BE DOING HERE IS LOADING THE GAME DATA FROM FILES. THIS
	// MEANS THE GUIS THEMSELVES AS WELL AS THE LEVELS. THAT WILL BE LEFT
	// FOR BECHMARK HWS. FOR NOW WE WILL JUST HARD CODE THE LOADING OF THE GUI

	// LOAD THE GUI STUFF, AGAIN, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	bugsDataLoader->loadGUI(bugsGame, W_GUI_INIT_FILE);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	BugsButtonEventHandler *bugsButtonHandler = new BugsButtonEventHandler();
	GameGUI *gui = bugsGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)bugsButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	BugsKeyEventHandler *bugsKeyHandler = new BugsKeyEventHandler();
	bugsGame->getInput()->registerKeyHandler((KeyEventHandler*)bugsKeyHandler);

	// SPECIFY WHO WILL HANDLE MOUSE EVENTS NOT RELATED TO THE GUI
	BugsMouseEventHandler *bugsMouseHandler = new BugsMouseEventHandler();
	bugsGame->getInput()->registerMouseHandler((MouseEventHandler*)bugsMouseHandler);

	// THIS WILL HANDLE PHYSICS COLLISION EVENTS

	BugsCollisionListener *bugsCollisionListener = new BugsCollisionListener();
	bugsGame->getGSM()->getPhysics()->setCollisionListener(bugsCollisionListener);

	// START THE GAME LOOP
	bugsGame->runGameLoop();

	// GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
	delete (WindowsOS*)bugsGame->getOS();
	delete (WindowsInput*)bugsGame->getInput();
	delete (WindowsTimer*)bugsGame->getTimer();
	delete (DirectXGraphics*)bugsGame->getGraphics();
	delete (BugsTextGenerator*)bugsGame->getText()->getTextGenerator();
	delete bugsButtonHandler;
	delete bugsKeyHandler;
	delete bugsGame;

	// AND RETURN
	return 0;
}