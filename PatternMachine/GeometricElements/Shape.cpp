#include "Shape.h"

namespace PatternMachine
{

	// TODO: add inner color
	bool Shape::isEditing()	{ return isSizing || isMoving; }
	void Shape::StartSizing(POINT) {}
	void Shape::Sizing(POINT, POINT) {}
	void Shape::StopSizing() { isSizing = false; }
	void Shape::HitTest(POINT pCurrent, POINT pPrevious)
	{
		if (!isEditing() && PtInRegion(hitRegion, pCurrent.x, pCurrent.y) != PtInRegion(hitRegion, pPrevious.x, pPrevious.y))
		{
			InvalidateRect(mainWindow, NULL, FALSE);
		}
	}
	void Shape::SetHitRegion() {}
	RECT Shape::BoundingRectangle()
	{
		RECT rc;
		SetRect(&rc, leftmostVertex.x, topmostVertex.y, rightmostVertex.x, bottommostVertex.y);
		return rc;
	}
}