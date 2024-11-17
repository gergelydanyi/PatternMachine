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
    pRectangle = new Rectangle();
    pRectangle->mainWindow = hWindow;
    pLine = (Line*)NewShape(LineShapeType);
    // TODO: this should be moved to the Shape constructor
    pLine->mainWindow = hWindow;
    pActiveShape = pLine;
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

void Canvas::SetActiveShapeType(ShapeType shapeType)
{
    switch (shapeType)
    {
    case RectangleShapeType:
        pActiveShape = pRectangle;
        break;
    case LineShapeType:
        pActiveShape = pLine;
        break;
    }
}

Shape* Canvas::NewShape(ShapeType t)
{
    Shape* pShape = nullptr;
    switch (t)
    {
    case LineShapeType:
    {
        Point* p1 = new Point();
        Point* p2 = new Point();
        pShape = new Line(*p1, *p2);
    }
        break;
    case RectangleShapeType:
    {
        pShape = new Rectangle();
    }
    }
    return pShape;
}

void Canvas::On_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONDOWN(lParam);
    //pLine->StartSizing(mouse.LD());
    pActiveShape->StartSizing(mouse.LD());
    /*pRectangle = new PatternMachine::Rectangle();
    pRectangle->mainWindow = hWindow;
    pRectangle->StartSizing(mouse.LD());*/
}

void Canvas::On_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONUP(lParam);
    //pLine->StopSizing();
    pActiveShape->StopSizing();
    /*pRectangle->StopSizing();*/
    // TODO: this should be placed elsewhere, because we use the object even after releasing left button
    //delete pRectangle;
}

void Canvas::On_WM_MOUSEMOVE(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_MOUSEMOVE(lParam);
    //pLine->Sizing(mouse.CurrentPosition(), mouse.MotionVector());
    pActiveShape->Sizing(mouse.CurrentPosition(), mouse.MotionVector());
    pActiveShape->HitTest(mouse.CurrentPosition(), mouse.PreviousPosition());
    /*pRectangle->Sizing(mouse.CurrentPosition(), mouse.MotionVector());*/
}

void Canvas::On_WM_PAINT(WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWindow, &ps);
    RECT clientRect;
    GetClientRect(hWindow, &clientRect);

    BitBlt((*pStage).hDC, 0, 0, (*pStage).rect.right - (*pStage).rect.left, (*pStage).rect.bottom - (*pStage).rect.top, (*pStorage).hDC, 0, 0, SRCCOPY);

    DrawRectangle();
    DrawLine();

    GdiTransparentBlt((*pStage).hDC, 0, 0, (*pStage).rect.right - (*pStage).rect.left, (*pStage).rect.bottom - (*pStage).rect.top, (*pDrawing).hDC, 0, 0, (*pDrawing).rect.right - (*pDrawing).rect.left, (*pDrawing).rect.bottom - (*pDrawing).rect.top, RGB(1, 1, 1));

    (*pDrawing).Reset();

    BitBlt(hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pStage).hDC, 0, 0, SRCCOPY);
    
    //if (!pRectangle->isEditing())
    if (!pActiveShape->isEditing())
    {
        BitBlt((*pStorage).hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, hDC, 0, 0, SRCCOPY);
    }
    DrawHitRegion(hDC);
    GdiTransparentBlt(hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pDrawing).hDC, 0, 0, (*pDrawing).rect.right - (*pDrawing).rect.left, (*pDrawing).rect.bottom - (*pDrawing).rect.top, RGB(1, 1, 1));
    (*pDrawing).Reset();

    EndPaint(hWindow, &ps);
}

void Canvas::DrawRectangle()
{
    if (!pRectangle->isDrawn)
    {
        ::Rectangle((*pDrawing).hDC, pRectangle->rect.left, pRectangle->rect.top, pRectangle->rect.right, pRectangle->rect.bottom);
        if (!pRectangle->isEditing())
        {
            pRectangle->isDrawn = true;
        }
    }
}

void Canvas::DrawLine()
{
    if (!pLine->isDrawn)
    {
        MoveToEx(pDrawing->hDC, pLine->p1.x, pLine->p1.y, NULL);
        LineTo(pDrawing->hDC, pLine->p2.x, pLine->p2.y);
        if (!pLine->isEditing())
        {
            pLine->isDrawn = true;
        }
    }
}

void Canvas::DrawHitRegion(HDC hDC)
{
    if (!pActiveShape->isEditing() && pActiveShape->isDrawn && PtInRegion(pActiveShape->hitRegion, mouse.X(), mouse.Y()) > 0)
    {
        FrameRgn(pDrawing->hDC, pActiveShape->hitRegion, pDrawing->hBrush, 1, 1);
    }
}