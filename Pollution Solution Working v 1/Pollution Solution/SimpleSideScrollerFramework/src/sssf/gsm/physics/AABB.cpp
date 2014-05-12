#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\AABB.h"

bool  AABB::overlaps(AABB *testAABB)
{
	return overlapsX(testAABB) && overlapsY(testAABB);
}

bool  AABB::overlapsX(AABB *testAABB)
{
	float diffX = centerX - testAABB->centerX;
	if (abs(diffX) > ((width/4) + (testAABB->width/4)))
		return false;
	else
		return true;
	return false;
}

bool  AABB::overlapsY(AABB *testAABB)
{
	float diffY = centerY - testAABB->centerY;
	if (abs(diffY) > ((height/2) + (testAABB->height/2)))
		return false;
	else
		return true;
	return false;
}