#include "Rectangle.h"
#include "Canvas.h"

namespace PatternMachine
{

    Rectangle::Rectangle(Canvas* pCanvas) : Rectangle(new Layer(pCanvas)) {}

    Rectangle::Rectangle(HWND hwnd) : Rectangle(new Layer(hwnd)) {}

    Rectangle::Rectangle(Layer* pLayer)
    {
        type = RectangleShapeType;
        SetLayer(pLayer);
        mainWindow = layer->pCanvas->hWindow;
    }

    Rectangle* Rectangle::Clone()
    {
        Rectangle* clone = new Rectangle(layer->pCanvas);
        clone->anchor = this->anchor;
        clone->rect = this->rect;
        clone->vertices = this->vertices;

        clone->type = this->type;
        clone->mainWindow = this->mainWindow;
        clone->layer->SetPen(this->layer->hPen);
        clone->layer->SetBrush(this->layer->hBrush);
        clone->SetHitRegion();
        return clone;
    }

    void Rectangle::StartSizing(POINT startPoint)
    {
        if (!isMoving)
        {
            anchor = startPoint;
            rect.left = startPoint.x;
            rect.top = startPoint.y;
            rect.right = rect.left + 100;
            rect.bottom = rect.top + 100;
            // TODO: set the following two lines for each shape type, then place to another function in Shape class
            // rotationCenter is required for rotation transformation, and its value should be corrected each time the shape is moved or resized
            rotationCenter.x = (rect.left + rect.right) / 2;
            rotationCenter.y = (rect.top + rect.bottom) / 2;
            vertices.push_back(startPoint);
            vertices.push_back({ rect.right, rect.bottom });
        }
        isSizing = true;
        isDrawn = false;
    }

    void Rectangle::Sizing(POINT movingCorner, POINT motionVector)
    {
        if (isSizing && !isMoving)
        {
            // TODO: calculate accurately the dimensions of the rectangle,
            // it is affected by the thickness of the border (which is based on the penwidth
            RECT rectToBeInvalidated = rect;
            long l = anchor.x;
            long r = movingCorner.x;
            long t = anchor.y;
            long b = movingCorner.y;
            rect.left = r < l ? r : l;
            rect.right = r > l ? r : l;
            rect.top = b < t ? b : t;
            rect.bottom = b > t ? b : t;
            // TODO: set the following two lines for each shape type, then place to another function in Shape class
            // rotationCenter is required for rotation transformation, and its value should be corrected each time the shape is moved or resized
            rotationCenter.x = (rect.left + rect.right) / 2;
            rotationCenter.y = (rect.top + rect.bottom) / 2;
            SetRect(&rectToBeInvalidated,
                rect.left - abs(motionVector.x),
                rect.top - abs(motionVector.y),
                rect.right + abs(motionVector.x),
                rect.bottom + abs(motionVector.y));
            InvalidateRect(mainWindow, &rectToBeInvalidated, FALSE);
            // This is necessary, because drawing leaves garbage on the screen
            // TODO: figure out why this is happening
            // (2026.01.07.) It is messing the screen only when pen width is more than 1
            InvalidateRect(mainWindow, NULL, FALSE);
            vertices[0] = { rect.left, rect.top };
            vertices[1] = { rect.right, rect.bottom };
        }
    }

    void Rectangle::StopSizing()
    {
        SetHitRegion();
        isSizing = false;
        InvalidateRect(mainWindow, &rect, FALSE);
    }

    void Rectangle::MoveBy(POINT p1)
    {
        rect.left += p1.x;
        rect.top += p1.y;
        rect.right += p1.x;
        rect.bottom += p1.y;
        SetHitRegion();
        InvalidateRect(mainWindow, NULL, FALSE);
        vertices[0].x += p1.x;
        vertices[0].y += p1.y;
        vertices[1].x += p1.x;
        vertices[1].y += p1.y;
        // TODO: set the following six lines for each shape type, then place to another function in Shape class
        // rotationCenter is required for rotation transformation, and its value should be corrected each time the shape is moved or resized
        rotationCenter.x += p1.x;
        rotationCenter.y += p1.y;
        layer->xForm2.eDx = (FLOAT)-rotationCenter.x;
        layer->xForm2.eDy = (FLOAT)-rotationCenter.y;
        layer->xForm3.eDx = (FLOAT)rotationCenter.x;
        layer->xForm3.eDy = (FLOAT)rotationCenter.y;
    }

    void Rectangle::SetHitRegion()
    {
        long hitAreaWidth = 5;
        DeleteObject(hitRegion);
        hitRegion = CreateRoundRectRgn(rect.left - hitAreaWidth, rect.top - hitAreaWidth, rect.right + hitAreaWidth + 1, rect.bottom + hitAreaWidth + 1, hitAreaWidth, hitAreaWidth);
    }

    void Rectangle::Draw()
    {
        //::Rectangle(layer->hDC, rect.left, rect.top, rect.right, rect.bottom);
        ::Rectangle(layer->hDC, vertices[0].x, vertices[0].y, vertices[1].x, vertices[1].y);
    }

}