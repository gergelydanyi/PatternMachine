#include "Shape.h"

namespace PatternMachine
{

	// TODO: add inner color
	Shape::Shape() {}
	Shape::Shape(Canvas* pCanvas) {}
	Shape::Shape(HWND) {}
	Shape::Shape(Layer* l) { SetLayer(l); }
	Shape::~Shape() { delete layer; }
	Shape* Shape::Clone()
	{
		Shape* clone = new Shape(layer->pCanvas);
		clone->anchor = this->anchor;
		clone->rect = this->rect;
		clone->vertices = this->vertices;

		clone->type = this->type;
		clone->mainWindow = this->mainWindow;
		clone->layer->SetPen(this->layer->hPen);
		clone->layer->SetBrush(this->layer->hBrush);
		clone->SetHitRegion();
		return clone;
	}
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
	// TODO: Due to transformations, hitregion does not follow the form and placement of the shape anymore,
	// because regions can not be transformed
	// Instead of regions, we should use paths, which can be transformed alongside with the shape,
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