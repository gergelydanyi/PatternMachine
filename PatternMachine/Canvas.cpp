#include "Canvas.h"

using namespace PatternMachine;

Canvas::Canvas(HWND hWndParent, HINSTANCE hInstance)
{
    pRectangle = new Rectangle();
    pSelectedShape = pRectangle;
    hWindow = CreateWindowW(
        L"Canvas",
        (LPCTSTR)NULL,
        WS_CHILD | WS_VISIBLE,
        50, 50, 500, 300,
        hWndParent,
        nullptr,
        hInstance,
        this);
    SetupLayers();
}

void Canvas::SetupLayers()
{
    pStage = new Layer(hWindow);
    layers.push_back(pStage);
    pStorage = new Layer(hWindow);
    layers.push_back(pStorage);
    pDrawing = new Layer(hWindow);
    layers.push_back(pDrawing);
    }

void Canvas::SetActivePen()
{
    if (activePen != NULL)
    {
        DeleteObject(activePen);
    }
    (*pDrawing).SetPen(CreatePen(penStyle, penWidth, penColor));
}

void Canvas::SetActiveBrush()
{
    if (activeBrush != NULL)
    {
        DeleteObject(activeBrush);
    }
    (*pDrawing).SetBrush(CreateSolidBrush(brushColor));
}

void Canvas::On_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONDOWN(lParam);
    
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

    BitBlt((*pStage).hDC, 0, 0, (*pStage).rect.right - (*pStage).rect.left, (*pStage).rect.bottom - (*pStage).rect.top, (*pStorage).hDC, 0, 0, SRCCOPY);

    DrawRectangle();

    GdiTransparentBlt((*pStage).hDC, 0, 0, (*pStage).rect.right - (*pStage).rect.left, (*pStage).rect.bottom - (*pStage).rect.top, (*pDrawing).hDC, 0, 0, (*pDrawing).rect.right - (*pDrawing).rect.left, (*pDrawing).rect.bottom - (*pDrawing).rect.top, RGB(1, 1, 1));

    (*pDrawing).Reset();

    BitBlt(hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pStage).hDC, 0, 0, SRCCOPY);
    
    if (!pRectangle->isEditing())
    {
        BitBlt((*pStorage).hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, hDC, 0, 0, SRCCOPY);
    }

    EndPaint(hWindow, &ps);
}

void Canvas::DrawRectangle()
{
    if (!pRectangle->isDrawn)
    {
        ::Rectangle(drawing.hDC, pRectangle->rect.left, pRectangle->rect.top, pRectangle->rect.right, pRectangle->rect.bottom);
        if (!pRectangle->isEditing())
        {
            pRectangle->isDrawn = true;
        }
    }
}