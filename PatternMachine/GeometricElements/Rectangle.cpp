#include "Rectangle.h"

namespace PatternMachine
{

    //RECT Rectangle::BoundingRectangle()
    //{
    //    RECT rc;
    //    return rc;
    //}

    Rectangle::Rectangle()
    {
        type = RectangleShapeType;
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
            SetRect(&rectToBeInvalidated,
                rect.left - abs(motionVector.x),
                rect.top - abs(motionVector.y),
                rect.right + abs(motionVector.x),
                rect.bottom + abs(motionVector.y));
            InvalidateRect(mainWindow, &rectToBeInvalidated, FALSE);
            // This is necessary, because drawing leaves garbage on the screen
            // TODO: figure out why this is happening
            InvalidateRect(mainWindow, NULL, FALSE);
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
    }

    void Rectangle::SetHitRegion()
    {
        long hitAreaWidth = 5;
        DeleteObject(hitRegion);
        hitRegion = CreateRoundRectRgn(rect.left - hitAreaWidth, rect.top - hitAreaWidth, rect.right + hitAreaWidth + 1, rect.bottom + hitAreaWidth + 1, hitAreaWidth, hitAreaWidth);
    }

}