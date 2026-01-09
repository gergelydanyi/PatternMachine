#pragma once
class Point
{
public:

	long x{};
	long y{};

	Point() : Point(0, 0) {};
	Point(long, long);
	~Point();
	void MoveBy(long, long);
	void MoveTo(long, long);
	long operator-(Point);
};

