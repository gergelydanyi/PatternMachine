#include "Route.h"

namespace PatternMachine {


bool Route::isEditing()
{
	return isSizing || isMoving;
}
void Route::StartSizing(POINT startPoint)
{
	isSizing = true;
	isDrawn = false;
	anchor = startPoint;
	SetRect(&rect, anchor.x, anchor.y, anchor.x, anchor.y);
	InvalidateRect(mainWindow, &rect, FALSE);
}
void Route::Sizing(POINT prevPoint, POINT currPoint)
{
	if (isSizing)
	{
		endPoint = currPoint;
		RECT rectToBeInvalidated;
		rectToBeInvalidated.left = prevPoint.x < currPoint.x ? prevPoint.x : currPoint.x;
		rectToBeInvalidated.top = prevPoint.y < currPoint.y ? prevPoint.y : currPoint.y;
		rectToBeInvalidated.right = prevPoint.x < currPoint.x ? currPoint.x : prevPoint.x;
		rectToBeInvalidated.bottom = prevPoint.y < currPoint.y ? currPoint.y : prevPoint.y;
		SetRect(&rect, rectToBeInvalidated.left, rectToBeInvalidated.top, rectToBeInvalidated.right, rectToBeInvalidated.bottom);
		InvalidateRect(mainWindow, NULL, FALSE);
	}
}
void Route::StopSizing()
{
	isSizing = false;
	InvalidateRect(mainWindow, NULL, FALSE);
	CalculateRoute();
}
void Route::CalculateRoute()
{
	std::vector<POINT> temp_vector;
	temp_vector = routePoints;
	temp_vector.clear();
	temp_vector.push_back(anchor);
	routePoints = temp_vector;
	SetSubRoute(anchor, endPoint);
	temp_vector = routePoints;
	temp_vector.push_back(endPoint);
	routePoints = temp_vector;
}
void Route::CalculateRoute2()
{
	std::vector<POINT> temp_vector;
	temp_vector = routePoints;
	temp_vector.clear();
	temp_vector.push_back(anchor);
	POINT previousPoint = anchor;
	POINT currentPoint;
	double d1;
	double d2;
	double dd;
	do
	{
		d1 = Distance(previousPoint, endPoint);
		dd = rand() * d1 / RAND_MAX + 1;
		dd = rand() * dd / RAND_MAX + 1;
		dd = rand() * dd / RAND_MAX + 1;
		do
		{
			long dx = rand() * (dd * 2 + 1) / RAND_MAX - dd;
			long dy = rand() * (dd * 2 + 1) / RAND_MAX - dd;
			currentPoint.x = previousPoint.x + dx;
			currentPoint.y = previousPoint.y + dy;
			d2 = Distance(currentPoint, endPoint);
		} while (d1 * 2 < d2);
		if (d1 > d2)
		{
			previousPoint = currentPoint;
		}
		temp_vector.push_back(currentPoint);
	} while (d2 > dd);
	temp_vector.push_back(endPoint);
	routePoints = temp_vector;
}

// TODO: put this method in an outer class, because it is used in several classes

double Route::Distance(POINT p1, POINT p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
void Route::SetSubRoute(POINT p1, POINT p2)
{
	double d = Distance(p1, p2);
	
	if (d > 2)
	{
		std::vector<POINT> temp_vector;
		long subx;
		long suby;
		POINT subPoint;
		double d1, d2, dLimit, dLimitUp, dLimitDown;
		dLimitDown = 0.667;
		dLimitUp = 0.0;
		dLimit = rand() * (0.7 + dLimitUp - dLimitUp * d / 2500 - dLimitDown) / RAND_MAX + dLimitDown;
		do
		{
			subx = p1.x - d + rand() * (d * 2 + 1) / RAND_MAX;
			suby = p1.y - d + rand() * (d * 2 + 1) / RAND_MAX;
			subPoint = { subx, suby };
			d1 = Distance(p1, subPoint);
			d2 = Distance(subPoint, p2);
		} while (d1 > d * dLimit || d2 > d * dLimit);
		SetSubRoute(p1, subPoint);
		temp_vector = routePoints;
		temp_vector.push_back(subPoint);
		routePoints = temp_vector;
		SetSubRoute(subPoint, p2);
	}
}
}