#include "Line.h"

namespace PatternMachine
{
	Line::Line(HWND hwnd) : Line(new Layer(hwnd)) {}

	Line::Line(Layer* pLayer) : Line(*new Point(), *new Point(), pLayer) {}

	Line::Line(Point& p1, Point& p2, Layer* pLayer)
	{
		type = LineShapeType;
		this->p1 = p1;
		this->p2 = p2;
		points[0] = &p1;
		points[1] = &p2;
		SetLayer(pLayer);
	}

	Line::~Line()
	{
		delete& p1;
		delete& p2;
	}

	void Line::StartSizing(POINT p)
	{
		p1.MoveTo(p.x, p.y);
		isSizing = true;
		isDrawn = false;
	}

	void Line::Sizing(POINT p1, POINT p2)
	{
		if (isSizing)
		{
			this->p2.MoveTo(p1.x, p1.y);
			InvalidateRect(mainWindow, NULL, FALSE);
		}
	}

	void Line::StopSizing()
	{
		SetHitRegion();
		isSizing = false;
		InvalidateRect(mainWindow, NULL, FALSE);
	}

	void Line::MoveBy(POINT p)
	{
		for (Point* point : points)
		{
			point->MoveBy(p.x, p.y);
		}
		p1.MoveBy(p.x, p.y);
		p2.MoveBy(p.x, p.y);
		SetHitRegion();
		InvalidateRect(mainWindow, NULL, FALSE);
	}

	void Line::Draw()
	{
		MoveToEx(layer->hDC, p1.x, p1.y, NULL);
		LineTo(layer->hDC, p2.x, p2.y);
	}

	void Line::SetHitRegion()
	{
		POINT corners[4];
		POINT pointOffset;
		long lineLength = p2 - p1;
		long hitAreaWidth = 5;
		pointOffset.x = (p2.x - p1.x) * hitAreaWidth / lineLength;
		pointOffset.y = (p2.y - p1.y) * hitAreaWidth / lineLength;
		corners[0].x = p1.x + pointOffset.y;
		corners[0].y = p1.y - pointOffset.x;
		corners[1].x = p2.x + pointOffset.y;
		corners[1].y = p2.y - pointOffset.x;
		corners[2].x = p2.x - pointOffset.y;
		corners[2].y = p2.y + pointOffset.x;
		corners[3].x = p1.x - pointOffset.y;
		corners[3].y = p1.y + pointOffset.x;
		DeleteObject(hitRegion);
		hitRegion = CreatePolygonRgn(corners, 4, WINDING);
		HRGN ellipseRegion1 = CreateEllipticRgn(p1.x - hitAreaWidth, p1.y - hitAreaWidth, p1.x + hitAreaWidth, p1.y + hitAreaWidth);
		HRGN ellipseRegion2 = CreateEllipticRgn(p2.x - hitAreaWidth, p2.y - hitAreaWidth, p2.x + hitAreaWidth, p2.y + hitAreaWidth);
		CombineRgn(hitRegion, hitRegion, ellipseRegion1, RGN_OR);
		CombineRgn(hitRegion, hitRegion, ellipseRegion2, RGN_OR);
		DeleteObject(ellipseRegion1);
		DeleteObject(ellipseRegion2);
	}

}