#pragma once
#include "Point.h"
#include "Shape.h"

namespace PatternMachine
{
	class Canvas;
	class Line :
		public Shape
	{
	public:

		Point* points[2];
		Point p1;
		Point p2;

		Line(Canvas*);
		Line(HWND);
		Line(Layer*);
		Line(Point&, Point&, Layer*);
		~Line();
		void StartSizing(POINT) override;
		void Sizing(POINT, POINT) override;
		void StopSizing() override;
		void MoveBy(POINT) override;
		void SetHitRegion() override;
		void Draw() override;
	};

}