#include "Canvas.h"

Canvas::Canvas()
{
    pRectangle = new PatternMachine::Rectangle();
    layers.push_back(&stage);
    layers.push_back(&storage);
    layers.push_back(&drawing);
}

void Canvas::Init(HWND hWnd)
{
    hWindow = hWnd;
    SetupLayers();
}

void Canvas::SetupLayers()
{
    HDC clientDC = GetDC(hWindow);
    RECT clientRect;
    GetClientRect(hWindow, &clientRect);
    for (Layer *layer : layers)
    {
        layer->rect = clientRect;
        layer->hWindow = hWindow;
        layer->hDC = CreateCompatibleDC(clientDC);
        layer->hBitmap = CreateCompatibleBitmap(clientDC, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        SelectObject(layer->hDC, layer->hBitmap);
    }
    ReleaseDC(hWindow, clientDC);
}

void Canvas::On_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONDOWN(lParam);
    drawing.Reset();
//    drawing.SetBrush(CreateSolidBrush(RGB(0, 100, 0)));
//    drawing.SetPen(CreatePen(PS_SOLID, 1, RGB(200, 0, 100)));
    pRectangle = new PatternMachine::Rectangle();
    pRectangle->mainWindow = hWindow;
    pRectangle->StartSizing(mouse.LD());
}

void Canvas::On_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONUP(lParam);
    pRectangle->StopSizing();
    // TODO: this should be placed elsewhere, because we use the object even after releasing left button
    //delete pRectangle;
}

void Canvas::On_WM_MOUSEMOVE(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_MOUSEMOVE(lParam);
    pRectangle->Sizing(mouse.CurrentPosition(), mouse.MotionVector());
}

void Canvas::On_WM_PAINT(WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWindow, &ps);
    RECT clientRect;
    GetClientRect(hWindow, &clientRect);

    BitBlt(stage.hDC, 0, 0, stage.rect.right - stage.rect.left, stage.rect.bottom - stage.rect.top, storage.hDC, 0, 0, SRCCOPY);

    /*HBRUSH hBrush = CreateSolidBrush(RGB(0, 100, 0));
    SelectObject(stage.hDC, hBrush);
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(200, 0, 100));
    SelectObject(stage.hDC, hPen);
    RECT rect = { 10, 10, 70, 50 };
    Rectangle(stage.hDC, rect.left, rect.top, rect.right, rect.bottom);
    DeleteObject(hBrush);*/

    DrawRectangle();

    GdiTransparentBlt(stage.hDC, 0, 0, stage.rect.right - stage.rect.left, stage.rect.bottom - stage.rect.top, drawing.hDC, 0, 0, drawing.rect.right - drawing.rect.left, drawing.rect.bottom - drawing.rect.top, RGB(1, 1, 1));

    drawing.Reset();

    BitBlt(hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, stage.hDC, 0, 0, SRCCOPY);
    
    if (!pRectangle->isEditing())
    {
        BitBlt(storage.hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, hDC, 0, 0, SRCCOPY);
    }

    EndPaint(hWindow, &ps);
}

void Canvas::DrawRectangle()
{
    if (!pRectangle->isDrawn)
    {
        Rectangle(drawing.hDC, pRectangle->rect.left, pRectangle->rect.top, pRectangle->rect.right, pRectangle->rect.bottom);
        if (!pRectangle->isEditing())
        {
            pRectangle->isDrawn = true;
        }
    }
}