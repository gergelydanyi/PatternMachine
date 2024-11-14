#include "Shape.h"

namespace PatternMachine
{

	// TODO: add inner color
	bool Shape::isEditing()
	{
		return isSizing || isMoving;
	}
	void Shape::StartSizing(POINT)
	{}
	void Shape::Sizing(POINT, POINT)
	{}
	void Shape::StopSizing()
	{
		isSizing = false;
	}
	RECT Shape::BoundingRectangle()
	{
		RECT rc;
		SetRect(&rc, leftmostVertex.x, topmostVertex.y, rightmostVertex.x, bottommostVertex.y);
		return rc;
	}
}