#pragma once
#include "framework.h"
#include "vector"

namespace PatternMachine

{

	class Shape;
	class Canvas;

	class Layer
	{
	public:
		HWND hWindow{};
		HDC hDC{};
		HBITMAP hBitmap{};
		HPEN hPen{};
		HBRUSH hBrush{};
		RECT rect{};
		std::vector<Shape*> shapes{};
		Canvas* pCanvas;

		Layer(Canvas*);
		Layer(HWND);
		~Layer();
		void SetBitmap(HBITMAP);
		void SetPen(HPEN);
		void SetBrush(HBRUSH);
		void Reset();
		void Refresh();
	};

}