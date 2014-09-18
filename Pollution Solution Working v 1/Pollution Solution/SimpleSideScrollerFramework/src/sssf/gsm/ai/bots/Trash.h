#pragma once
#include "sssf_vs\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\physics.h"
#include "sssf\game\Game.h"

class Trash : public Bot
{
private: 
	Trash(unsigned int initMin, 
		unsigned int initMax, 
		unsigned int initMaxVelocity);
public:
	Trash(Physics *physics,
		unsigned int initMin, 
		unsigned int initMax, 
		unsigned int initMaxVelocity);
	void initBot(	unsigned int initMin,
		unsigned int initMax,
		unsigned int initMaxVelocity);
	void	think(Game *game);
	void	collisionResponse(Game *game);
	Bot*	clone();
	virtual ~Trash(void);
};

