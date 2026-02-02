#include "Ellipse.h"
#include "Canvas.h"

namespace PatternMachine
{

	Ellipse::Ellipse(Canvas* pCanvas) : Ellipse(new Layer(pCanvas)) {}

	Ellipse::Ellipse(HWND hwnd) : Ellipse(new Layer(hwnd)) {}

    Ellipse::Ellipse(Layer* pLayer) : Shape(pLayer, EllipseShapeType) {}

    Ellipse::Ellipse(const Ellipse& other) : Shape(other) {}

    Ellipse* Ellipse::Clone()
    {
        return new Ellipse(*this);
    }

    void Ellipse::StartSizing(POINT startPoint)
    {
        if (!isMoving)
        {
            anchor = startPoint;
            rect.left = startPoint.x;
            rect.top = startPoint.y;
            rect.right = rect.left + 100;
            rect.bottom = rect.top + 100;
            rotationCenter.x = (rect.left + rect.right) / 2;
            rotationCenter.y = (rect.top + rect.bottom) / 2;
            vertices.push_back(startPoint);
            vertices.push_back({ rect.right, rect.bottom });
        }
        isSizing = true;
        isDrawn = false;
    }
    void Ellipse::Sizing(POINT movingCorner, POINT motionVector)
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

    void Ellipse::StopSizing()
    {
        SetHitRegion();
        isSizing = false;
        InvalidateRect(mainWindow, &rect, FALSE);
    }

    void Ellipse::MoveBy(POINT p1)
    {
        rect.left += p1.x;
        rect.top += p1.y;
        rect.right += p1.x;
        rect.bottom += p1.y;
        rotationCenter.x += p1.x;
        rotationCenter.y += p1.y;
        layer->xForm2.eDx = (FLOAT)-rotationCenter.x;
        layer->xForm2.eDy = (FLOAT)-rotationCenter.y;
        layer->xForm3.eDx = (FLOAT)rotationCenter.x;
        layer->xForm3.eDy = (FLOAT)rotationCenter.y;
        InvalidateRect(mainWindow, NULL, FALSE);
        vertices[0].x += p1.x;
        vertices[0].y += p1.y;
        vertices[1].x += p1.x;
        vertices[1].y += p1.y;
        SetHitRegion();
    }
    
    void Ellipse::SetHitRegion()
    {
        long hitAreaWidth = 5;
        DeleteObject(hitRegion);
        hitRegion = CreateEllipticRgn(rect.left - hitAreaWidth, rect.top - hitAreaWidth, rect.right + hitAreaWidth + 1, rect.bottom + hitAreaWidth + 1);
        RGNDATA rgnData;
        auto rgnDataSize = GetRegionData(hitRegion, 0, NULL);
        std::vector<char> buffer(rgnDataSize);
        auto ret = GetRegionData(hitRegion, rgnDataSize, (LPRGNDATA)buffer.data());
        XFORM x;
        XFORM x2;
        CombineTransform(&x, &layer->xForm2, &layer->xForm);
        CombineTransform(&x2, &x, &layer->xForm3);
        hitRegion = ExtCreateRegion(&x2, ret, (LPRGNDATA)buffer.data());
    }

    void Ellipse::Draw()
    {
        //::Ellipse(layer->hDC, rect.left, rect.top, rect.right, rect.bottom);
        ::Ellipse(layer->hDC, vertices[0].x, vertices[0].y, vertices[1].x, vertices[1].y);
    }


}