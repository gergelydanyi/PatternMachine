#pragma once
#include "framework.h"

class Layer
{
public:
	HWND hWindow{};
	HDC hDC{};
	HBITMAP hBitmap{};
	HPEN hPen{};
	HBRUSH hBrush{};
	RECT rect{};

	Layer(HWND);
	~Layer();
	void SetBitmap(HBITMAP);
	void SetPen(HPEN);
	void SetBrush(HBRUSH);
	void Reset();
};

