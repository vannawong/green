#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\NPC.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

/*
	This private constructor is only to be used for cloning bots, note
	that this does not setup the velocity for this bot.
*/
NPC::NPC(		unsigned int initMin, 
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
NPC::NPC(b2Body *b, unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);
	setBody(b);
}

/*
	clone - this method makes another NPC object, but does
	not completely initialize it with similar data to this. Most of the 
	object, like velocity and position, are left uninitialized.
*/
Bot* NPC::clone()
{
	NPC *botClone = new NPC(minCyclesBeforeThinking, maxCyclesBeforeThinking, maxVelocity);
	return botClone;
}

/*
	initBot - this initialization method sets up the basic bot
	properties, but does not setup its velocity.
*/
void NPC::initBot(unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity)
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
		GameStateManager *gsm = game->getGSM();
		pickRandomCyclesInRange();
	}
	else
		cyclesRemainingBeforeThinking--;
}