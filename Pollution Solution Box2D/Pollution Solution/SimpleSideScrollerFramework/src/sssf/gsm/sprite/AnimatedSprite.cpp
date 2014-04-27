/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

AnimatedSprite.cpp

See AnimatedSprite.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"

/*
AnimatedSprite - Default constructor, just sets everything to 0.
*/
AnimatedSprite::AnimatedSprite()  
{
	spriteType = 0;
	frameIndex = 0;
	animationCounter = 0;
	b2Vec2 *v = new b2Vec2();
	v->Set(0,0);
	body->SetLinearVelocity(*v);
	body->SetTransform(*v, 0.0);
	direction = 0;
}

/*
Destructor - Nothing to clean up. We don't want to clean up the
sprite type because it is a shared variable.
*/
AnimatedSprite::~AnimatedSprite() 
{

}

/*
changeFrame - This method allows for the changing of an image in an 
animation sequence for a given animation state.
*/
void AnimatedSprite::changeFrame()
{
	// RESET THE COUNTER
	animationCounter = 0;

	// REMEMBER, WE COUNT BY 2s
	frameIndex += 2;

	// GO BACK TO FIRST INDEX IF NECESSARY
	if (frameIndex == spriteType->getSequenceSize(currentState)){
		frameIndex = 0;
		if (wcscmp(currentState.c_str(), L"DYING") == 0)
			setCurrentState(L"DEAD");
	}
}

/*
getCurrentImageID - This method does the work of retrieving
the image id that corresponds to this sprite's current state
and frame index.
*/
unsigned int AnimatedSprite::getCurrentImageID()
{
	return spriteType->getAnimationFrameID(currentState, frameIndex);
}

/*
setCurrentState - This method sets this sprite to the newState
state and starts its animtion sequence from the beginning.
*/
void AnimatedSprite::setCurrentState(wstring newState) 
{
	string cs(currentState.begin(), currentState.end());
	string ns(newState.begin(), newState.end());
	if (strcmp(cs.c_str(), ns.c_str()) != 0)
	{
		// SET THE ANIMATINO STATE
		currentState = newState;

		// AND RESET OUR COUNTERS
		animationCounter = 0;
		frameIndex = 0;
	}
}

/*
updateSprite - To be called every frame of animation. This
method advances the animation counter appropriately per
the animation speed. It also updates the sprite location
per the current velocity.
*/
void AnimatedSprite::updateSprite()
{
	unsigned int duration = spriteType->getDuration(currentState, frameIndex);
	animationCounter++;

	wstring currState = getCurrentState();
	int temp = currState.find_last_of(L"_");
	wstring state = currState.substr(0, temp);
	switch (direction){
	case 0: setCurrentState(state + L"UP"); break;
	case 1: setCurrentState(state + L"RIGHT"); break;
	case 2: setCurrentState(state + L"DOWN"); break;
	case 3: setCurrentState(state + L"LEFT"); break;
	}

	// WE ONLY CHANGE THE ANIMATION FRAME INDEX WHEN THE
	// ANIMATION COUNTER HAS REACHED THE DURATION
	if (animationCounter >= duration){
		changeFrame();
	}
}