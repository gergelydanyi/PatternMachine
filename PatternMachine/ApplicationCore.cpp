#include "ApplicationCore.h"

void ApplicationCore::On_WM_LBUTTONDOWN(LPARAM lParam)
{
    mouse.On_WM_LBUTTONDOWN(lParam);
    RECT rect;
    rect.left = mouse.LD().x;
    rect.top = mouse.LD().y;
    rect.right = rect.left + 100;
    rect.bottom = rect.top + 100;
    InvalidateRect(mainWindow, &rect, FALSE);
}

void ApplicationCore::On_WM_LBUTTONUP(LPARAM lParam)
{

}

void ApplicationCore::On_WM_PAINT()
{
    // TODO: Adding capability of drawing shapes with the following options
    //       - let the color of the interior be selectable
    //       - can select the color of the frame
    // later options:
    //      - can draw lines and curves
    //      - can select pens, brushes and colors
    //      - can create bitmaps and reuse them in drawing and painting
    //      - can edit bitmaps pixel by pixel
    //      - save the state of the drawing
    //      - save the drawing as a picture in different file formats
    //      - add undo/redo option
    //      - differentiate the individual shapes as objects
    //      - can copy-paste objects
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

    PAINTSTRUCT ps;
    clientDC = BeginPaint(mainWindow, &ps);
    BitBlt(clientDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memoryDC, 0, 0, SRCCOPY);
    DrawFrameRect();
    BitBlt(memoryDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, clientDC, 0, 0, SRCCOPY);
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
        BitBlt(memoryDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, clntDC, 0, 0, SRCCOPY);
        DeleteDC(clntDC);
    }
    if (memoryDC != NULL)
    {
        //BitBlt(clientDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, memoryDC, 0, 0, SRCCOPY);
        //clientDC = GetDC(mainWindow);
        HDC copyDC = CreateCompatibleDC(memoryDC);
        HBITMAP copyBitMap = CreateCompatibleBitmap(memoryDC, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top);
        SelectObject(copyDC, copyBitMap);
        BitBlt(copyDC, 0, 0, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top, memoryDC, 0, 0, SRCCOPY);
        DeleteObject(memoryBitMap);
        DeleteDC(memoryDC);
        memoryDC = CreateCompatibleDC(copyDC);
        memoryBitMap = CreateCompatibleBitmap(copyDC, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top);
        SelectObject(memoryDC, memoryBitMap);
        BitBlt(memoryDC, 0, 0, rcMemory.right - rcMemory.left, rcMemory.bottom - rcMemory.top, copyDC, 0, 0, SRCCOPY);
        DeleteObject(copyBitMap);
        DeleteDC(copyDC);
        GetClientRect(mainWindow, &rcClient);
        if (rcClient.right - rcClient.left > rcMemory.right)
        {
            rcMemory.right = rcClient.right - rcClient.left;
        }
        if (rcClient.bottom - rcClient.top > rcMemory.bottom)
        {
            rcMemory.bottom = rcClient.bottom - rcClient.top;
        }
    }
}

void ApplicationCore::DrawFrameRect()
{
    // The following section draws a simple red square frame
    RECT rect;
    rect.left = mouse.LD().x;
    rect.top = mouse.LD().y;
    rect.right = rect.left + 100;
    rect.bottom = rect.top + 100;
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
    FrameRect(clientDC, &rect, hBrush);
    DeleteObject(hBrush);
}

