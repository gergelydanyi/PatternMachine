#include "Ellipse.h"
#include "Canvas.h"

namespace PatternMachine
{

	Ellipse::Ellipse(Canvas* pCanvas) : Ellipse(new Layer(pCanvas)) {}

	Ellipse::Ellipse(HWND hwnd) : Ellipse(new Layer(hwnd)) {}

	Ellipse::Ellipse(Layer* pLayer)
	{
		type = EllipseShapeType;
		SetLayer(pLayer);
		mainWindow = pLayer->pCanvas->hWindow;
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
        SetHitRegion();
        InvalidateRect(mainWindow, NULL, FALSE);
    }

    void Ellipse::SetHitRegion()
    {
        long hitAreaWidth = 5;
        DeleteObject(hitRegion);
        hitRegion = CreateRoundRectRgn(rect.left - hitAreaWidth, rect.top - hitAreaWidth, rect.right + hitAreaWidth + 1, rect.bottom + hitAreaWidth + 1, hitAreaWidth, hitAreaWidth);
    }

    void Ellipse::Draw()
    {
        ::Ellipse(layer->hDC, rect.left, rect.top, rect.right, rect.bottom);
    }


}