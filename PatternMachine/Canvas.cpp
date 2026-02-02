#include "Canvas.h"
#include "Ellipse.h"
#include "Freehand.h"
#include "Line.h"
#include "Rectangle.h"
#include "Route.h"

using namespace PatternMachine;

Canvas::Canvas(HWND hWndParent, HINSTANCE hInstance) : penStyle(PS_SOLID), penWidth(1), penColor(RGB(255, 0, 255)), brushColor(RGB(0, 255, 255))
{
    hWindow = CreateWindowW(
        L"Canvas",
        (LPCTSTR)NULL,
        WS_CHILD | WS_VISIBLE,
        30, 60, 1200, 900,
        hWndParent,
        nullptr,
        hInstance,
        this);
    SetupLayers();
    //penStyle = PS_SOLID;

}

void Canvas::SetupLayers()
{
    pStage = new Layer(this);
    pStorage = new Layer(this);
    pDrawing = new Layer(this);
    pSelector = new Layer(this);
    pSelector->SetPen(CreatePen(PS_DASH, 1, RGB(170, 170, 170)));
    pSelector->SetBrush(CreateSolidBrush(RGB(1, 1, 1)));

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
    break;
    case EllipseShapeType:
    {
        pActiveShape = new Ellipse(this);
    }
    break;
    case RouteShapeType:
    {
        pActiveShape = new Route(this);
    }
    break;
    case FreehandShapeType:
    {
        pActiveShape = new Freehand(this);
    }
    break;
    }
    layers.push_back(pActiveShape->layer);
    shapes.push_back(pActiveShape);
}

void Canvas::ChangeBehaviour(CanvasBehaviour behaviour)
{
    this->behaviour = behaviour;
}

void Canvas::SelectHighlightedShapes(bool CtrlPressed)
{
    // If CTRL is not pressed, erase all selection
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
    case FramingSelection:
    {
        selectorRect = { mouse.LD().x, mouse.LD().y, mouse.LD().x, mouse.LD().y };
    }
        break;
    case PointingSelection:
    {
        selectionMode = true;
        copyMode = wParam& MK_CONTROL; // CTRL pressed
    }
        break;
    case Drawing:
        {
            NewShape();
            ActiveShape().StartSizing(mouse.LD());
        }
        break;
    case Rotation:
        {
            
        }
        break;
    }
}

void Canvas::On_WM_LBUTTONUP(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_LBUTTONUP(lParam);
    switch (behaviour)
    {
    case FramingSelection:
    {
        for (Shape* pShape : selectedShapes)
        {
            pShape->isSelected = false;
        }
        selectedShapes.clear();
        for (Shape* pShape : shapes)
        {
            // TODO:choose shapes which have intersection with the selector rectangle
            BOOL intersect = RectInRegion(pShape->hitRegion, &selectorRect);
            if (intersect != 0)
            {
                pShape->isSelected = true;
                selectedShapes.push_back(pShape);
            }
        }
        InvalidateRect(hWindow, NULL, FALSE);
    }
    break;
    case PointingSelection:
    {
        if (selectionMode)
        {
            bool CtrlPressed = wParam & MK_CONTROL;
            SelectHighlightedShapes(CtrlPressed);
            selectionMode = false;
        }
        copyMode = false;
    }
    break;
    case Drawing:
    {
        ActiveShape().StopSizing();
    }
    break;
    case Rotation:
    {
        for (auto shape : selectedShapes)
        {
            shape->SetHitRegion();
        }
    }
    break;
    }
}

void Canvas::On_WM_MOUSEMOVE(WPARAM wParam, LPARAM lParam)
{
    mouse.On_WM_MOUSEMOVE(lParam);
    switch (behaviour)
    {
    case FramingSelection:
    {
        if (mouse.LeftButtonPressed())
        {
            if (mouse.CurrentPosition().x < mouse.LD().x)
            {
                selectorRect.left = mouse.CurrentPosition().x;
                selectorRect.right = mouse.LD().x;
            }
            else
            {
                selectorRect.left = mouse.LD().x;
                selectorRect.right = mouse.CurrentPosition().x;
            }
            if (mouse.CurrentPosition().y < mouse.LD().y)
            {
                selectorRect.top = mouse.CurrentPosition().y;
                selectorRect.bottom = mouse.LD().y;
            }
            else
            {
                selectorRect.top = mouse.LD().y;
                selectorRect.bottom = mouse.CurrentPosition().y;
            }
            for (Shape* pShape : selectedShapes)
            {
                pShape->isSelected = false;
            }
            selectedShapes.clear();
            for (Shape* pShape : shapes)
            {
                // TODO:choose shapes which have intersection with the selector rectangle
                BOOL intersect = RectInRegion(pShape->hitRegion, &selectorRect);
                if (intersect != 0)
                {
                    pShape->isSelected = true;
                    selectedShapes.push_back(pShape);
                }
            }
            InvalidateRect(hWindow, NULL, FALSE);
        }
    }
        break;
    case PointingSelection:
    {
        if (mouse.LeftButtonPressed())
        {
            bool CtrlPressed = wParam & MK_CONTROL;
            bool mouseOverSelection = false;
            for (Shape* pShape : selectedShapes)
            {
                mouseOverSelection |= PtInRegion(pShape->hitRegion, mouse.CurrentPosition().x, mouse.CurrentPosition().y);
            }
            if (selectionMode && !mouseOverSelection)
            {
                SelectHighlightedShapes(CtrlPressed);
                selectionMode = false;
            }
            if (mouseOverSelection)
            {
                if (CtrlPressed)
                {
                    if (copyMode)
                    {
                        for (Shape* pShape : selectedShapes)
                        {
                            Shape* newShape = pShape->Clone();
                            layers.push_back(newShape->layer);
                            shapes.push_back(newShape);
                            //newShape->Draw();
                            newShape->layer->DrawShape(newShape);
                        }
                        copyMode = false;
                    }
                }
                //if (selectedShapes.size() < 2)
                //{
                //    SelectHighlightedShapes(CtrlPressed);
                //    selectionMode = false;
                //}
                for (Shape* pShape : selectedShapes)
                {
                    pShape->MoveBy(mouse.MotionVector());
                }
            }
            selectionMode = false;
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
    case Rotation:
        if (mouse.LeftButtonPressed())
        {
            POINT rotationCenter;
            for (Shape* pShape : selectedShapes)
            {
                rotationCenter.x = (pShape->rect.right + pShape->rect.left) / 2;
                rotationCenter.y = (pShape->rect.bottom + pShape->rect.top) / 2;
                FLOAT w = (FLOAT)mouse.CurrentPosition().x - (FLOAT)rotationCenter.x;
                FLOAT h = (FLOAT)mouse.CurrentPosition().y - (FLOAT)rotationCenter.y;
                FLOAT l = (FLOAT)sqrt(w * w + h * h);
                pShape->layer->xForm.eM11 = w / l;
                pShape->layer->xForm.eM12 = h / l;
                pShape->layer->xForm.eM21 = -h / l;
                pShape->layer->xForm.eM22 = w / l;
                pShape->layer->xForm.eDx = 0;// (FLOAT)rotationCenter.x* (1 - w / l) + (FLOAT)rotationCenter.y * h / l;
                pShape->layer->xForm.eDy = 0;// (FLOAT)rotationCenter.y* (1 - w / l) - (FLOAT)rotationCenter.x * h / l;
                pShape->layer->xForm2.eM11 = 1;
                pShape->layer->xForm2.eM12 = 0;
                pShape->layer->xForm2.eM21 = 0;
                pShape->layer->xForm2.eM22 = 1;
                pShape->layer->xForm2.eDx = (FLOAT)-rotationCenter.x;
                pShape->layer->xForm2.eDy = (FLOAT)-rotationCenter.y;
                pShape->layer->xForm3.eM11 = 1;
                pShape->layer->xForm3.eM12 = 0;
                pShape->layer->xForm3.eM21 = 0;
                pShape->layer->xForm3.eM22 = 1;
                pShape->layer->xForm3.eDx = (FLOAT)rotationCenter.x;
                pShape->layer->xForm3.eDy = (FLOAT)rotationCenter.y;
//                pShape->SetHitRegion();
                InvalidateRect(hWindow, NULL, FALSE);
            }
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
    DrawSelectorRectangle();
    // layer -> pStage
    for (Layer* layer : layers)
    {
        GdiTransparentBlt(pStage->hDC, 0, 0, pStage->rect.right - pStage->rect.left, pStage->rect.bottom - pStage->rect.top, layer->hDC, 0, 0, layer->rect.right - layer->rect.left, layer->rect.bottom - layer->rect.top, RGB(1, 1, 1));
    }
    // pDrawing -> pStage | pDrawing contains the hitRegions
    GdiTransparentBlt((*pStage).hDC, 0, 0, (*pStage).rect.right - (*pStage).rect.left, (*pStage).rect.bottom - (*pStage).rect.top, (*pDrawing).hDC, 0, 0, (*pDrawing).rect.right - (*pDrawing).rect.left, (*pDrawing).rect.bottom - (*pDrawing).rect.top, RGB(1, 1, 1));
    (*pDrawing).Reset();
    // pSelector -> pStage
    if (behaviour == FramingSelection)
    {
        // TODO: place selector rectangle drawing in a separate method; set the interior opac and the border to dashed line
        ::Rectangle(pSelector->hDC, selectorRect.left, selectorRect.top, selectorRect.right, selectorRect.bottom);

        GdiTransparentBlt((*pStage).hDC, 0, 0, (*pStage).rect.right - (*pStage).rect.left, (*pStage).rect.bottom - (*pStage).rect.top, (*pSelector).hDC, 0, 0, (*pSelector).rect.right - (*pSelector).rect.left, (*pSelector).rect.bottom - (*pSelector).rect.top, RGB(1, 1, 1));
        (*pSelector).Reset();
    }
    // pStage -> pStorage
    if (pActiveShape != 0 && !ActiveShape().isEditing())
    {
        BitBlt((*pStorage).hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pStage).hDC, 0, 0, SRCCOPY);
    }
    // pDrawing -> pStage
    if (behaviour == PointingSelection || behaviour == FramingSelection)
    {
        DrawHitRegion();
        GdiTransparentBlt((*pStage).hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pDrawing).hDC, 0, 0, (*pDrawing).rect.right - (*pDrawing).rect.left, (*pDrawing).rect.bottom - (*pDrawing).rect.top, RGB(1, 1, 1));
        (*pDrawing).Reset();
    }
    // pStage -> ClientScreen
    BitBlt(hDC, 0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, (*pStage).hDC, 0, 0, SRCCOPY);

    EndPaint(hWindow, &ps);
}



void Canvas::DrawShape()
{
    if (pActiveShape != 0 && !ActiveShape().isDrawn)
    {
        if (pActiveShape->type != FreehandShapeType)
        {
            pActiveShape->layer->Reset();
        }
        //pActiveShape->Draw();
        pActiveShape->layer->DrawShape(pActiveShape);
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
        //shape->Draw();
        shape->layer->DrawShape(shape);
    }
}

void Canvas::DrawSelectorRectangle()
{

}

void Canvas::DrawHitRegion()
{
    for (Shape* pShape : shapes)
    {
        if (!pShape->isEditing() && pShape->isDrawn && PtInRegion(pShape->hitRegion, mouse.X(), mouse.Y()) > 0)
        {
            FrameRgn(pDrawing->hDC, pShape->hitRegion, pDrawing->hBrush, 2, 2);
        }
    }
    for (Shape* pShape : selectedShapes)
    {
        FrameRgn(pDrawing->hDC, pShape->hitRegion, pDrawing->hBrush, 1, 1);
    }
}