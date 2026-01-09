#include "ApplicationCore.h"
#include <string>
#include <time.h>
#include <limits>

namespace PatternMachine
{

ApplicationCore::ApplicationCore()
{
    penColor = RGB(0, 255, 0);
    brushColor = RGB(255, 0, 0);
    penWidth = 2;
    penStyle = PS_SOLID;
    activePen = CreatePen(penStyle, penWidth, penColor);
    bgColor = RGB(255, 0, 255);
}

void ApplicationCore::On_WM_LBUTTONDOWN(LPARAM lParam)
{
    mouse.On_WM_LBUTTONDOWN(lParam);
}

void ApplicationCore::On_WM_LBUTTONUP(LPARAM lParam)
{
    mouse.On_WM_LBUTTONUP(lParam);
}

void ApplicationCore::On_WM_RBUTTONDOWN(LPARAM lParam)
{
    mouse.On_WM_RBUTTONDOWN(lParam);
}

void ApplicationCore::On_WM_RBUTTONUP(LPARAM lParam)
{
    mouse.On_WM_RBUTTONUP(lParam);
}

void ApplicationCore::On_WM_MOUSEMOVE(LPARAM lParam)
{
    mouse.On_WM_MOUSEMOVE(lParam);
    ShowColor();
}

void ApplicationCore::On_WM_PAINT()
{
    PAINTSTRUCT ps;
    clientDC = BeginPaint(mainWindow, &ps);

    // TODO: calculate the clipping area accurately, based on the height of the rebar; store the handle of the rebar
    HRGN hRgn = CreateRectRgn(0, 40, rcClient.right, rcClient.bottom);
    //SelectClipRgn(clientDC, hRgn);
    // memoryDCfinal and memoryBitMapfinal added to prevent flickering while painting
    // Formerly we painted directly to the screen after copying the previous bitmap stored in memory,
    // now we paint into the memory (the Draw methods use memoryDCfinal), and after this the final picture is copied to the screen
    BitBlt(memoryDCfinal, 0, 0, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top, memoryDCstorage, 0, 0, SRCCOPY);
    GdiTransparentBlt(memoryDCfinal, memoryBitMapTopLeft.x, memoryBitMapTopLeft.y, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memoryDCdrawing, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, RGB(1, 1, 1));
    // reset the DC to transparent background color:
    RECT rc = { 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
    HBRUSH hbr = CreateSolidBrush(RGB(1, 1, 1));
    FillRect(memoryDCdrawing, &rc, hbr);
    DeleteObject(hbr);
    // ---------------------------------------------
    BitBlt(clientDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memoryDCfinal, memoryBitMapTopLeft.x, memoryBitMapTopLeft.y, SRCCOPY);

    EndPaint(mainWindow, &ps);
    DeleteObject(hRgn);
}

void ApplicationCore::On_WM_SIZE()
{
    // DONE: remove the unnecessary parts used for debugging
    // TODO: initialize memoryDCstorage and memoryDCfinal before this method, ex. during the creation of the window
    // TODO: use a separate method to set the size of the bitmap, ex. in a separate class, where the background should take place
    if (memoryDCstorage == NULL)
    {
        GetClientRect(mainWindow, &rcClient);
        rcMemory = rcClient;
        OffsetRect(&rcMemory, rcMemory.left - rcMemory.right, rcMemory.top - rcMemory.bottom);
        HDC clntDC = GetDC(mainWindow);
        memoryDCstorage = CreateCompatibleDC(clntDC);
        memoryDCdrawing = CreateCompatibleDC(clntDC);
        memoryDCfinal = CreateCompatibleDC(clntDC);
        memoryBitMapstorage = CreateCompatibleBitmap(clntDC, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
        memoryBitMapdrawing = CreateCompatibleBitmap(clntDC, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
        memoryBitMapfinal = CreateCompatibleBitmap(clntDC, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
        SelectObject(memoryDCstorage, memoryBitMapstorage);
        SelectObject(memoryDCdrawing, memoryBitMapdrawing);
        SelectObject(memoryDCfinal, memoryBitMapfinal);
        BitBlt(memoryDCstorage, memoryBitMapTopLeft.x, memoryBitMapTopLeft.y, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, clntDC, 0, 0, SRCCOPY);
        DeleteDC(clntDC);
        RECT rc = { 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
        HBRUSH hbr = CreateSolidBrush(RGB(1, 1, 1));
        FillRect(memoryDCdrawing, &rc, hbr);
        DeleteObject(hbr);

    }
    if (memoryDCstorage != NULL)
    {
        // TODO: consider whether it is necessary to delete the DC-s
        HDC copyDC = CreateCompatibleDC(memoryDCstorage);
        HBITMAP copyBitMap = CreateCompatibleBitmap(memoryDCstorage, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top);
        SelectObject(copyDC, copyBitMap);
        BitBlt(copyDC, 0, 0, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top, memoryDCstorage, 0, 0, SRCCOPY);
        DeleteObject(memoryBitMapstorage);
        DeleteDC(memoryDCstorage);
        DeleteObject(memoryBitMapdrawing);
        DeleteDC(memoryDCdrawing);
        DeleteObject(memoryBitMapfinal);
        DeleteDC(memoryDCfinal);

        GetClientRect(mainWindow, &rcClient);
        if (rcClient.right - rcClient.left > rcMemory.right)
        {
            rcMemory.right = rcClient.right - rcClient.left;
        }
        if (rcClient.bottom - rcClient.top > rcMemory.bottom)
        {
            rcMemory.bottom = rcClient.bottom - rcClient.top;
        }

        memoryDCstorage = CreateCompatibleDC(copyDC);
        memoryBitMapstorage = CreateCompatibleBitmap(copyDC, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top);
        memoryDCdrawing = CreateCompatibleDC(copyDC);
        memoryBitMapdrawing = CreateCompatibleBitmap(copyDC, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
        memoryDCfinal = CreateCompatibleDC(copyDC);
        memoryBitMapfinal = CreateCompatibleBitmap(copyDC, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top);
        SelectObject(memoryDCstorage, memoryBitMapstorage);
        SelectObject(memoryDCfinal, memoryBitMapfinal);
        SelectObject(memoryDCdrawing, memoryBitMapdrawing);
        RECT rc = { 0, 0, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top };
        HBRUSH hbr = CreateSolidBrush(bgColor);
        FillRect(memoryDCstorage, &rc, hbr);
        DeleteObject(hbr);
        rc = { 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
        hbr = CreateSolidBrush(RGB(1, 1, 1));
        FillRect(memoryDCdrawing, &rc, hbr);
        DeleteObject(hbr);
        BitBlt(memoryDCstorage, 0, 0, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top, copyDC, 0, 0, SRCCOPY);
        DeleteObject(copyBitMap);
        DeleteDC(copyDC);
        SetActivePen();
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

void ApplicationCore::DeleteSelection()
{
    pCanvas->DeleteSelection();
}

void ApplicationCore::ChangeCanvasBehaviour(CanvasBehaviour behaviour)
{
    pCanvas->ChangeBehaviour(behaviour);
}

// DONE: put the drawing methods in a new class, which is responsible for drawing

// TODO: put this method in an outer class, because it is used in several classes

int Distance(POINT p1, POINT p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
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
    pCanvas->selectedShapeType = selection;
    //pCanvas->SetActiveShapeType(selection);
}

void ApplicationCore::SetActivePen()
{
    if (memoryDCdrawing != NULL)
    {
        if (activePen != NULL)
        {
            DeleteObject(activePen);
        }
        activePen = CreatePen(penStyle, penWidth, penColor);
        SelectObject(memoryDCdrawing, activePen);
    }
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

}