#include "Line.h"

namespace PatternMachine
{

	Line::Line(Point& p1, Point& p2)
	{
		this->p1 = p1;
		this->p2 = p2;
		points[0] = &p1;
		points[1] = &p2;
	}

	Line::~Line()
	{
		delete& p1;
		delete& p2;
	}

	void Line::StartSizing(POINT p)
	{
		isSizing = true;
		isDrawn = false;
	}

	void Line::Sizing(POINT p1, POINT p2)
	{
		if (isSizing)
		{
			this->p2.x = p1.x;
			this->p2.y = p1.y;
			InvalidateRect(mainWindow, NULL, FALSE);
		}
	}

	void Line::StopSizing()
	{
		isSizing = false;
		InvalidateRect(mainWindow, NULL, FALSE);
	}

}