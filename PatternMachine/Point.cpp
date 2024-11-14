#include "Point.h"

Point::Point()
{
	x = 0;
	y = 0;
}

Point::Point(long x, long y)
{
	this->x = x;
	this->y = y;
}

Point::~Point()
{

}

void Point::MoveBy(long x, long y)
{
	this->x += x;
	this->y += y;
}

void Point::MoveTo(long x, long y)
{
	this->x = x;
	this->y = y;
}