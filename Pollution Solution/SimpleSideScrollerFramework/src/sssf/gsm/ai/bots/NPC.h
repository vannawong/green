#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"

class NPC : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking;
	unsigned int minCyclesBeforeThinking;
	unsigned int maxCyclesBeforeThinking;
	unsigned int maxVelocity;

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	NPC(				unsigned int initMin, 
		unsigned int initMax, 
		unsigned int initMaxVelocity);

public:
	NPC(b2Body *body, unsigned int initMin,	unsigned int initMax, unsigned int initMaxVelocity);
	~NPC();
	Bot*	clone();
	void initBot(unsigned int initMin,
		unsigned int initMax,
		unsigned int initMaxVelocity);
	void	pickRandomCyclesInRange();
	void	think(Game *game);

	// INLINED METHODS
	unsigned int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
	unsigned int getMinCyclesBeforeThinking()		{ return minCyclesBeforeThinking;		}
	unsigned int getMaxCyclesBeforeThinking()		{ return maxCyclesBeforeThinking;		}
};