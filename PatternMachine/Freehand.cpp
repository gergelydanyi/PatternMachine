#include "Freehand.h"
namespace PatternMachine
{

void Freehand::StartSizing(POINT startPoint)
{
	isSizing = true;
	anchor = startPoint;
	previousPoint = anchor;
	currentPoint = anchor;
	SetRect(&rect, anchor.x, anchor.y, anchor.x, anchor.y);
	InvalidateRect(mainWindow, &rect, FALSE);
}
void Freehand::Sizing(POINT prevPoint, POINT currPoint)
{
	if (isSizing)
	{
		//previousPoint = currentPoint; // this was wrong
		currentPoint = currPoint;
		RECT rectToBeInvalidated;
		rectToBeInvalidated.left = prevPoint.x < currPoint.x ? prevPoint.x : currPoint.x;
		rectToBeInvalidated.top = prevPoint.y < currPoint.y ? prevPoint.y : currPoint.y;
		rectToBeInvalidated.right = prevPoint.x < currPoint.x ? currPoint.x : prevPoint.x;
		rectToBeInvalidated.bottom = prevPoint.y < currPoint.y ? currPoint.y : prevPoint.y;
		SetRect(&rect, rectToBeInvalidated.left, rectToBeInvalidated.top, rectToBeInvalidated.right, rectToBeInvalidated.bottom);
		InvalidateRect(mainWindow, NULL, FALSE);
	}
}
void Freehand::StopSizing()
{
	isSizing = false;
}
}
