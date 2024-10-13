#include "ApplicationCore.h"
#include <string>
#include <time.h>
#include <limits>

ApplicationCore::ApplicationCore()
{
    borderColor = RGB(255, 0, 0);
    penWidth = 20;
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
    // TODO: Move the below list before the winmain function in the application

    // TODO: Adding capability of drawing shapes with the following options
    //       - let the color of the interior be selectable
    //       - can select the color of the frame
    // later options:
    //      - can draw lines and curves
    //          - as many parameters as possible can be set during drawing:
    //            ex. pressing Ctrl-key enables rotating the shape during drawing,
    //            or pressing the right mouse button enables moving a shape before finishing drawing (so before releasing left button)
    //      - can select pens, brushes and colors
    //      - can create bitmaps and reuse them in drawing and painting
    //      - can edit bitmaps pixel by pixel
    //      - save the state of the drawing
    //      - save the drawing as a picture in different file formats
    //      - add undo/redo option
    //      - differentiate the individual shapes as objects
    //      - can copy-paste objects
    //      - TAB button is used to switch the editable parameter during drawing
    //          - ex. in case of a rectangle TAB switches between sizing and rotating
    //          - mouse cursor can indicate the actual editing mode
    //      - can move and resize the shapes (it requires that those are objects)
    //      - the shapes are parameterizable, angles can be set in polygons, number of vertices can be changed and so on
    //      - all-in-all object can be edited, copied, deleted and so on
    //      - edit can be performed on more objects at the same time
    //      - the final goal is to have a tool which can create patterns from objects (shapes) by giving a rule or ruleset for generating them
    //      - this pattern generation is performed by creating rules
    //      - rules can be ex. functions for setting the size, rotation or color of the shape
    //      - rules can be applied after combining them, as real functions
    //      - function can be applied for everything, ex. angle of lines in shapes, a color of a pixel in a bitmap and so on
    //      - perhaps later the whole ruleset can get a mathematical formulation
    //      
    //      - multithreading
    PAINTSTRUCT ps;
    clientDC = BeginPaint(mainWindow, &ps);
    // This is only for DEBUG
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            //RECT rc = { rcMemory.left + 250 + i * 50, rcMemory.top + j * 50, rcMemory.left + 275 + i * 50, rcMemory.top + 25 + j * 50 };
            //RECT rc = { 300, 50, 400, 150 };
            //HBRUSH hbr = CreateSolidBrush(RGB(0, 100, 100));
            //FrameRect(memoryDC, &rc, hbr);
            //DeleteObject(hbr);
        }
    }
    // This is only for DEBUG
    BitBlt(clientDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memoryDC, memoryBitMapTopLeft.x, memoryBitMapTopLeft.y, SRCCOPY);
    //DrawRGB();
    //BitBlt(clientDC, 10, 10, 256 * 6, 256 + 10 + 256 * 2, RGBDC, 0, 0, SRCCOPY);
    // success variable is used only for DEBUG
    int success = DrawFrameRect();
    DrawFreehand();
    DrawRoute();

    // DEBUG
    int debug = false;
    if (debug)
    {

        RECT rc = { rcClient.left, rcClient.top, 200, 200 };
        HBRUSH hbr = CreateSolidBrush(RGB(0, 20, 0));
        FillRect(clientDC, &rc, hbr);
        DeleteObject(hbr);
        TextOutW(clientDC, rcClient.left + 10, rcClient.top + 0, L"KeyState: ", 8);
        TextOutW(clientDC, rcClient.left + 10, rcClient.top + 30, L"RIGHT: ", 5);
        TextOutW(clientDC, rcClient.left + 10, rcClient.top + 60, L"LEFT: ", 4);
        TextOutW(clientDC, rcClient.left + 10, rcClient.top + 90, L"TOP: ", 3);
        TextOutW(clientDC, rcClient.left + 10, rcClient.top + 120, L"BOTTOM: ", 6);
        TextOutW(clientDC, rcClient.left + 10, rcClient.top + 150, L"lParam: ", 6);
        TextOutW(clientDC, rcClient.left + 10, rcClient.top + 180, L"success: ", 7);
        std::wstring ws;
        wchar_t wct[20];
        int sl;

        ws = std::to_wstring(keyState);
        wcscpy_s(wct, ws.c_str());
        sl = std::wcslen(wct);
        TextOutW(clientDC, rcClient.left + 100, rcClient.top + 0, wct, sl);

        ws = std::to_wstring(rectangleShape.rect.right);
        wcscpy_s(wct, ws.c_str());
        sl = std::wcslen(wct);
        TextOutW(clientDC, rcClient.left + 100, rcClient.top + 30, wct, sl);

        ws = std::to_wstring(rectangleShape.rect.left);
        wcscpy_s(wct, ws.c_str());
        sl = std::wcslen(wct);
        TextOutW(clientDC, rcClient.left + 100, rcClient.top + 60, wct, sl);

        ws = std::to_wstring(rectangleShape.rect.top);
        wcscpy_s(wct, ws.c_str());
        sl = std::wcslen(wct);
        TextOutW(clientDC, rcClient.left + 100, rcClient.top + 90, wct, sl);

        ws = std::to_wstring(rectangleShape.rect.bottom);
        wcscpy_s(wct, ws.c_str());
        sl = std::wcslen(wct);
        TextOutW(clientDC, rcClient.left + 100, rcClient.top + 120, wct, sl);

        ws = std::to_wstring(mouse.lastlParam);
        wcscpy_s(wct, ws.c_str());
        sl = std::wcslen(wct);
        TextOutW(clientDC, rcClient.left + 100, rcClient.top + 150, wct, sl);

        ws = std::to_wstring(success);
        wcscpy_s(wct, ws.c_str());
        sl = std::wcslen(wct);
        TextOutW(clientDC, rcClient.left + 100, rcClient.top + 180, wct, sl);
    }
    // DEBUG

    if (!(
        rectangleShape.isEditing() || routeShape.isEditing()
        ))
    {
        BitBlt(memoryDC, memoryBitMapTopLeft.x, memoryBitMapTopLeft.y, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, clientDC, 0, 0, SRCCOPY);
    }
    EndPaint(mainWindow, &ps);
}

void ApplicationCore::On_WM_SIZE()
{
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
        HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));
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

}

void ApplicationCore::On_WM_HSCROLL()
{

}

void ApplicationCore::On_WM_INITDIALOG()
{
    DrawRGB();
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

// TODO: put the drawing methods in a new class, which is responsibel for drawing

int ApplicationCore::DrawFrameRect()
{
    // The following section draws a simple red square frame
    int success = -10;
    if (!rectangleShape.isDrawn)
    {
        HBRUSH hBrush = CreateSolidBrush(borderColor);
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
        HPEN hPen = CreatePen(PS_SOLID, penWidth, borderColor);
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
        //HPEN hPen = CreatePen(PS_SOLID, penWidth, borderColor);
        HPEN hPen = CreatePen(PS_SOLID, penWidth, RGB(rand() * 255 / RAND_MAX + 1, rand() * 255 / RAND_MAX + 1, rand() * 255 / RAND_MAX + 1));
        SelectObject(clientDC, hPen);
        if (routeShape.isEditing())
        {
            //HBRUSH hBrush = CreateSolidBrush(borderColor);
            //FrameRect(clientDC, &routeShape.rect, hBrush);
            MoveToEx(clientDC, routeShape.anchor.x, routeShape.anchor.y, NULL);
            LineTo(clientDC, routeShape.endPoint.x, routeShape.endPoint.y);
            //DeleteObject(hBrush);
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

void ApplicationCore::DrawRGB()
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
        POINT center = { 256 + 5, 256 + 5 };
        int med;
        double slope;
        double atg;
        int endGrey = 255;
        for (int i = 5; i < 517; ++i)
        {
            for (int j = 261 - 256; j < 773 - 256; ++j)
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