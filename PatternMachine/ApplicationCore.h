#pragma once
#include "framework.h"
#include "MouseInfo.h"
#include "Rectangle.h"
#include "Freehand.h"

enum ShapeType
{
	RectangleShapeType,
	LineShapeType,
	FreehandShapeType
};

class ApplicationCore
{
public:
	MouseInfo mouse;
	HWND mainWindow;
	void On_WM_LBUTTONDOWN(LPARAM);
	void On_WM_LBUTTONUP(LPARAM);
	void On_WM_RBUTTONDOWN(LPARAM);
	void On_WM_RBUTTONUP(LPARAM);
	void On_WM_MOUSEMOVE(LPARAM);
	void On_WM_PAINT();
	void On_WM_SIZE();
	void On_WM_VSCROLL();
	void On_WM_HSCROLL();
	void On_WM_INITDIALOG();
	SHORT keyState;
	void CopyToClipboard();
	void SelectShapeType(ShapeType);
	HDC RGBDC;
private:
	HDC clientDC;
	HDC memoryDC;
	HBITMAP memoryBitMap;
	HBITMAP RGBBitMap;
	POINT memoryBitMapTopLeft = { 0, 0 };
	bool bgDragging = false;
	RECT rcClient;
	RECT rcMemory;
	RECT rcRGB;
	PatternMachine::Rectangle rectangleShape;
	PatternMachine::Freehand freehandShape;
	ShapeType selectedShapeType;
	int DrawFrameRect();
	void DrawFreehand();
	void DrawRGB();

	void StartDrawingShape();
	void DrawingShape();
	void StopDrawingShape();

	void SizingRectangle();
	void FinishSizingRectangle();
	void StartMovingRectangle();
	void MoveRectangle();
	void StopMovingRectangle();
	void StartDraggingBackground();
	void DraggingBackground();
	void StopDraggingBackground();

	void ShowColor();
};

