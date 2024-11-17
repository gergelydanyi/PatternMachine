#pragma once
class Point
{
public:

	long x{};
	long y{};

	Point();
	Point(long, long);
	~Point();
	void MoveBy(long, long);
	void MoveTo(long, long);
	long operator-(Point);
};

