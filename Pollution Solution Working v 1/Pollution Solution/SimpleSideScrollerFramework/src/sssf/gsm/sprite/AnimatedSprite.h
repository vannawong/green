/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.h

	This class represents a sprite that can can
	be used to animate a game character or object.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\pathfinding\GridPathfinder.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "Box2D\Dynamics\b2Body.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gui\Viewport.h"

class AnimatedSprite : public CollidableObject
{
protected:
	// SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
	// OF AN AnimatedSpriteType OBJECT AS STORED IN THE SpriteManager
	AnimatedSpriteType *spriteType;

	// TRANSPARENCY/OPACITY
	int alpha;

	// THE "current" STATE DICTATES WHICH ANIMATION SEQUENCE 
	// IS CURRENTLY IN USE, BUT IT MAP ALSO BE USED TO HELP
	// WITH OTHER GAME ACTIVITIES, LIKE PHYSICS
	wstring currentState;

	// THE INDEX OF THE CURRENT FRAME IN THE ANIMATION SEQUENCE
	// NOTE THAT WE WILL COUNT BY 2s FOR THIS SINCE THE VECTOR
	// THAT STORES THIS DATA HAS (ID,DURATION) TUPLES
	unsigned int frameIndex;

	// USED TO ITERATE THROUGH THE CURRENT ANIMATION SEQUENCE
	unsigned int animationCounter;

	// CURRENT PATH TO FOLLOW
	list<PathNode> currentPathToFollow;
	list<PathNode>::iterator currentPathNode;

	// DIRECTION: 0 for up, 1 for right, 2 for down, and 3 for left
	unsigned int direction;

public:
	// INLINED ACCESSOR METHODS
	int					getAlpha()			{ return alpha;				}
	list<PathNode>*		getCurrentPathToFollow() { return &currentPathToFollow; }
	list<PathNode>::iterator getCurrentPathNode() { return currentPathNode; }
	wstring				getCurrentState()	{ return currentState;		}
	unsigned int		getFrameIndex()		{ return frameIndex;		}
	AnimatedSpriteType*	getSpriteType()		{ return spriteType;		}
	int					getAnimationCounter() { return animationCounter;}
	bool hasReachedDestination()
	{	return currentPathNode == currentPathToFollow.end(); }

	// INLINED MUTATOR METHODS
	void setAlpha(int initAlpha)
	{	alpha = initAlpha;						}
	void setSpriteType(AnimatedSpriteType *initSpriteType)
	{	spriteType = initSpriteType;			}
	void advanceCurrentPathNode()
	{	currentPathNode++; }
	void resetCurrentPathNode()
	{	currentPathNode = currentPathToFollow.begin(); }
	void clearPath()
	{	currentPathToFollow.clear();
		currentPathNode = currentPathToFollow.end(); 
	}

	// METHODS DEFINED IN AnimatedSprite.cpp
	AnimatedSprite();
	~AnimatedSprite();
	void changeFrame();
	unsigned int getCurrentImageID();
	void setCurrentState(wstring newState);
	virtual void updateSprite();
	void affixTightAABBBoundingVolume();
	void correctToTightBoundingVolume();
	virtual float getRotationInRadians() { return 0.0f; }
};