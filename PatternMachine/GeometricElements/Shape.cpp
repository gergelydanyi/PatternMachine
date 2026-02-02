#include "Shape.h"
#include "Canvas.h"

namespace PatternMachine
{

	// TODO: add inner color
	Shape::Shape() {}
	Shape::Shape(Canvas* pCanvas) {}
	Shape::Shape(HWND) {}
	Shape::Shape(Layer* l, ShapeType shapeType) :
		type(shapeType),
		mainWindow(l->pCanvas->hWindow)
	{
		SetLayer(l);
	}
	Shape::Shape(const Shape& s) :
		Shape(new Layer(s.layer->pCanvas), s.type)
	{
		anchor = s.anchor;
		rect = s.rect;
		vertices = s.vertices;
		layer->SetPen(s.layer->hPen);
		layer->SetBrush(s.layer->hBrush);
		SetHitRegion();
	}
	Shape::~Shape() { delete layer; }
	bool Shape::isEditing()	{ return isSizing || isMoving; }
	void Shape::StartSizing(POINT) {}
	void Shape::Sizing(POINT, POINT) {}
	void Shape::StopSizing() { isSizing = false; }
	void Shape::MoveBy(POINT) {}
	void Shape::Refresh()
	{
		isDrawn = false;
		InvalidateRect(mainWindow, NULL, FALSE);
	}
	void Shape::Draw() {}
	void Shape::SetLayer(Layer* layer) { this->layer = layer; }
	void Shape::HitTest(POINT pCurrent, POINT pPrevious)
	{
		if (!isEditing() && PtInRegion(hitRegion, pCurrent.x, pCurrent.y) != PtInRegion(hitRegion, pPrevious.x, pPrevious.y))
		{
			InvalidateRect(mainWindow, NULL, FALSE);
		}
	}
	// DONE: Due to transformations, hitregion does not follow the form and placement of the s anymore,
	// because regions can not be transformed -> regions can be transformed by ExtCreateRegion function
	// TODO: Instead of regions, we should use paths, which can be transformed alongside with the s,
	// and after that it can be converted to a region to do hit testing
	void Shape::SetHitRegion()
	{
		long hitAreaWidth = 5;
		DeleteObject(hitRegion);
		hitRegion = CreateRoundRectRgn(rect.left - hitAreaWidth, rect.top - hitAreaWidth, rect.right + hitAreaWidth + 1, rect.bottom + hitAreaWidth + 1, hitAreaWidth, hitAreaWidth);
	}
	void Shape::MoveHitRegion(POINT p)
	{
		OffsetRgn(hitRegion, p.x, p.y);
	}
	RECT Shape::BoundingRectangle()
	{
		RECT rc;
		SetRect(&rc, leftmostVertex.x, topmostVertex.y, rightmostVertex.x, bottommostVertex.y);
		return rc;
	}
}