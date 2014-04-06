#include "ChaosGamePluginPCH.h"
#include "mArea.h"

mArea::mArea(void)
{
}


mArea::~mArea(void)
{
}

hkvVec3 mArea::Center()
{
	return (startPoint + endPoint) / 2;
}

hkvVec3 mArea::TopLeft()
{
	float X = (startPoint.x >= endPoint.x) ? endPoint.x : startPoint.x;
	float Y = (startPoint.y >= endPoint.y) ? startPoint.y : endPoint.y;

	return hkvVec3(X,Y,0);
}

hkvVec3 mArea::BottomRight()
{
	float X = (startPoint.x >= endPoint.x) ? startPoint.x : endPoint.x;
	float Y = (startPoint.y >= endPoint.y) ? endPoint.y : startPoint.y;

	return hkvVec3(X,Y,0);
}

int mArea::Width()
{
	return (int)(endPoint.x - startPoint.x);
}

int mArea::Height()
{
	return (int)(endPoint.y - startPoint.y);
}

void mArea::OffSet(int x, int y)
{
	this->startPoint.x += x;
	this->startPoint.y += y;
	this->endPoint.x += x;
	this->endPoint.y += y;
}

bool mArea::Intersects(std::shared_ptr<mArea> b, int LengthApart)
{
	hkvVec3 minA = this->TopLeft();
	hkvVec3 maxA = this->BottomRight();
	hkvVec3 minB = b->TopLeft();
	hkvVec3 maxB = b->BottomRight();

	bool A = minA.x - LengthApart < maxB.x;
	bool B = maxA.x + LengthApart > minB.x;
	bool C = minA.y - LengthApart < maxB.y;
	bool D = maxA.y + LengthApart > minB.y;

	return A && B && C && D;
}

