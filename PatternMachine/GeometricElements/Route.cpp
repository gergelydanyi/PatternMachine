#include "Route.h"
#include "Canvas.h"

namespace PatternMachine {

	Route::Route(Canvas* pCanvas) : Route(new Layer(pCanvas)) {}

	Route::Route(HWND hwnd) : Route(new Layer(hwnd)) {}

	Route::Route(Layer* pLayer)
	{
		type = RouteShapeType;
		SetLayer(pLayer);
		mainWindow = layer->pCanvas->hWindow;
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
			SetRect(&rect, anchor.x, anchor.y, prevPoint.x, prevPoint.y);
			endPoint = prevPoint;
			RECT rectToBeInvalidated;
			rectToBeInvalidated.left = prevPoint.x < currPoint.x ? prevPoint.x : currPoint.x;
			rectToBeInvalidated.top = prevPoint.y < currPoint.y ? prevPoint.y : currPoint.y;
			rectToBeInvalidated.right = prevPoint.x < currPoint.x ? currPoint.x : prevPoint.x;
			rectToBeInvalidated.bottom = prevPoint.y < currPoint.y ? currPoint.y : prevPoint.y;
			//SetRect(&rect, rectToBeInvalidated.left, rectToBeInvalidated.top, rectToBeInvalidated.right, rectToBeInvalidated.bottom);
			InvalidateRect(mainWindow, NULL, FALSE);
		}
	}
	void Route::StopSizing()
	{
		isSizing = false;
		CalculateRoute();
		SetHitRegion();
		InvalidateRect(mainWindow, NULL, FALSE);
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
	void Route::SetHitRegion()
	{
		long hitAreaWidth = 5;
		DeleteObject(hitRegion);
		//hitRegion = CreateRoundRectRgn(rect.left - hitAreaWidth, rect.top - hitAreaWidth, rect.right + hitAreaWidth + 1, rect.bottom + hitAreaWidth + 1, hitAreaWidth, hitAreaWidth);
		hitRegion = CreateEllipticRgn(anchor.x - hitAreaWidth, anchor.y - hitAreaWidth, anchor.x + hitAreaWidth, anchor.y + hitAreaWidth);
		for (int i = 1; i < routePoints.size(); ++i)
		{
			Point* p1 = new Point(routePoints[i].x, routePoints[i].y);
			Point* p2 = new Point(routePoints[i - 1].x, routePoints[i - 1].y);
			HRGN segmentHitRegion = SetSegmentHitRegion(p1, p2);
			CombineRgn(hitRegion, hitRegion, segmentHitRegion, RGN_OR);
			DeleteObject(segmentHitRegion);
			delete p1;
			delete p2;
		}
	}
	void Route::MoveBy(POINT p1)
	{
		for (int i = 0; i < routePoints.size(); ++i)
		{
			routePoints[i].x += p1.x;
			routePoints[i].y += p1.y;
		}
		rect.left += p1.x;
		rect.top += p1.y;
		rect.right += p1.x;
		rect.bottom += p1.y;
		MoveHitRegion(p1);
		InvalidateRect(mainWindow, NULL, FALSE);
	}
	void Route::Draw()
	{
			if (isSizing)
			{
				MoveToEx(layer->hDC, anchor.x, anchor.y, NULL);
				LineTo(layer->hDC, endPoint.x, endPoint.y);
			}
			else
			{
				POINT startPoint = routePoints[0];
				POINT nextPoint;
				MoveToEx(layer->hDC, startPoint.x, startPoint.y, NULL);
				for (int i = 1; i < routePoints.size(); ++i)
				{
					nextPoint = routePoints[i];
					LineTo(layer->hDC, nextPoint.x, nextPoint.y);
				}
			}
	}
	HRGN Route::SetSegmentHitRegion(Point* p1, Point* p2)
	{
		POINT corners[4];
		POINT pointOffset;
		long lineLength = p2 - p1;
		long hitAreaWidth = 5;
		pointOffset.x = (p2->y - p1->y) * hitAreaWidth / lineLength;
		pointOffset.y = (p2->x - p1->x) * hitAreaWidth / lineLength;
		corners[0].x = p1->x + pointOffset.x;
		corners[0].y = p1->y - pointOffset.y;
		corners[1].x = p2->x + pointOffset.x;
		corners[1].y = p2->y - pointOffset.y;
		corners[2].x = p2->x - pointOffset.x;
		corners[2].y = p2->y + pointOffset.y;
		corners[3].x = p1->x - pointOffset.x;
		corners[3].y = p1->y + pointOffset.y;
		HRGN hitRegion;
		hitRegion = CreatePolygonRgn(corners, 4, WINDING);
		HRGN ellipseRegion1 = CreateEllipticRgn(p1->x - hitAreaWidth, p1->y - hitAreaWidth, p1->x + hitAreaWidth, p1->y + hitAreaWidth);
		HRGN ellipseRegion2 = CreateEllipticRgn(p2->x - hitAreaWidth, p2->y - hitAreaWidth, p2->x + hitAreaWidth, p2->y + hitAreaWidth);
		CombineRgn(hitRegion, hitRegion, ellipseRegion1, RGN_OR);
		CombineRgn(hitRegion, hitRegion, ellipseRegion2, RGN_OR);
		DeleteObject(ellipseRegion1);
		DeleteObject(ellipseRegion2);
		return hitRegion;
	}
}