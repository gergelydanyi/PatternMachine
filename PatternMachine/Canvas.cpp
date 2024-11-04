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
        layer->SetBitmap(CreateCompatibleBitmap(clientDC, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));
    }
    ReleaseDC(hWindow, clientDC);
}

void Canvas::SetActivePen()
{
    if (activePen != NULL)
    {
        DeleteObject(activePen);
    }
    drawing.SetPen(CreatePen(penStyle, penWidth, penColor));
}

void Canvas::SetActiveBrush()
{
    if (activeBrush != NULL)
    {
        DeleteObject(activeBrush);
    }
    drawing.SetBrush(CreateSolidBrush(brushColor));
}

void Canvas::On_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONDOWN(lParam);
    drawing.Reset();
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