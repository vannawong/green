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

//Box2D
#include "Box2D\Box2D.h"

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
	b2World* bworld = game->getbworld();
	b2Vec2* v;

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
		if (input->isKeyDownForFirstTime(Q_KEY))
		{
			game->getGraphics()->toggleRenderQuests();
		}
		
		//item stuff
		if (input->isKeyDownForFirstTime('A'))
		{
			//if (player->getPhysicalProperties()->
			//	game->getGSM()->getSpriteManager()->removeBot();
		}

		if (input->isKeyDownForFirstTime(R_KEY))
		{
			game->blackOut();
		/*	if (wcscmp(player->getCurrentState().c_str(), L"RUNNING") == 0)
				player->setCurrentState(L"WALKING");
			else if ((wcscmp(player->getCurrentState().c_str(), L"IDLE") != 0) && (wcscmp(player->getCurrentState().c_str(), L"DANCING") != 0))
				player->setCurrentState(L"RUNNING");*/
		}
		if (input->isKeyDownForFirstTime(Y_KEY)){
			player->setCurrentState(DYING);
		}
		if (input->isKeyDownForFirstTime(I_KEY)){
			player->setCurrentState(IDLE);
		}
		if (input->isKeyDownForFirstTime(ONE_KEY)){
			if(wcscmp((game->getCurrentLevelFileName().c_str()), (LEVEL_1.c_str())) == 0){
			}
			else{
			game->quitGame();
			game->setCurrentLevelFileName(LEVEL_1);
			game->startGame();
			}
		}
		if (input->isKeyDownForFirstTime(TWO_KEY)){
			if(wcscmp((game->getCurrentLevelFileName().c_str()), (LEVEL_2.c_str())) == 0){
			}
			else{
			game->quitGame();
			game->setCurrentLevelFileName(LEVEL_2);
			game->startGame();
			}
		}
		if (input->isKeyDownForFirstTime(THREE_KEY)){
			if(wcscmp((game->getCurrentLevelFileName().c_str()), (LEVEL_3.c_str())) == 0){
			}
			else{
			game->quitGame();
			game->setCurrentLevelFileName(LEVEL_3);
			game->startGame();
			}
		}
		if (input->isKeyDownForFirstTime(U_KEY)){ 
			SpriteManager *sm = game->getGSM()->getSpriteManager(); 
			int counter = 0; 
			list<Bot*>::iterator botIterator = sm->getBotsIterator(); 
			while (botIterator != sm->getEndOfBotsIterator()) { 
				Bot *bot = (*botIterator); 
				if (bot->getSpriteType()->getSpriteTypeID() != 1){ 
					if(bot->getPhysicalProperties()->getX() == 9999.9f || bot->getPhysicalProperties()->getY() == 9999.9f){}
					counter++; 
				} 
				botIterator++; 
			} 
			AnimatedSprite *pol = sm->getPolBar(); 
			int i = (counter / 3 * 10);
			pol->setCurrentState(to_wstring(counter / 3 * 10)); 
			GameDataLoader* dl = game->getDataLoader();
			if(i >= 30){
				AnimatedSpriteType *botSpriteType = game->getGSM()->getSpriteManager()->getSpriteType(2);
				//dl->makeGarbageMon(game, botSpriteType, 7.0f + (i*4), 13.0f);
			}
		}

		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;
		float vX = pp->getVelocityX();
		float vY = pp->getVelocityY();

		if (input->isKeyDown(UP_KEY))
		{
			player->setCurrentState(L"WALKINGBACK");
			if (pp->getY() < (viewport->getViewportY() + 0.5f * viewport->getViewportHeight()))
				viewportVy -= MAX_PLAYER_VELOCITY;
			vY = -1 * MAX_PLAYER_VELOCITY;
			vX = 0;
			viewportMoved = true;
			game->getGSM()->getCurrentGameState();
		}
		else if (input->isKeyDown(DOWN_KEY))
		{
			player->setCurrentState(L"WALKINGFORWARD");
			if (pp->getY() > (viewport->getViewportY() + 0.5f * viewport->getViewportHeight()))
				viewportVy += MAX_PLAYER_VELOCITY;
			vY = MAX_PLAYER_VELOCITY;
			vX = 0;
			viewportMoved = true;
		}
		else if (input->isKeyDown(LEFT_KEY))
		{
			player->setCurrentState(L"WALKINGLEFT");
			if (pp->getX() < (viewport->getViewportX() + 0.5f * viewport->getViewportWidth()))
				viewportVx -= MAX_PLAYER_VELOCITY;
			vX = -1 * MAX_PLAYER_VELOCITY;
			vY = 0;
			viewportMoved = true;
		}
		else if (input->isKeyDown(RIGHT_KEY))
		{
			player->setCurrentState(L"WALKINGRIGHT");
			if (pp->getX() > (viewport->getViewportX() + 0.5f * viewport->getViewportWidth()))
			viewportVx += MAX_PLAYER_VELOCITY;
			vX = MAX_PLAYER_VELOCITY;
			vY = 0;
			viewportMoved = true;
			//b2Body* b = player->getBody();
			//b->SetLinearVelocity (b2Vec2 (7.5f, 0.0f));
		}
		else {
			if((wcscmp(player->getCurrentState().c_str(), L"WALKINGRIGHT") == 0) && !input->isKeyDown(RIGHT_KEY))
				player->setCurrentState(L"WALKINGRIGHTIDLE");
		else if((wcscmp(player->getCurrentState().c_str(), L"WALKINGLEFT") == 0) && !input->isKeyDown(LEFT_KEY))
			player->setCurrentState(L"WALKINGLEFTIDLE");
		else if((wcscmp(player->getCurrentState().c_str(), L"WALKINGBACK") == 0) && !input->isKeyDown(UP_KEY))
			player->setCurrentState(L"WALKINGBACKIDLE");
		else if((wcscmp(player->getCurrentState().c_str(), L"WALKINGFORWARD") == 0) && !input->isKeyDown(DOWN_KEY))
			player->setCurrentState(IDLE);
			vX = 0.0f;
			vY = 0.0f;
		}
		if (viewportMoved)
			viewport->moveViewport((int)floor(viewportVx+0.5f), (int)floor(viewportVy+0.5f), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());

		//b2Vec2 moveVec(vX * 0.2f, vY * 0.2f); 
		//player->getBody()->ApplyLinearImpulse (moveVec, player->getBody()->GetPosition(), true);

		//pp->setVelocity(vX, vY);
		b2Vec2 moveVec(vX * 1.0f, vY * 1.0f); 
		b2Vec2 a = player->getBody()->GetPosition();
		player->getBody()->ApplyForce (moveVec, player->getBody()->GetPosition(), true);
		player->getBody()->SetLinearVelocity(b2Vec2(0,0));
		player->getBody()->SetAngularVelocity(0);

		//if(vX < 0)
			//player->

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