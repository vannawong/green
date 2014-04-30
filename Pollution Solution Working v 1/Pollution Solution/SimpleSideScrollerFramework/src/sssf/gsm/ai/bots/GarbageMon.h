#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"

class GarbageMon : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking;
	unsigned int minCyclesBeforeThinking;
	unsigned int maxCyclesBeforeThinking;
	unsigned int maxVelocity;

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	GarbageMon(	unsigned int initMin, 
						unsigned int initMax, 
						unsigned int initMaxVelocity);

public:
	GarbageMon(	Physics *physics,
						unsigned int initMin, 
						unsigned int initMax, 
						unsigned int initMaxVelocity);
	~GarbageMon();
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