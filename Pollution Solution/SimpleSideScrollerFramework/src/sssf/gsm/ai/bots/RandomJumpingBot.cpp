#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\RandomJumpingBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Dynamics\b2World.h"
#include "Box2D\Box2D.h"

/*
This private constructor is only to be used for cloning bots, note
that this does not setup the velocity for this bot.
*/
RandomJumpingBot::RandomJumpingBot(	unsigned int initMin, 
								   unsigned int initMax, 
								   unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);

	b2Vec2 v;
	v.Set(0,0);
	body->SetTransform(v,0);
}

/*
This is the public constructor used by other classes for 
creating these types of bots.
*/
RandomJumpingBot::RandomJumpingBot(	b2Body *b,
								   unsigned int initMin, 
								   unsigned int initMax, 
								   unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);
	// AND START THE BOT OFF IN A RANDOM DIRECTION AND VELOCITY
	// AND WITH RANDOM INTERVAL UNTIL IT THINKS AGAIN
	//pickRandomJump(physics);
	pickRandomCyclesInRange();


}

/*
clone - this method makes another RandomJumpingBot object, but does
not completely initialize it with similar data to this. Most of the 
object, like velocity and position, are left uninitialized.
*/
Bot* RandomJumpingBot::clone()
{
	RandomJumpingBot *botClone = new RandomJumpingBot(	minCyclesBeforeThinking, 
		maxCyclesBeforeThinking,
		maxVelocity);
	return botClone;
}

/*
initBot - this initialization method sets up the basic bot
properties, but does not setup its velocity.
*/
void RandomJumpingBot::initBot(unsigned int initMin,
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

	// AND WE'LL USE THIS TO ENSURE OUR BOTS ALL LOOK A LITTLE DIFFERENT
	animationRandomizer = (rand() % 45) + 5;
}

/*
pickRandomCyclesInRange - a randomized method for determining when this bot
will think again. This method sets that value.
*/
void RandomJumpingBot::pickRandomCyclesInRange()
{
	cyclesRemainingBeforeThinking = maxCyclesBeforeThinking - minCyclesBeforeThinking + 1;
	cyclesRemainingBeforeThinking = rand() % cyclesRemainingBeforeThinking;
	cyclesRemainingBeforeThinking += minCyclesBeforeThinking;
}

/*
pickRandomVelocity - calculates a random velocity vector for this
bot and initializes the appropriate instance variables.
*/
void RandomJumpingBot::pickRandomJump(b2Body *b)
{
	// FIRST GET A RANDOM float FROM 0.0 TO 1.0
	float randMax = RAND_MAX;
	float randReal = (float)rand();
	float randomReal = randReal/randMax;

	// NOW SCALE IT FROM 0 TO PI PI
	float randomAngleInRadians = (randomReal * PI/6.0f) + (PI*2.5f/6.0f);

	// NOW WE CAN SCALE OUR X AND Y VELOCITIES
	float jumpVelocity = (float)maxVelocity;
	float jumpVelocityX = jumpVelocity * cos(randomAngleInRadians);
	float jumpVelocityY = -jumpVelocity * sin(randomAngleInRadians);
	b2Vec2 *vel = new b2Vec2(jumpVelocityX, jumpVelocityY);
	body->SetLinearVelocity(*vel);
}

/*
think - called once per frame, this is where the bot performs its
decision-making. Note that we might not actually do any thinking each
frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void RandomJumpingBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN

	if (cyclesRemainingBeforeThinking == 0)
	{

	}
	else
		cyclesRemainingBeforeThinking--;

	animationRandomizer--;
	if (animationRandomizer == 0)
	{
		animationCounter++;
		animationRandomizer = (rand() % 45) + 5;
	}
}