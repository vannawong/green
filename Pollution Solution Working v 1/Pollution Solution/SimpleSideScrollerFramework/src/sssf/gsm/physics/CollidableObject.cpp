#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\Collision.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "box2D\box2D.h"

/*
	This method updates the position and dimensions of this
	dynamic object's swept shape based on how much time
	is left during this frame.
*/
void CollidableObject::updateSweptShape(float percentageOfFrameTimeRemaining)
{
	// NOTE THAT IF WE ARE MID-FRAME WE DON'T WISH TO
	// APPLY THE FULL VELOCITY
	float scaledVx = pp.getVelocityX() * percentageOfFrameTimeRemaining;
	float scaledVy = pp.getVelocityY() * percentageOfFrameTimeRemaining;

	// NOTE THE DIMENSIONS OF THE SWEPT SHAPE ARE CALCULATED USING
	// THE ABOLUTE VALUE OF THE VELOCITY
	sweptShape.setWidth(boundingVolume.getWidth() + abs(pp.getX()));//scaledVx));
	sweptShape.setHeight(boundingVolume.getHeight() + abs(pp.getY()));//scaledVy));

	// UPDATE ITS CENTER?
	float centerX = boundingVolume.getCenterX();
	centerX = centerX + (scaledVx/2.0f);
	sweptShape.setCenterX(centerX);
	float centerY = boundingVolume.getCenterY();
	centerY = centerY + (scaledVy/2.0f);
	sweptShape.setCenterY(centerY);
}

//box2d
void CollidableObject::setBody(b2Body* b) {
	body = b;
}

b2Body* CollidableObject::getBody () {
	return body;
}