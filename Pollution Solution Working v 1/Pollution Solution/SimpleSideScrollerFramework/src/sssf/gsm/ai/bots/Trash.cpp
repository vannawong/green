#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\Trash.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\physics.h"
#include "box2D\box2D.h"

Trash::Trash(unsigned int initMin, 
			 unsigned int initMax, 
			 unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	setCurrentState(L"IDLE");

	// AND START THE BOT OFF IN A RANDOM DIRECTION AND VELOCITY
	// AND WITH RANDOM INTERVAL UNTIL IT THINKS AGAIN
	pp.setVelocity(0,0);	
}

Trash::Trash( Physics *physics,
			 unsigned int initMin, 
			 unsigned int initMax, 
			 unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	setCurrentState(L"IDLE");

	// AND START THE BOT OFF IN A RANDOM DIRECTION AND VELOCITY
	// AND WITH RANDOM INTERVAL UNTIL IT THINKS AGAIN
	pp.setVelocity(0,0);	
}

void Trash::think(Game *game){

}

Bot* Trash::clone()
{
	Trash *botClone = new Trash(0,0,0);
	return botClone;
}

void Trash::collisionResponse(Game *game){
	setCurrentState(L"DEAD");
}

Trash::~Trash(void)
{
}
