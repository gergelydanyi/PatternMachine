#include "Line.h"
#include "Canvas.h"

namespace PatternMachine
{
	Line::Line(Canvas* pCanvas) : Line(new Layer(pCanvas)) {}

	Line::Line(HWND hwnd) : Line(new Layer(hwnd)) {}

	Line::Line(Layer* pLayer) : Line(*new Point(), *new Point(), pLayer) {}

	Line::Line(Point& p1, Point& p2, Layer* pLayer)
	{
		type = LineShapeType;
		this->p1 = p1;
		this->p2 = p2;
		points[0] = &p1;
		points[1] = &p2;
		vertices.push_back({ p1.x, p1.y });
		vertices.push_back({ p2.x, p2.y });
		SetLayer(pLayer);
		mainWindow = layer->pCanvas->hWindow;
	}

	Line::~Line()
	{
		delete& p1;
		delete& p2;
	}

	Line* Line::Clone()
	{
		Line* clone = new Line(layer->pCanvas);
		clone->anchor = this->anchor;
		clone->rect = this->rect;
		clone->vertices = this->vertices;
		clone->p1 = this->p1;
		clone->p2 = this->p2;
		//clone->points[0] = &(clone->p1);
		//clone->points[1] = &(clone->p2);
		clone->type = this->type;
		clone->mainWindow = this->mainWindow;
		clone->layer->SetPen(this->layer->hPen);
		clone->layer->SetBrush(this->layer->hBrush);
		clone->SetHitRegion();
		return clone;
	}

	void Line::StartSizing(POINT p)
	{
		p1.MoveTo(p.x, p.y);
		isSizing = true;
		isDrawn = false;
		rotationCenter.x = (rect.left + rect.right) / 2;
		rotationCenter.y = (rect.top + rect.bottom) / 2;
		vertices[0] = p;
	}

	void Line::Sizing(POINT p1, POINT p2)
	{
		if (isSizing)
		{
			this->p2.MoveTo(p1.x, p1.y);
			InvalidateRect(mainWindow, NULL, FALSE);
			vertices[1] = p1;
			rotationCenter.x = (rect.left + rect.right) / 2;
			rotationCenter.y = (rect.top + rect.bottom) / 2;
		}
	}

	void Line::StopSizing()
	{
		SetHitRegion();
		isSizing = false;
		InvalidateRect(mainWindow, NULL, FALSE);
		rect = { p1.x , p1.y, p2.x, p2.y };
	}

	void Line::MoveBy(POINT p)
	{
		for (Point* point : points)
		{
			point->MoveBy(p.x, p.y);
		}
		p1.MoveBy(p.x, p.y);
		p2.MoveBy(p.x, p.y);
		InvalidateRect(mainWindow, NULL, FALSE);
		vertices[0] = { p1.x, p1.y };
		vertices[1] = { p2.x, p2.y };
		rect = { p1.x, p1.y, p2.x, p2.y };
		rotationCenter.x += p1.x;
		rotationCenter.y += p1.y;
		layer->xForm2.eDx = (FLOAT)-rotationCenter.x;
		layer->xForm2.eDy = (FLOAT)-rotationCenter.y;
		layer->xForm3.eDx = (FLOAT)rotationCenter.x;
		layer->xForm3.eDy = (FLOAT)rotationCenter.y;
		SetHitRegion();
	}

	void Line::Draw()
	{
		//MoveToEx(layer->hDC, p1.x, p1.y, NULL);
		//LineTo(layer->hDC, p2.x, p2.y);
		MoveToEx(layer->hDC, vertices[0].x, vertices[0].y, NULL);
		LineTo(layer->hDC, vertices[1].x, vertices[1].y);
	}

	void Line::SetHitRegion()
	{
		POINT corners[4];
		POINT pointOffset;
		long lineLength = p2 - p1;
		long hitAreaWidth = 5;
		pointOffset.x = (p2.y - p1.y) * hitAreaWidth / lineLength;
		pointOffset.y = (p2.x - p1.x) * hitAreaWidth / lineLength;
		corners[0].x = p1.x + pointOffset.x;
		corners[0].y = p1.y - pointOffset.y;
		corners[1].x = p2.x + pointOffset.x;
		corners[1].y = p2.y - pointOffset.y;
		corners[2].x = p2.x - pointOffset.x;
		corners[2].y = p2.y + pointOffset.y;
		corners[3].x = p1.x - pointOffset.x;
		corners[3].y = p1.y + pointOffset.y;
		DeleteObject(hitRegion);
		hitRegion = CreatePolygonRgn(corners, 4, WINDING);
		HRGN ellipseRegion1 = CreateEllipticRgn(p1.x - hitAreaWidth, p1.y - hitAreaWidth, p1.x + hitAreaWidth, p1.y + hitAreaWidth);
		HRGN ellipseRegion2 = CreateEllipticRgn(p2.x - hitAreaWidth, p2.y - hitAreaWidth, p2.x + hitAreaWidth, p2.y + hitAreaWidth);
		CombineRgn(hitRegion, hitRegion, ellipseRegion1, RGN_OR);
		CombineRgn(hitRegion, hitRegion, ellipseRegion2, RGN_OR);
		DeleteObject(ellipseRegion1);
		DeleteObject(ellipseRegion2);
		RGNDATA rgnData;
		auto rgnDataSize = GetRegionData(hitRegion, 0, NULL);
		std::vector<char> buffer(rgnDataSize);
		auto ret = GetRegionData(hitRegion, rgnDataSize, (LPRGNDATA)buffer.data());
		XFORM x;
		XFORM x2;
		CombineTransform(&x, &layer->xForm2, &layer->xForm);
		CombineTransform(&x2, &x, &layer->xForm3);
		hitRegion = ExtCreateRegion(&x2, ret, (LPRGNDATA)buffer.data());

	}

}