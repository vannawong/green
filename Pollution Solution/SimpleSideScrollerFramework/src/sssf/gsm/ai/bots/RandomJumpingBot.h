#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\game\Game.h"
#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Dynamics\b2World.h"

class RandomJumpingBot : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking;
	unsigned int minCyclesBeforeThinking;
	unsigned int maxCyclesBeforeThinking;
	unsigned int maxVelocity; 
	unsigned int animationRandomizer;

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	RandomJumpingBot(	unsigned int initMin, 
						unsigned int initMax, 
						unsigned int initMaxVelocity);

public:
	RandomJumpingBot(	b2Body *body, unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity);
	~RandomJumpingBot();
	Bot*	clone();
	void initBot(	unsigned int initMin,
					unsigned int initMax,
					unsigned int initMaxVelocity);
	void	pickRandomCyclesInRange();
	void	pickRandomJump(b2Body *b);
	void	think(Game *game);

	// INLINED METHODS
	unsigned int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
	unsigned int getMinCyclesBeforeThinking()		{ return minCyclesBeforeThinking;		}
	unsigned int getMaxCyclesBeforeThinking()		{ return maxCyclesBeforeThinking;		}
};