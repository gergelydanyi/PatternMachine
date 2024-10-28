#include "ApplicationCore.h"
#include <string>
#include <time.h>
#include <limits>

ApplicationCore::ApplicationCore()
{
    penColor = RGB(255, 0, 0);
    penWidth = 20;
    penStyle = PS_SOLID;
}

void ApplicationCore::On_WM_LBUTTONDOWN(LPARAM lParam)
{
    mouse.On_WM_LBUTTONDOWN(lParam);
    StartDrawingShape();
}

void ApplicationCore::On_WM_LBUTTONUP(LPARAM lParam)
{
    mouse.On_WM_LBUTTONUP(lParam);
    StopDrawingShape();
}

void ApplicationCore::On_WM_RBUTTONDOWN(LPARAM lParam)
{
    mouse.On_WM_RBUTTONDOWN(lParam);
    if (rectangleShape.isSizing)
    {
        StartMovingRectangle();
    }
    if (!rectangleShape.isSizing)
    {
        StartDraggingBackground();
    }
}

void ApplicationCore::On_WM_RBUTTONUP(LPARAM lParam)
{
    mouse.On_WM_RBUTTONUP(lParam);
    StopMovingRectangle();
    StopDraggingBackground();
}

void ApplicationCore::On_WM_MOUSEMOVE(LPARAM lParam)
{
    mouse.On_WM_MOUSEMOVE(lParam);
    DrawingShape();
    MoveRectangle();
    DraggingBackground();
    ShowColor();
}

void ApplicationCore::On_WM_PAINT()
{
    PAINTSTRUCT ps;
    clientDC = BeginPaint(mainWindow, &ps);

    // TODO: calculate the clipping area accurately, based on the height of the rebar; store the handle of the rebar
    HRGN hRgn = CreateRectRgn(0, 40, rcClient.right, rcClient.bottom);
    SelectClipRgn(clientDC, hRgn);
    BitBlt(clientDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memoryDC, memoryBitMapTopLeft.x, memoryBitMapTopLeft.y, SRCCOPY);
    DrawFrameRect();
    DrawFreehand();
    DrawRoute();

    if (!(
        rectangleShape.isEditing() || routeShape.isEditing()
        ))
    {
        BitBlt(memoryDC, memoryBitMapTopLeft.x, memoryBitMapTopLeft.y, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, clientDC, 0, 0, SRCCOPY);
    }
    EndPaint(mainWindow, &ps);
    DeleteObject(hRgn);
}

void ApplicationCore::On_WM_SIZE()
{
    // TODO: remove the unnecessary parts used for debugging
    // TODO: initialize memoryDC before this method, ex. during the creation of the window
    // TODO: use a separate method to set the size of the bitmap, ex. in a separate class, where the background should take place
    if (memoryDC == NULL)
    {
        GetClientRect(mainWindow, &rcClient);
        rcMemory = rcClient;
        OffsetRect(&rcMemory, rcMemory.left - rcMemory.right, rcMemory.top - rcMemory.bottom);
        HDC clntDC = GetDC(mainWindow);
        memoryDC = CreateCompatibleDC(clntDC);
        memoryBitMap = CreateCompatibleBitmap(clntDC, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
        SelectObject(memoryDC, memoryBitMap);
        BitBlt(memoryDC, memoryBitMapTopLeft.x, memoryBitMapTopLeft.y, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, clntDC, 0, 0, SRCCOPY);
        // This is only for DEBUG
        
        RECT rc = { 0, 0, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top };
        HBRUSH hbr = CreateSolidBrush(RGB(255, 150, 150));
        FillRect(memoryDC, &rc, hbr);
        DeleteObject(hbr);
//        SetRect(&rc, 240, 240, 485, 485);
//        hbr = CreateSolidBrush(RGB(100, 200, 180));
//        FillRect(memoryDC, &rc, hbr);
        DeleteObject(hbr);
        if (false)
        {
            //grid
            int gridWidth = 20;
            int lineWidth = 2;
            for (int i = 0; i < rcClient.right - rcClient.left; ++i)
            {
                for (int j = 0; j < rcClient.bottom - rcClient.top; ++j)
                {
                    long r = 150;
                    long g = 150;
                    long b = 150;
                    if ((i % gridWidth < gridWidth - lineWidth) && (j % gridWidth < gridWidth - lineWidth))
                    {
                        r = g = b = 150;
                    }
                    else
                    {
                        r = g = b = 130;
                    }
                    SetPixelV(memoryDC, i, j, RGB(r, g, b));
                }
            }
            SetPixelV(memoryDC, 1, 1, RGB(255, 0, 0));
            //grid
        }
        if (false)
        {
            // rectangles for example
            int repeat = 5;
            int side = 20;
            for (int i = 0; i < repeat; ++i)
            {
                for (int j = 0; j < repeat; ++j)
                {
                    rc = { 3 + side * i,
                           3 + side * j,
                           3 + (side + 1) * i,
                           3 + (side + 1) * j };
                    HBRUSH hbrFrame = CreateSolidBrush(RGB(0, 0, 0));
                    FrameRect(memoryDC, &rc, hbrFrame);
                    DeleteObject(hbrFrame);

                    rc = { 13 + side * i,
                           3 + side * j,
                           13 + (side + 1) * i,
                           3 + (side + 1) * j };
                    HBRUSH hbrFill = CreateSolidBrush(RGB(0, 200, 0));
                    FillRect(memoryDC, &rc, hbrFill);
                    DeleteObject(hbrFill);

                    rc = { 3 + side * i,
                           13 + side * j,
                           3 + (side + 1) * i,
                           13 + (side + 1) * j };
                    HBRUSH hbrRect = CreateSolidBrush(RGB(200, 0, 0));
                    SelectObject(memoryDC, hbrRect);
                    HPEN hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 200));
                    SelectObject(memoryDC, hPen);
                    Rectangle(memoryDC, rc.left, rc.top, rc.right, rc.bottom);
                    DeleteObject(hbrRect);
                    DeleteObject(hPen);

                    POINT vertices[4];
                    vertices[0].x = 13 + side * i;
                    vertices[0].y = 13 + side * j;
                    vertices[1].x = 13 + (side + 1) * i;
                    vertices[1].y = vertices[0].y;
                    vertices[2].x = vertices[1].x;
                    vertices[2].y = 13 + (side + 1) * j;
                    vertices[3].x = vertices[0].x;
                    vertices[3].y = vertices[2].y;
                    LOGBRUSH logBrush = { BS_NULL, 0, 0 };
                    HBRUSH hbrPoly = CreateSolidBrush(RGB(200, 100, 200));
                    hbrPoly = CreateBrushIndirect(&logBrush);
                    SelectObject(memoryDC, hbrPoly);
                    HPEN hpPoly = CreatePen(PS_SOLID, 0, RGB(200, 200, 0));
                    SelectObject(memoryDC, hpPoly);
                    Polygon(memoryDC, vertices, 4);
                    DeleteObject(hbrPoly);
                    DeleteObject(hpPoly);
                }
            }
            // rectangles for example
        }
        // This is only for DEBUG
        DeleteDC(clntDC);
    }
    if (memoryDC != NULL)
    {
        HDC copyDC = CreateCompatibleDC(memoryDC);
        HBITMAP copyBitMap = CreateCompatibleBitmap(memoryDC, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top);
        SelectObject(copyDC, copyBitMap);
        BitBlt(copyDC, 0, 0, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top, memoryDC, 0, 0, SRCCOPY);
        DeleteObject(memoryBitMap);
        DeleteDC(memoryDC);

        GetClientRect(mainWindow, &rcClient);
        if (rcClient.right - rcClient.left > rcMemory.right)
        {
            rcMemory.right = rcClient.right - rcClient.left;
        }
        if (rcClient.bottom - rcClient.top > rcMemory.bottom)
        {
            rcMemory.bottom = rcClient.bottom - rcClient.top;
        }

        memoryDC = CreateCompatibleDC(copyDC);
        memoryBitMap = CreateCompatibleBitmap(copyDC, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top);
        SelectObject(memoryDC, memoryBitMap);
        RECT rc = { 0, 0, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top };
        HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(memoryDC, &rc, hbr);
        DeleteObject(hbr); BitBlt(memoryDC, 0, 0, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top, copyDC, 0, 0, SRCCOPY);
        DeleteObject(copyBitMap);
        DeleteDC(copyDC);
    }
}

void ApplicationCore::On_WM_VSCROLL()
{
    // TODO: Add code
}

void ApplicationCore::On_WM_HSCROLL()
{
    // TODO: Add code
}

void ApplicationCore::On_WM_INITDIALOG()
{
    DrawColorWheel();
}

void ApplicationCore::CopyToClipboard()
{
    OpenClipboard(mainWindow);
    EmptyClipboard();
    HDC dc = GetDC(mainWindow);
    HDC memoryDC = CreateCompatibleDC(dc);
    HBITMAP bm = CreateCompatibleBitmap(dc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
    SelectObject(memoryDC, bm);
    BitBlt(memoryDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, dc, 0, 0, SRCCOPY);
    SetClipboardData(CF_BITMAP, bm);
    //DeleteObject(bm);
    DeleteDC(memoryDC);
    DeleteDC(dc);
    CloseClipboard();
}

// TODO: put the drawing methods in a new class, which is responsible for drawing

int ApplicationCore::DrawFrameRect()
{
    // The following section draws a simple square frame
    int success = -10;
    if (!rectangleShape.isDrawn)
    {
        HBRUSH hBrush = CreateSolidBrush(penColor);
        success = FrameRect(clientDC, &rectangleShape.rect, hBrush);

        if (!rectangleShape.isEditing())
        {
            rectangleShape.isDrawn = true;
        }
        DeleteObject(hBrush);
    }
    return success;
}

void ApplicationCore::DrawFreehand()
{
    if (freehandShape.isSizing)
    {
        HPEN hPen = CreatePen(PS_SOLID, penWidth, penColor);
        SelectObject(clientDC, hPen);
        if (freehandShape.previousPoint.x == freehandShape.currentPoint.x &&
            freehandShape.previousPoint.y == freehandShape.currentPoint.y)
        {
            SetPixelV(clientDC, freehandShape.previousPoint.x, freehandShape.previousPoint.y, RGB(200, 200, 0));
        }
        else
        {
            MoveToEx(clientDC, freehandShape.previousPoint.x, freehandShape.previousPoint.y, NULL);
            LineTo(clientDC, freehandShape.currentPoint.x, freehandShape.currentPoint.y);
        }
        freehandShape.previousPoint = freehandShape.currentPoint;
        DeleteObject(hPen);
    }
}

// TODO: put this method in an outer class, because it is used in several classes

int Distance(POINT p1, POINT p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void ApplicationCore::DrawRoute()
{
    if (!routeShape.isDrawn)
    {
        //HPEN hPen = CreatePen(PS_SOLID, penWidth, penColor);
        HPEN hPen = CreatePen(PS_SOLID, penWidth, RGB(rand() * 255 / RAND_MAX + 1, rand() * 255 / RAND_MAX + 1, rand() * 255 / RAND_MAX + 1));
        SelectObject(clientDC, hPen);
        if (routeShape.isEditing())
        {
            MoveToEx(clientDC, routeShape.anchor.x, routeShape.anchor.y, NULL);
            LineTo(clientDC, routeShape.endPoint.x, routeShape.endPoint.y);
        }
        else
        {
            POINT startPoint = routeShape.routePoints[0];
            POINT nextPoint;
            MoveToEx(clientDC, startPoint.x, startPoint.y, NULL);
            for (int i = 1; i < routeShape.routePoints.size(); ++i)
            {
                nextPoint = routeShape.routePoints[i];
                LineTo(clientDC, nextPoint.x, nextPoint.y);
            }
            routeShape.isDrawn = true;
        }
        DeleteObject(hPen);
    }

}

void ApplicationCore::DrawColorWheel()
{
    if (RGBDC == NULL)
    {
        HDC clientDC = GetDC(mainWindow);
        RGBDC = CreateCompatibleDC(clientDC);
        RECT clientRect;
        GetClientRect(mainWindow, &clientRect);
        RGBBitMap = CreateCompatibleBitmap(clientDC, 256 * 6, 256 + 10 + 256 * 2);
        SelectObject(RGBDC, RGBBitMap);
        int r, g, b;
        /*for (int i = 0; i < 256; ++i)
        {
            for (int j = 0; j < 256; ++j)
            {
                r = 255 + ((128 - 255) * j / 255);
                g = i + ((128 - i) * j / 255);
                b = 0 + ((128 - 0) * j / 255);
                SetPixelV(RGBDC, 256 * 0 - 0 + i, j, RGB(r, g, b));
                SetPixelV(RGBDC, 256 * 2 - 1 - i, j, RGB(g, r, b));
                SetPixelV(RGBDC, 256 * 2 - 0 + i, j, RGB(b, r, g));
                SetPixelV(RGBDC, 256 * 4 - 1 - i, j, RGB(b, g, r));
                SetPixelV(RGBDC, 256 * 4 - 0 + i, j, RGB(g, b, r));
                SetPixelV(RGBDC, 256 * 6 - 1 - i, j, RGB(r, b, g));
            }
        }*/
        POINT translate = { 0, 0 };
        POINT center = { 256 + translate.x, 256 + translate.y};
        int med;
        double slope;
        double atg;
        int endGrey = 128;
        for (int i = 0 + translate.x; i < 512 + translate.x; ++i)
        {
            for (int j = 0 + translate.y; j < 512 + translate.y; ++j)
            {
                POINT p = { i, j };
                int d = Distance(p, center);
                POINT c = { i - center.x, j - center.y };
                if (d < 256)
                {
                    d = 255 - d;
                    int max = 255;
                    int min = 0;
                    max += (endGrey - max) * d / 255;
                    //med += (128 - med) * d / 255;
                    min += (endGrey - min) * d / 255;
                    slope = c.y != 0 ? (double)c.x / c.y : DBL_MAX;
                    r = (c.y < 0 && slope <= 1.73205 && slope >= -1.73205) ? max : min; //r = min;
                    g = (c.x >= 0 && (slope <= -1.73205 || (slope >= 0 && c.y >= 0))) ? max : min; //g = min;
                    b = (c.x < 0 && (slope < 0 || slope >= 1.73205)) ? max : min; //b = min;
                    if (c.x > 0 && slope >= -1.73205 && c.y < 0)
                    {
                        atg = (atan((double)-c.y / c.x) - 3.141592653589 / 6) * 3.0 / 3.141592653589 * 255;
                        med = 255 - atg;
                        med += (endGrey - med) * d / 255;
                        g = med; //g = min;
                    }
                    if (c.x > 0 && (slope <= -1.73205 || slope >= 1.73205))
                    {
                        atg = (atan((double)-c.y / c.x) + 3.141592653589 / 6) * 3.0 / 3.141592653589 * 255;
                        med = atg;
                        med += (endGrey - med) * d / 255;
                        r = med; //r = min;
                    }
                    if (c.x >= 0 && slope < 1.73205 && c.y > 0)
                    {
                        atg = atan((double)c.x / c.y) * 3.0 / 3.141592653589 * 255;
                        med = 255 - atg;
                        med += (endGrey - med) * d / 255;
                        b = med; //b = min;
                    }
                    if (c.x < 0 && slope >= -1.73205 && c.y > 0)
                    {
                        atg = atan((double)-c.x / c.y) * 3.0 / 3.141592653589 * 255;
                        med = 255 - atg;
                        med += (endGrey - med) * d / 255;
                        g = med; //g = min;
                    }
                    if (c.x < 0 && (slope < -1.73205 || slope > 1.73205))
                    {
                        atg = (atan((double)-c.y / -c.x) + 3.141592653589 / 6) * 3.0 / 3.141592653589 * 255;
                        med = atg;
                        med += (endGrey - med) * d / 255;
                        r = med; //r = min;
                    }
                    if (c.x < 0 && slope <= 1.73205 && c.y < 0)
                    {
                        atg = atan((double)c.x / c.y) * 3.0 / 3.141592653589 * 255;
                        med = atg;
                        med += (endGrey - med) * d / 255;
                        b = med; //b = min;
                    }
                    SetPixelV(RGBDC, i, j, RGB(r, g, b));
                }
            }
        }
        DeleteDC(clientDC);
    }
}

void ApplicationCore::SelectShapeType(ShapeType selection)
{
    selectedShapeType = selection;
}

void ApplicationCore::StartDrawingShape()
{
    switch (selectedShapeType)
    {
        case RectangleShapeType:
            rectangleShape.mainWindow = mainWindow;
            rectangleShape.StartSizing(mouse.LD());
            break;
        case LineShapeType:
            break;
        case FreehandShapeType:
            freehandShape.mainWindow = mainWindow;
            freehandShape.StartSizing(mouse.LD());
            break;
        case RouteShapeType:
            routeShape.mainWindow = mainWindow;
            routeShape.StartSizing(mouse.LD());
            break;
        default:
            break;
    }
}

void ApplicationCore::DrawingShape()
{
    switch (selectedShapeType)
    {
    case RectangleShapeType:
        SizingRectangle();
        //rectangleShape.Sizing(mouse.CurrentPosition(), mouse.MotionVector());
        break;
    case LineShapeType:
        break;
    case FreehandShapeType:
        freehandShape.Sizing(mouse.PreviousPosition(), mouse.CurrentPosition());
        break;
    case RouteShapeType:
        routeShape.Sizing(routeShape.anchor, mouse.CurrentPosition());
        break;
    default:
        break;
    }
}

void ApplicationCore::StopDrawingShape()
{
    switch (selectedShapeType)
    {
    case RectangleShapeType:
        rectangleShape.StopSizing();
        break;
    case LineShapeType:
        break;
    case FreehandShapeType:
        freehandShape.StopSizing();
        break;
    case RouteShapeType:
        routeShape.StopSizing();
        break;
    default:
        break;
    }
}

void ApplicationCore::SizingRectangle()
{
    if (rectangleShape.isSizing && !rectangleShape.isMoving)
    {
        RECT rectToBeInvalidated = rectangleShape.rect;
        long l = rectangleShape.anchor.x;
        long r = mouse.CurrentPosition().x;
        long t = rectangleShape.anchor.y;
        long b = mouse.CurrentPosition().y;
        rectangleShape.rect.left = r < l ? r : l;
        rectangleShape.rect.right = l < r ? r : l;
        rectangleShape.rect.top = b < t ? b : t;
        rectangleShape.rect.bottom = t < b ? b : t;
        SetRect(&rectToBeInvalidated,
            rectangleShape.rect.left - abs(mouse.MotionVector().x) - 1,
            rectangleShape.rect.top - abs(mouse.MotionVector().y) - 1,
            rectangleShape.rect.right + abs(mouse.MotionVector().x) + 1,
            rectangleShape.rect.bottom + abs(mouse.MotionVector().y)) + 1;
        InvalidateRect(mainWindow, &rectToBeInvalidated, FALSE);
        // This is for DEBUG:
        //SetRect(&rectToBeInvalidated, rcClient.left, rcClient.top, 200, 200);
        //InvalidateRect(mainWindow, &rectToBeInvalidated, FALSE);
        //InvalidateRect(mainWindow, NULL, FALSE);
        // This is for DEBUG:
    }
}

void ApplicationCore::StartMovingRectangle()
{
    rectangleShape.isMoving = true;
}

void ApplicationCore::MoveRectangle()
{
    if (rectangleShape.isMoving)
    {
        RECT rectToBeInvalidated = rectangleShape.rect;
        OffsetRect(&rectangleShape.rect, mouse.MotionVector().x, mouse.MotionVector().y);
        rectangleShape.anchor.x += mouse.MotionVector().x;
        rectangleShape.anchor.y += mouse.MotionVector().y;
        SetRect(&rectToBeInvalidated,
            rectangleShape.rect.left - abs(mouse.MotionVector().x),
            rectangleShape.rect.top - abs(mouse.MotionVector().y),
            rectangleShape.rect.right + abs(mouse.MotionVector().x),
            rectangleShape.rect.bottom + abs(mouse.MotionVector().y));
        InvalidateRect(mainWindow, &rectToBeInvalidated, FALSE);
    }
}

void ApplicationCore::StopMovingRectangle()
{
    rectangleShape.isMoving = false;
}

void ApplicationCore::StartDraggingBackground()
{
    bgDragging = true;
}

// TODO: handle the background as a class, so it can have choosable color, and methods for moving, resizing, and so on

void ApplicationCore::DraggingBackground()
{
    if (bgDragging)
    {
        memoryBitMapTopLeft.x -= mouse.MotionVector().x;
        memoryBitMapTopLeft.y -= mouse.MotionVector().y;
        if (memoryBitMapTopLeft.x < 0)
        {
            memoryBitMapTopLeft.x = 0;
        }
        if (memoryBitMapTopLeft.y < 0)
        {
            memoryBitMapTopLeft.y = 0;
        }
        if (memoryBitMapTopLeft.x > (rcMemory.right - rcMemory.left) - (rcClient.right - rcClient.left))
        {
            memoryBitMapTopLeft.x = (rcMemory.right - rcMemory.left) - (rcClient.right - rcClient.left);
        }
        if (memoryBitMapTopLeft.y > (rcMemory.bottom - rcMemory.top) - (rcClient.bottom - rcClient.top))
        {
            memoryBitMapTopLeft.y = (rcMemory.bottom - rcMemory.top) - (rcClient.bottom - rcClient.top);
        }
        InvalidateRect(mainWindow, NULL, FALSE);
    }
}

void ApplicationCore::StopDraggingBackground()
{
    bgDragging = false;
}

void ApplicationCore::ShowColor()
{
    HDC dc = GetDC(mainWindow);
    COLORREF color = GetPixel(dc, mouse.X(), mouse.Y());
    if (RGBDC != NULL)
    {
        HBRUSH hbr = CreateSolidBrush(color);
        RECT rect = { rcClient.left + 20, rcClient.top + 20, rcClient.left + 100, rcClient.top + 100 };
        FillRect(RGBDC, &rect, hbr);
        DeleteObject(hbr);
        InvalidateRect(mainWindow, NULL, FALSE);
    }
    DeleteDC(dc);
}