#pragma once
#include "framework.h"
#include "Pen.h"
#include "Point.h"
#include "Brush.h"
#include "Layer.h"
#include <vector>

namespace PatternMachine
{

	enum ShapeType
	{
		RectangleShapeType,
		EllipseShapeType,
		LineShapeType,
		FreehandShapeType,
		RouteShapeType
	};

	class Shape
	{
	protected:
		Shape();
		Shape(Canvas*); // Use this when shape is created by Canvas
		Shape(HWND);
		Shape(Layer*, ShapeType); // Use this when shape is created on an existing layer
		Shape(const Shape&);
	public:
		~Shape();
		virtual Shape* Clone() = 0;
		POINT anchor;
		POINT rotationCenter;
		RECT rect = { 0, 0, 100, 100 };
		std::vector<POINT> vertices;
		LPPOINT POINTS; // This is for drawing, initiated from the layer. When attempted to start drawing from layer by vertices, DPtoLP function caused a crash.
		virtual RECT BoundingRectangle();
		bool isEditing();
		virtual void StartSizing(POINT);
		virtual void Sizing(POINT, POINT);
		virtual void StopSizing();
		virtual void MoveBy(POINT);
		void Refresh();
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
		void MoveHitRegion(POINT);
	protected:
		POINT topmostVertex;
		POINT rightmostVertex;
		POINT bottommostVertex;
		POINT leftmostVertex;
	};

}

