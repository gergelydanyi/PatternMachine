#include "Canvas.h"

using namespace PatternMachine;

Canvas::Canvas(HWND hWndParent, HINSTANCE hInstance)
{
    hWindow = CreateWindowW(
        L"Canvas",
        (LPCTSTR)NULL,
        WS_CHILD | WS_VISIBLE,
        0, 30, 800, 600,
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

Shape& Canvas::ActiveShape()
{
    switch (selectedShapeType)
    {
    case RectangleShapeType:
        pActiveShape = pRectangle;
        break;
    case LineShapeType:
        pActiveShape = pLine;
        break;
    }
    return *pActiveShape;
}

void Canvas::NewShape()
{
    Shape* pShape = nullptr;
    switch (selectedShapeType)
    {
    case LineShapeType:
    {
        Point* p1 = new Point();
        Point* p2 = new Point();
        pLine = new Line(*p1, *p2);
        pShape = pLine;
        pShape->mainWindow = hWindow;
    }
        break;
    case RectangleShapeType:
    {
        pRectangle = new Rectangle();
        pShape = pRectangle;
        pShape->mainWindow = hWindow;
    }
    }
    shapes.push_back(pShape);
}

void Canvas::On_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONDOWN(lParam);
    if (!editingMode)
    {
        editingMode = true;
        NewShape();
        ActiveShape().StartSizing(mouse.LD());
    }
    else
    {
    }
}

void Canvas::On_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONUP(lParam);
    if (editingMode)
    {
        editingMode = false;
        ActiveShape().StopSizing();
    }
}

void Canvas::On_WM_MOUSEMOVE(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_MOUSEMOVE(lParam);
    if (editingMode)
    {
        ActiveShape().Sizing(mouse.CurrentPosition(), mouse.MotionVector());
    }
    else
    {
        for (Shape* pShape : shapes)
        {
            pShape->HitTest(mouse.CurrentPosition(), mouse.PreviousPosition());
        }
    }
}

void Canvas::On_WM_PAINT(WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWindow, &ps);
    RECT clientRect;
    GetClientRect(hWindow, &clientRect);

    BitBlt((*pStage).hDC, 0, 0, (*pStage).rect.right - (*pStage).rect.left, (*pStage).rect.bottom - (*pStage).rect.top, (*pStorage).hDC, 0, 0, SRCCOPY);

    DrawShape();

    GdiTransparentBlt((*pStage).hDC, 0, 0, (*pStage).rect.right - (*pStage).rect.left, (*pStage).rect.bottom - (*pStage).rect.top, (*pDrawing).hDC, 0, 0, (*pDrawing).rect.right - (*pDrawing).rect.left, (*pDrawing).rect.bottom - (*pDrawing).rect.top, RGB(1, 1, 1));

    (*pDrawing).Reset();

    BitBlt(hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pStage).hDC, 0, 0, SRCCOPY);
    
    if (pActiveShape != 0 && !ActiveShape().isEditing())
    {
        BitBlt((*pStorage).hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, hDC, 0, 0, SRCCOPY);
    }
    if (!editingMode)
    {
        DrawHitRegion(hDC);
        GdiTransparentBlt(hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pDrawing).hDC, 0, 0, (*pDrawing).rect.right - (*pDrawing).rect.left, (*pDrawing).rect.bottom - (*pDrawing).rect.top, RGB(1, 1, 1));
        (*pDrawing).Reset();
    }

    EndPaint(hWindow, &ps);
}

void Canvas::DrawShape()
{
    if (pActiveShape != 0 && !ActiveShape().isDrawn)
    {
        switch (selectedShapeType)
        {
        case LineShapeType:
        {
            MoveToEx(pDrawing->hDC, pLine->p1.x, pLine->p1.y, NULL);
            LineTo(pDrawing->hDC, pLine->p2.x, pLine->p2.y);
        }
        break;
        case RectangleShapeType:
        {
            ::Rectangle((*pDrawing).hDC, pRectangle->rect.left, pRectangle->rect.top, pRectangle->rect.right, pRectangle->rect.bottom);
        }
        break;
        }
        if (!ActiveShape().isEditing())
        {
            ActiveShape().isDrawn = true;
        }
    }
}

void Canvas::DrawHitRegion(HDC hDC)
{
    for (Shape* pShape : shapes)
    {
        if (!pShape->isEditing() && pShape->isDrawn && PtInRegion(pShape->hitRegion, mouse.X(), mouse.Y()) > 0)
        {
            FrameRgn(pDrawing->hDC, pShape->hitRegion, pDrawing->hBrush, 1, 1);
        }
    }
}