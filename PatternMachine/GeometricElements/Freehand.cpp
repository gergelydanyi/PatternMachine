#include "Freehand.h"
#include "Canvas.h"
namespace PatternMachine
{
	Freehand::Freehand(Canvas* pCanvas) : Freehand(new Layer(pCanvas)) {}

	Freehand::Freehand(HWND hwnd) : Freehand(new Layer(hwnd)) {}

	Freehand::Freehand(Layer* pLayer)
	{
		type = FreehandShapeType;
		SetLayer(pLayer);
		mainWindow = layer->pCanvas->hWindow;
	}

	void Freehand::StartSizing(POINT startPoint)
	{
		isSizing = true;
		isDrawn = false;
		anchor = startPoint;
		vertices.push_back(startPoint);
		previousPoint = anchor;
		currentPoint = anchor;
		SetRect(&rect, anchor.x, anchor.y, anchor.x, anchor.y);
		InvalidateRect(mainWindow, &rect, FALSE);
	}
	void Freehand::Sizing(POINT prevPoint, POINT currPoint)
	{
		if (isSizing)
		{
			currentPoint = prevPoint;
			vertices.push_back(prevPoint);
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
		SetHitRegion();
		InvalidateRect(mainWindow, NULL, FALSE);
	}
	void Freehand::SetHitRegion()
	{
		long hitAreaWidth = 5;
		DeleteObject(hitRegion);
		//hitRegion = CreateRoundRectRgn(rect.left - hitAreaWidth, rect.top - hitAreaWidth, rect.right + hitAreaWidth + 1, rect.bottom + hitAreaWidth + 1, hitAreaWidth, hitAreaWidth);
		hitRegion = CreateEllipticRgn(anchor.x - hitAreaWidth, anchor.y - hitAreaWidth, anchor.x + hitAreaWidth, anchor.y + hitAreaWidth);
		for (int i = 1; i < vertices.size(); ++i)
		{
			Point* p1 = new Point(vertices[i].x, vertices[i].y);
			Point* p2 = new Point(vertices[i - 1].x, vertices[i - 1].y);
			HRGN segmentHitRegion = SetSegmentHitRegion(p1, p2);
			CombineRgn(hitRegion, hitRegion, segmentHitRegion, RGN_OR);
			DeleteObject(segmentHitRegion);
			delete p1;
			delete p2;
		}
	}
	void Freehand::MoveBy(POINT p1)
	{
		for (int i = 0; i < vertices.size(); ++i)
		{
			vertices[i].x += p1.x;
			vertices[i].y += p1.y;
		}
		rect.left += p1.x;
		rect.top += p1.y;
		rect.right += p1.x;
		rect.bottom += p1.y;
		MoveHitRegion(p1);
		InvalidateRect(mainWindow, NULL, FALSE);
	}
	void Freehand::Draw()
	{
		if (isSizing)
		{
			if (previousPoint.x == currentPoint.x &&
				previousPoint.y == currentPoint.y)
			{
				SetPixelV(layer->hDC, previousPoint.x, previousPoint.y, RGB(200, 200, 0));
			}
			else
			{
				MoveToEx(layer->hDC, previousPoint.x, previousPoint.y, NULL);
				LineTo(layer->hDC, currentPoint.x, currentPoint.y);
			}
			previousPoint = currentPoint;
		}
		else
		{
			POINT startPoint = vertices[0];
			POINT nextPoint;
			MoveToEx(layer->hDC, startPoint.x, startPoint.y, NULL);
			for (int i = 1; i < vertices.size(); ++i)
			{
				nextPoint = vertices[i];
				LineTo(layer->hDC, nextPoint.x, nextPoint.y);
			}
		}
	}
	HRGN Freehand::SetSegmentHitRegion(Point* p1, Point* p2)
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
