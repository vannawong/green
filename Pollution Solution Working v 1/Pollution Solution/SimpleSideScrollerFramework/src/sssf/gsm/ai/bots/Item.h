#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"

class Item : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking;
	unsigned int minCyclesBeforeThinking;
	unsigned int maxCyclesBeforeThinking;
	unsigned int maxVelocity;
	unsigned int direction;

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	Item(	unsigned int initMin, 
						unsigned int initMax, 
						unsigned int initMaxVelocity);

public:
	Item(	Physics *physics,
						unsigned int initMin, 
						unsigned int initMax, 
						unsigned int initMaxVelocity);
	~Item();
	Bot*	clone();
	void initBot(	unsigned int initMin,
					unsigned int initMax,
					unsigned int initMaxVelocity);
	void	pickRandomCyclesInRange();
	void	pickRandomVelocity(Physics *physics);
	void	think(Game *game);

	// INLINED METHODS
	unsigned int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
	unsigned int getMinCyclesBeforeThinking()		{ return minCyclesBeforeThinking;		}
	unsigned int getMaxCyclesBeforeThinking()		{ return maxCyclesBeforeThinking;		}
};