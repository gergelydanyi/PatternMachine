#include "Layer.h"

Layer::Layer(HWND clientWindow)
{
	HDC clientDC = GetDC(clientWindow);
	hWindow = clientWindow;
	GetClientRect(hWindow, &rect);
	hDC = CreateCompatibleDC(clientDC);
	SetBitmap(CreateCompatibleBitmap(clientDC, rect.right - rect.left, rect.bottom - rect.top));
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
	SelectObject(hDC, hBitmap);
}

void Layer::SetPen(HPEN h)
{
	hPen = h;
	SelectObject(hDC, hPen);
}

void Layer::SetBrush(HBRUSH h)
{
	hBrush = h;
	SelectObject(hDC, hBrush);
}

void Layer::Reset()
{
	RECT rc = { 0, 0, rect.right - rect.left, rect.bottom - rect.top };
	HBRUSH hbr = CreateSolidBrush(RGB(1, 1, 1));
	FillRect(hDC, &rc, hbr);
	DeleteObject(hbr);
}