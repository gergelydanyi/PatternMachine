#include "Layer.h"
#include "Canvas.h"

namespace PatternMachine
{

	Layer::Layer(Canvas* pCanvas) : Layer(pCanvas->hWindow)
	{
		this->pCanvas = pCanvas;
		SetPen(CreatePen(pCanvas->penStyle, pCanvas->penWidth, pCanvas->penColor));
		SetBrush(CreateSolidBrush(pCanvas->brushColor));
	}

	Layer::Layer(HWND clientWindow)
	{
		HDC clientDC = GetDC(clientWindow);
		hWindow = clientWindow;
		GetClientRect(hWindow, &rect);
		hDC = CreateCompatibleDC(clientDC);
		SetBitmap(CreateCompatibleBitmap(clientDC, rect.right - rect.left, rect.bottom - rect.top));
		SetPen(CreatePen(PS_SOLID, 1, RGB(0, 255, 0)));
		SetBrush(CreateSolidBrush(RGB(255, 0, 0)));
		SetBkMode(hDC, TRANSPARENT);
		ReleaseDC(clientWindow, clientDC);
		Reset();
	}

	Layer::~Layer()
	{
		DeleteDC(hDC);
		DeleteObject(hBitmap);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	}

	void Layer::SetBitmap(HBITMAP h)
	{
		hBitmap = h;
		DeleteObject(SelectObject(hDC, hBitmap));
	}

	void Layer::SetPen(HPEN h)
	{
		hPen = h;
		DeleteObject(SelectObject(hDC, hPen));
	}

	void Layer::SetBrush(HBRUSH h)
	{
		hBrush = h;
		DeleteObject(SelectObject(hDC, hBrush));
	}

	void Layer::Reset()
	{
		RECT rc = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
		HBRUSH hbr = CreateSolidBrush(RGB(1, 1, 1));
		FillRect(hDC, &rc, hbr);
		DeleteObject(hbr);
	}

}