#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\NPC.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

/*
	This private constructor is only to be used for cloning bots, note
	that this does not setup the velocity for this bot.
*/
NPC::NPC(	unsigned int initMin, 
										unsigned int initMax, 
										unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);
}

/*
	This is the public constructor used by other classes for 
	creating these types of bots.
*/
NPC::NPC(	Physics *physics,
										unsigned int initMin, 
										unsigned int initMax, 
										unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);

	// AND START THE BOT OFF IN A RANDOM DIRECTION AND VELOCITY
	// AND WITH RANDOM INTERVAL UNTIL IT THINKS AGAIN
	pickRandomVelocity(physics);
	pickRandomCyclesInRange();
}

/*
	clone - this method makes another NPC object, but does
	not completely initialize it with similar data to this. Most of the 
	object, like velocity and position, are left uninitialized.
*/
Bot* NPC::clone()
{
	NPC *botClone = new NPC(	minCyclesBeforeThinking, 
															maxCyclesBeforeThinking, 
															maxVelocity);
	return botClone;
}

/*
	initBot - this initialization method sets up the basic bot
	properties, but does not setup its velocity.
*/
void NPC::initBot(	unsigned int initMin,
									unsigned int initMax,
									unsigned int initMaxVelocity)
{
	// IF THE MAX IS SMALLER THAN THE MIN, SWITCH THEM
	if (initMax < initMin)
	{
		unsigned int temp = initMax;
		initMax = initMin;
		initMin = temp;
	}
	// IF THEY ARE THE SAME, ADD 100 CYCLES TO THE MAX
	else if (initMax == initMin)
		initMax += 100;

	// INIT OUR RANGE VARIABLES
	minCyclesBeforeThinking = initMin;
	maxCyclesBeforeThinking = initMax;

	// AND OUR VELOCITY CAPPER
	maxVelocity = initMaxVelocity;
}

/*
	pickRandomCyclesInRange - a randomized method for determining when this bot
	will think again. This method sets that value.
*/
void NPC::pickRandomCyclesInRange()
{
	cyclesRemainingBeforeThinking = maxCyclesBeforeThinking - minCyclesBeforeThinking + 1;
	cyclesRemainingBeforeThinking = rand() % cyclesRemainingBeforeThinking;
	cyclesRemainingBeforeThinking += minCyclesBeforeThinking;
}

/*
	pickRandomVelocity - calculates a random velocity vector for this
	bot and initializes the appropriate instance variables.
*/
void NPC::pickRandomVelocity(Physics *physics)
{
	// FIRST GET A RANDOM float FROM 0.0 TO 1.0
	float randomAngleInRadians = ((float)rand())/((float)RAND_MAX);

	// NOW SCALE IT FROM 0 TO 2 PI
	randomAngleInRadians *= 2.0f;
	randomAngleInRadians *= PI;

	// NOW WE CAN SCALE OUR X AND Y VELOCITIES
	this->pp.setVelocity(maxVelocity * sin(randomAngleInRadians), maxVelocity * cos(randomAngleInRadians));
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void NPC::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN

	if (cyclesRemainingBeforeThinking == 0)
	{
		/*GameStateManager *gsm = game->getGSM();
		pickRandomVelocity(gsm->getPhysics());
		pickRandomCyclesInRange();*/
		cyclesRemainingBeforeThinking = 30;
		//pickRandomCyclesInRange();

		/*switch(direction){
		case 0: pp.setVelocity(0.0, 10.0f); direction++; break;
		case 1: pp.setVelocity(-10.0, 0.0f); direction++; break;
		case 2: pp.setVelocity(0.0, -10.0f); direction++; break;
		case 3: pp.setVelocity(10.0, 0.0f); direction = 0; break;

		}*/
		b2Vec2 bah;
		switch(direction){
		case 0: bah = b2Vec2(pp.getX(), pp.getY()); //getBody()->ApplyLinearImpulse (b2Vec2 (0.0f, 2.0f), bah, true); 
			direction++; 
			getBody()->SetLinearVelocity(b2Vec2(0,0));
			getBody()->SetAngularVelocity(0);
			break;
		case 1: bah = b2Vec2(pp.getX(), pp.getY()); //getBody()->ApplyLinearImpulse (b2Vec2 (-2.0f, 0.0f), bah, true);; 
			direction++; 
			getBody()->SetLinearVelocity(b2Vec2(0,0));
			getBody()->SetAngularVelocity(0);
			break;
		case 2: bah = b2Vec2(pp.getX(), pp.getY()); //getBody()->ApplyLinearImpulse (b2Vec2 (0.0f, -2.0f), bah, true); 
			direction++; 
			getBody()->SetLinearVelocity(b2Vec2(0,0));
			getBody()->SetAngularVelocity(0);
			break;
		case 3: bah = b2Vec2(pp.getX(), pp.getY()); //getBody()->ApplyLinearImpulse (b2Vec2 (2.0f, 0.0f), bah, true);; 
			direction = 0; 
			getBody()->SetLinearVelocity(b2Vec2(0,0));
			getBody()->SetAngularVelocity(0);
			break;
		}
	}
	else
		cyclesRemainingBeforeThinking--;
}