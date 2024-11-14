#pragma once
#include "framework.h"
#include "MouseInfo.h"
#include "Rectangle.h"
#include "Shape.h"
#include "Layer.h"

namespace PatternMachine
{


class Canvas
{
public:
	MouseInfo mouse;
	HWND hWindow{};
	HPEN hPen{};
	HPEN activePen{};
	HBRUSH activeBrush{};
	COLORREF penColor;
	int penWidth;
	int penStyle;
	COLORREF brushColor;
	/*	HBRUSH hBrush{};
	HDC	memoryDCstorage{};
	HDC memoryDCdrawing{};
	HBITMAP memoryBitmapStorage{};
	HBITMAP memoryBitmapDrawing{};*/
	Rectangle* pRectangle;

	Canvas(HWND, HINSTANCE);
	void Init(HWND);
	void SetupLayers();
	void On_WM_PAINT(WPARAM, LPARAM);
	void On_WM_LBUTTONDOWN(WPARAM, LPARAM);
	void On_WM_LBUTTONUP(WPARAM, LPARAM);
	void On_WM_MOUSEMOVE(WPARAM, LPARAM);
	void SetActivePen();
	void SetActiveBrush();

private:
	std::vector<Layer*> layers;
	Layer* pStage{};
	Layer* pStorage{};
	Layer* pDrawing{};

	void DrawRectangle();
};

}
