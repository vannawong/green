#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\GarbageMon.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"
#include "box2D\box2D.h"

/*
This private constructor is only to be used for cloning bots, note
that this does not setup the velocity for this bot.
*/
GarbageMon::GarbageMon(	unsigned int initMin, 
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
GarbageMon::GarbageMon(	Physics *physics,
					   unsigned int initMin, 
					   unsigned int initMax, 
					   unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);

	// AND START THE BOT OFF IN A RANDOM DIRECTION AND VELOCITY
	// AND WITH RANDOM INTERVAL UNTIL IT THINKS AGAIN
	direction = 0;
	pp.setVelocity(10.0f, 0);
	cyclesRemainingBeforeThinking = 30;
}

/*
clone - this method makes another GarbageMon object, but does
not completely initialize it with similar data to this. Most of the 
object, like velocity and position, are left uninitialized.
*/
Bot* GarbageMon::clone()
{
	GarbageMon *botClone = new GarbageMon(	minCyclesBeforeThinking, 
		maxCyclesBeforeThinking, 
		maxVelocity);
	return botClone;
}

/*
initBot - this initialization method sets up the basic bot
properties, but does not setup its velocity.
*/
void GarbageMon::initBot(	unsigned int initMin,
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
void GarbageMon::pickRandomCyclesInRange()
{
	cyclesRemainingBeforeThinking = maxCyclesBeforeThinking - minCyclesBeforeThinking + 1;
	cyclesRemainingBeforeThinking = rand() % cyclesRemainingBeforeThinking;
	cyclesRemainingBeforeThinking += minCyclesBeforeThinking;
}

/*
pickRandomVelocity - calculates a random velocity vector for this
bot and initializes the appropriate instance variables.
*/
void GarbageMon::pickRandomVelocity(Physics *physics)
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
void GarbageMon::think(Game *game)
{
	SpriteManager *sm = game->getGSM()->getSpriteManager();
	AnimatedSprite *player = game->getGSM()->getSpriteManager()->getPlayer();
	int health;


	/*if (((getBoundingVolume()->getTop() - player->getBoundingVolume()->getBottom()) < 0 ||
	((getBoundingVolume()->getBottom() - player->getBoundingVolume()->getTop()) > 0)) &&
	((getBoundingVolume()->getRight() - player->getBoundingVolume()->getLeft()) > 0 ||
	((getBoundingVolume()->getLeft() - player->getBoundingVolume()->getRight()) < 0))){*/
	if(getBoundingVolume()->overlapsX(player->getBoundingVolume()) || getBoundingVolume()->overlapsY(player->getBoundingVolume())){
		int t1 = getBoundingVolume()->getTop() - player->getBoundingVolume()->getBottom();
		int t2 = getBoundingVolume()->getBottom() - player->getBoundingVolume()->getTop();
		int t3 = getBoundingVolume()->getRight() - player->getBoundingVolume()->getLeft();
		int t4 = getBoundingVolume()->getLeft() - player->getBoundingVolume()->getRight();
		health = (int) _wtoi(sm->getHealthBar()->getCurrentState().c_str());
		if (health <= 10){
			// WHEN PLAYER RUNS OUT OF HEALTH, SKIP TO NEXT DAY
			// PENALTY LIES IN THE PLAYER NOT ACHIEVING THE DAY'S GOALS
			// AND HAVE TO SUFFER INCREASE IN POLLUTION BAR
			sm->getHealthBar()->setCurrentState(to_wstring(100));
		}
		else{
			sm->getHealthBar()->setCurrentState(to_wstring(health - 10));
		}
	}

	if (cyclesRemainingBeforeThinking == 0)
	{
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
		case 0: bah = b2Vec2(pp.getX(), pp.getY()); getBody()->ApplyLinearImpulse (b2Vec2 (0.0f, 2.0f), bah, true); direction++; break;
		case 1: bah = b2Vec2(pp.getX(), pp.getY()); getBody()->ApplyLinearImpulse (b2Vec2 (-2.0f, 0.0f), bah, true);; direction++; break;
		case 2: bah = b2Vec2(pp.getX(), pp.getY()); getBody()->ApplyLinearImpulse (b2Vec2 (0.0f, -2.0f), bah, true); direction++; break;
		case 3: bah = b2Vec2(pp.getX(), pp.getY()); getBody()->ApplyLinearImpulse (b2Vec2 (2.0f, 0.0f), bah, true);; direction = 0; break;
		}
	}
	else
		cyclesRemainingBeforeThinking--;
}