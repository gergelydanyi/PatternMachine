#pragma once
#include "framework.h"
#include "MouseInfo.h"

class ApplicationCore
{
public:
	MouseInfo mouse;
	HWND mainWindow;
	void On_WM_LBUTTONDOWN(LPARAM);
	void On_WM_LBUTTONUP(LPARAM);
	void On_WM_PAINT();
	void On_WM_SIZE();
private:
	HDC clientDC;
	HDC memoryDC;
	HBITMAP memoryBitMap;
	RECT rcClient;
	RECT rcMemory;
	void DrawFrameRect();
};

