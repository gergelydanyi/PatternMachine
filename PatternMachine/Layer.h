#pragma once
#include "framework.h"
#include "vector"
//#include "Shape.h"


namespace PatternMachine

{

	class Shape;

	class Layer
	{
		//friend class Shape;
	public:
		HWND hWindow{};
		HDC hDC{};
		HBITMAP hBitmap{};
		HPEN hPen{};
		HBRUSH hBrush{};
		RECT rect{};
		std::vector<Shape*> shapes{};

		Layer(HWND);
		~Layer();
		void SetBitmap(HBITMAP);
		void SetPen(HPEN);
		void SetBrush(HBRUSH);
		void Reset();
		void Refresh();
	};

}