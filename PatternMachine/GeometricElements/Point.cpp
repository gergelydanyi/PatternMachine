#include "Point.h"
#include "math.h"

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

long Point::operator-(Point p)
{
	return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
}

void Point::operator=(Point p)
{
	MoveTo(p.x, p.y);
}