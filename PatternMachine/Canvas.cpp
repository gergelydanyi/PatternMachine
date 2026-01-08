#include "Canvas.h"
#include "Line.h"
#include "Rectangle.h"

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
    pStorage = new Layer(hWindow);
    pDrawing = new Layer(hWindow);
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
    return *pActiveShape;
}

void Canvas::NewShape()
{
    switch (selectedShapeType)
    {
    case LineShapeType:
    {
        pActiveShape = new Line(this);
    }
        break;
    case RectangleShapeType:
    {
        pActiveShape = new Rectangle(this);
    }
    }
    //pActiveShape->SetLayer(new Layer(hWindow));
    layers.push_back(pActiveShape->layer);
    //pActiveShape->layer->Reset();
    //pActiveShape->layer->SetPen(CreatePen(penStyle, penWidth, penColor));
    //pActiveShape->layer->SetBrush(CreateSolidBrush(brushColor));
    pActiveShape->mainWindow = hWindow;
    shapes.push_back(pActiveShape);
}

void Canvas::ChangeBehaviour(CanvasBehaviour behaviour)
{
    this->behaviour = behaviour;
}

void Canvas::SelectHighlightedShapes(bool CtrlPressed)
{
    if (!CtrlPressed)
    {
        for (Shape* pShape : selectedShapes)
        {
            pShape->isSelected = false;
        }
        selectedShapes.clear();
    }
    Shape* pShape = nullptr;
    int j = shapes.size() - 1;
    if (j < 0)
    {
        return;
    }
    while (j >= 0)
    {
        if (PtInRegion(shapes[j]->hitRegion, mouse.X(), mouse.Y()) > 0)
        {
            pShape = shapes[j];
            j = 0;
        }
        --j;
    }
    if (pShape != 0)
    {
        if (!pShape->isSelected)
        {
            pShape->isSelected = true;
            selectedShapes.push_back(pShape);
        }
        else
        {
            pShape->isSelected = false;
            for (int i = selectedShapes.size() - 1; i >= 0; --i)
            {
                if (selectedShapes[i] == pShape)
                {
                    selectedShapes.erase(selectedShapes.begin() + i);
                }
            }
        }
    }
    InvalidateRect(hWindow, NULL, FALSE);
}

void Canvas::DeleteSelection()
{
    if (pActiveShape != 0 && pActiveShape->isSelected)
    {
        pActiveShape = 0;
    }
    for (int i = layers.size() - 1; i >= 0; --i)
    {
        if (shapes[i]->isSelected)
        {
            layers.erase(layers.begin() + i);
        }
    }
    for (int i = shapes.size() - 1; i >= 0; --i)
    {
        if (shapes[i]->isSelected)
        {
            shapes.erase(shapes.begin() + i);
        }
    }
    for (Shape* pShape : selectedShapes)
    {
        delete pShape;
    }
    selectedShapes.clear();
    InvalidateRect(hWindow, NULL, FALSE);
}

void Canvas::On_WM_LBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONDOWN(lParam);
    switch (behaviour)
    {
    case PointingSelection:
    {
        bool CtrlPressed = wParam & MK_CONTROL;
        SelectHighlightedShapes(CtrlPressed);
    }
        break;
    case Drawing:
        {
            NewShape();
            ActiveShape().StartSizing(mouse.LD());
        }
        break;
    }
}

void Canvas::On_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONUP(lParam);
    switch (behaviour)
    {
    case PointingSelection:
        break;
    case Drawing:
        ActiveShape().StopSizing();
        break;
    }
}

void Canvas::On_WM_MOUSEMOVE(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_MOUSEMOVE(lParam);
    switch (behaviour)
    {
    case PointingSelection:
    {
        if (mouse.LeftButtonPressed())
        {
            for (Shape* pShape : selectedShapes)
            {
                pShape->MoveBy(mouse.MotionVector());
            }
        }
        else
        {
            for (Shape* pShape : shapes)
            {
                pShape->HitTest(mouse.CurrentPosition(), mouse.PreviousPosition());
            }
        }
    }
        break;
    case Drawing:
        if (mouse.LeftButtonPressed())
        {
            ActiveShape().Sizing(mouse.CurrentPosition(), mouse.MotionVector());
        }
        break;
    }
}

void Canvas::On_WM_PAINT(WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWindow, &ps);
    RECT clientRect;
    GetClientRect(hWindow, &clientRect);

    pStage->Reset();
    DrawShape();
    DrawSelectedShapes();
    for (Layer* layer : layers)
    {
        GdiTransparentBlt(pStage->hDC, 0, 0, pStage->rect.right - pStage->rect.left, pStage->rect.bottom - pStage->rect.top, layer->hDC, 0, 0, layer->rect.right - layer->rect.left, layer->rect.bottom - layer->rect.top, RGB(1, 1, 1));
    }


    GdiTransparentBlt((*pStage).hDC, 0, 0, (*pStage).rect.right - (*pStage).rect.left, (*pStage).rect.bottom - (*pStage).rect.top, (*pDrawing).hDC, 0, 0, (*pDrawing).rect.right - (*pDrawing).rect.left, (*pDrawing).rect.bottom - (*pDrawing).rect.top, RGB(1, 1, 1));

    (*pDrawing).Reset();

    if (pActiveShape != 0 && !ActiveShape().isEditing())
    {
        BitBlt((*pStorage).hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pStage).hDC, 0, 0, SRCCOPY);
    }
    if (behaviour == PointingSelection)
    {
        DrawHitRegion();
        GdiTransparentBlt((*pStage).hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pDrawing).hDC, 0, 0, (*pDrawing).rect.right - (*pDrawing).rect.left, (*pDrawing).rect.bottom - (*pDrawing).rect.top, RGB(1, 1, 1));
        (*pDrawing).Reset();
    }
    
    BitBlt(hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pStage).hDC, 0, 0, SRCCOPY);

    EndPaint(hWindow, &ps);
}

void Canvas::DrawShape()
{
    if (pActiveShape != 0 && !ActiveShape().isDrawn)
    {
        pActiveShape->layer->Reset();
        /*switch (selectedShapeType)
        {
        case LineShapeType:
        {
            Line& line = (Line&)*pActiveShape;
            //MoveToEx(pDrawing->hDC, line.p1.x, line.p1.y, NULL);
            MoveToEx(pActiveShape->layer->hDC, line.p1.x, line.p1.y, NULL);
            //LineTo(pDrawing->hDC, line.p2.x, line.p2.y);
            LineTo(pActiveShape->layer->hDC, line.p2.x, line.p2.y);
        }
        break;
        case RectangleShapeType:
        {
            Rectangle& rectangle = (Rectangle&)*pActiveShape;
            //::Rectangle((*pDrawing).hDC, rectangle.rect.left, rectangle.rect.top, rectangle.rect.right, rectangle.rect.bottom);
            ::Rectangle(pActiveShape->layer->hDC, rectangle.rect.left, rectangle.rect.top, rectangle.rect.right, rectangle.rect.bottom);
        }
        break;
        }*/
        // Instead of the above:
        pActiveShape->Draw();
        if (!ActiveShape().isEditing())
        {
            ActiveShape().isDrawn = true;
        }
    }
}

void Canvas::DrawSelectedShapes()
{
    for (Shape* shape : selectedShapes)
    {
        shape->layer->Reset();
        switch (shape->type)
        {
        case LineShapeType:
        {
            //Line& line = (Line&)*pActiveShape;
            Line& line = (Line&)*shape;
            //MoveToEx(pDrawing->hDC, line.p1.x, line.p1.y, NULL);
            MoveToEx(shape->layer->hDC, line.p1.x, line.p1.y, NULL);
            //LineTo(pDrawing->hDC, line.p2.x, line.p2.y);
            LineTo(shape->layer->hDC, line.p2.x, line.p2.y);
        }
        break;
        case RectangleShapeType:
        {
            //Rectangle& rectangle = (Rectangle&)*pActiveShape;
            Rectangle& rectangle = (Rectangle&)*shape;
            //::Rectangle((*pDrawing).hDC, rectangle.rect.left, rectangle.rect.top, rectangle.rect.right, rectangle.rect.bottom);
            ::Rectangle(shape->layer->hDC, rectangle.rect.left, rectangle.rect.top, rectangle.rect.right, rectangle.rect.bottom);
        }
        break;
        }
    }
}

void Canvas::DrawHitRegion()
{
    for (Shape* pShape : shapes)
    {
        if (!pShape->isEditing() && pShape->isDrawn && PtInRegion(pShape->hitRegion, mouse.X(), mouse.Y()) > 0)
        {
            FrameRgn(pDrawing->hDC, pShape->hitRegion, pDrawing->hBrush, 1, 1);
        }
    }
    for (Shape* pShape : selectedShapes)
    {
        FrameRgn(pDrawing->hDC, pShape->hitRegion, pDrawing->hBrush, 1, 1);
    }
}