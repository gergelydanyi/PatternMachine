#pragma once
#include "framework.h"
#include "Pen.h"
#include "Brush.h"
#include "Layer.h"
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

	//class Layer;

	class Shape
	{
	public:
		Shape();
		Shape(Layer*);
		~Shape();
		POINT anchor;
		RECT rect = { 0, 0, 100, 100 };
		virtual RECT BoundingRectangle();
		bool isEditing();
		virtual void StartSizing(POINT);
		virtual void Sizing(POINT, POINT);
		virtual void StopSizing();
		virtual void MoveBy(POINT);
		virtual void Draw();
		void SetLayer(Layer*);
		bool isSizing = false;
		bool isMoving = false;
		bool isDrawn = true;
		bool isSelected = false;
		ShapeType type;
		HWND mainWindow;
		HRGN hitRegion;
		Pen pen;
		Brush brush;
		Layer* layer;

		void HitTest(POINT, POINT);
		virtual void SetHitRegion();
	private:
		std::vector<POINT> vertices;
		POINT topmostVertex;
		POINT rightmostVertex;
		POINT bottommostVertex;
		POINT leftmostVertex;
	};

}

