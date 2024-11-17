#pragma once
#include "framework.h"
#include <vector>

namespace PatternMachine
{

	enum ShapeType
	{
		RectangleShapeType,
		LineShapeType,
		FreehandShapeType,
		RouteShapeType
	};

	class Shape
	{
	public:
		POINT anchor;
		RECT rect = { 0, 0, 100, 100 };
		virtual RECT BoundingRectangle();
		virtual bool isEditing();
		virtual void StartSizing(POINT);
		virtual void Sizing(POINT, POINT);
		virtual void StopSizing();
		bool isSizing = false;
		bool isMoving = false;
		bool isDrawn = true;
		HWND mainWindow;
		HRGN hitRegion;
		virtual void HitTest(POINT, POINT);
		virtual void SetHitRegion();
	private:
		std::vector<POINT> vertices;
		POINT topmostVertex;
		POINT rightmostVertex;
		POINT bottommostVertex;
		POINT leftmostVertex;
	};

}

