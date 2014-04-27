/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

BugsKeyEventHandler.cpp

See BugsKeyEventHandler.h for a class description.
*/

#include "bugs_VS\stdafx.h"
#include "bugs\BugsGame.h"
#include "bugs\BugsKeyEventHandler.h"
#include "sssf\game\Game.h"
#include "sssf\game\WStringTable.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

/*
handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
key states since the last frame, which can allow us to respond to key presses, including when keys
are held down for multiple frames.
*/
void BugsKeyEventHandler::handleKeyEvents(Game *game)
{
	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
	PhysicalProperties *pp = player->getPhysicalProperties();
	Viewport *viewport = game->getGUI()->getViewport();

	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		if (input->isKeyDownForFirstTime(D_KEY))
		{
			viewport->toggleDebugView();
			game->getGraphics()->toggleDebugTextShouldBeRendered();
		}
		if (input->isKeyDownForFirstTime(L_KEY))
		{
			game->getGraphics()->togglePathfindingGridShouldBeRendered();
		}
		if (input->isKeyDownForFirstTime(F_KEY))
		{
			game->getGraphics()->togglePathfindingPathShouldBeRendered();
		}
		if (input->isKeyDownForFirstTime(Q_KEY))
		{
			game->getGraphics()->toggleRenderQuests();
		}
		if (input->isKeyDownForFirstTime(R_KEY))
		{
			if (wcscmp(player->getCurrentState().c_str(), L"RUNNING") == 0)
				player->setCurrentState(L"WALKING");
			else if ((wcscmp(player->getCurrentState().c_str(), L"IDLE") != 0) && (wcscmp(player->getCurrentState().c_str(), L"DANCING") != 0))
				player->setCurrentState(L"RUNNING");
		}
		if (input->isKeyDownForFirstTime(Y_KEY)){
			player->setCurrentState(DYING);
		}
		if (input->isKeyDownForFirstTime(I_KEY)){
			player->setCurrentState(IDLE);
		}
		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;
		float vX = pp->getVelocityX();
		float vY = pp->getVelocityY();
		if (input->isKeyDown(UP_KEY))
		{
			if (pp->getY() < (viewport->getViewportY() + 0.5f * viewport->getViewportHeight()))
				viewportVy -= MAX_PLAYER_VELOCITY;
			vY = -1 * MAX_PLAYER_VELOCITY;
			vX = 0;
			viewportMoved = true;
		}
		else if (input->isKeyDown(DOWN_KEY))
		{
			if (pp->getY() > (viewport->getViewportY() + 0.5f * viewport->getViewportHeight()))
				viewportVy += MAX_PLAYER_VELOCITY;
			vY = MAX_PLAYER_VELOCITY;
			vX = 0;
			viewportMoved = true;
		}
		else if (input->isKeyDown(LEFT_KEY))
		{
			if (pp->getX() < (viewport->getViewportX() + 0.5f * viewport->getViewportWidth()))
				viewportVx -= MAX_PLAYER_VELOCITY;
			vX = -1 * MAX_PLAYER_VELOCITY;
			vY = 0;
			viewportMoved = true;
		}
		else if (input->isKeyDown(RIGHT_KEY))
		{
			if (pp->getX() > (viewport->getViewportX() + 0.5f * viewport->getViewportWidth()))
			viewportVx += MAX_PLAYER_VELOCITY;
			vX = MAX_PLAYER_VELOCITY;
			vY = 0;
			viewportMoved = true;
		}
		else {
			vX = 0.0f;
			vY = 0.0f;
		}
		if (viewportMoved)
			viewport->moveViewport((int)floor(viewportVx+0.5f), (int)floor(viewportVy+0.5f), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		pp->setVelocity(vX, vY);
	}

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		unsigned int id = cursor->getActiveCursorID();
		id++;
		if (id == cursor->getNumCursorIDs())
			id = 0;		
		cursor->setActiveCursorID(id);
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();
	int fps = timer->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_HOME) && (fps < MAX_FPS))
		timer->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_END) && (fps > MIN_FPS))
		timer->setTargetFPS(fps - FPS_INC);
}