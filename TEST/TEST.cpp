// TEST.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "TEST.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;          // handle to device context (DC)  
    PAINTSTRUCT ps;   // paint data for Begin/EndPaint  
    POINT ptClientUL; // client area upper left corner  
    POINT ptClientLR; // client area lower right corner  
    static HDC hdcCompat; // handle to DC for bitmap  
    static POINT pt;      // x- and y-coordinates of cursor  
    static RECT rcTarget; // rect to receive filled shape  
    static RECT rcClient; // client area rectangle  
    static BOOL fSizeEllipse; // TRUE if ellipse is sized  
    static BOOL fDrawEllipse;   // TRUE if ellipse is drawn  
    static BOOL fDrawRectangle; // TRUE if rectangle is drawn  
    static BOOL fSizeRectangle; // TRUE if rectangle is sized  
    static BOOL fSizeRoundRect; // TRUE if rounded rect is sized  
    static BOOL fDrawRoundRect; // TRUE if rounded rect is drawn  
    static int nEllipseWidth;   // width for round corners  
    static int nEllipseHeight;  // height for round corners

    switch (uMsg)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
                // Set the appropriate flag to indicate which  
                // filled shape the user is drawing.  

            case IDM_ELLIPSE:
                fSizeEllipse = TRUE;
                break;

            case IDM_RECTANGLE:
                fSizeRectangle = TRUE;
                break;

            case IDM_ROUNDRECT:
                fSizeRoundRect = TRUE;
                break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
        nEllipseWidth = 20;
        nEllipseHeight = 20;

        return 0;

    case WM_PAINT:
        {
        BeginPaint(hWnd, &ps);

        // Because the default brush is white, select  
        // a different brush into the device context  
        // to demonstrate the painting of filled shapes.  

        SelectObject(ps.hdc, GetStockObject(GRAY_BRUSH));

        // If one of the filled shape "draw" flags is TRUE,  
        // draw the corresponding shape.  

        if (fDrawEllipse)
        {
            Ellipse(ps.hdc, rcTarget.left, rcTarget.top,
                rcTarget.right, rcTarget.bottom);
            fDrawEllipse = FALSE;
            rcTarget.left = rcTarget.right = 0;
            rcTarget.top = rcTarget.bottom = 0;
        }

        if (fDrawRectangle)
        {
            Rectangle(ps.hdc, rcTarget.left, rcTarget.top,
                rcTarget.right, rcTarget.bottom);
            fDrawRectangle = FALSE;
            rcTarget.left = rcTarget.right = 0;
            rcTarget.top = rcTarget.bottom = 0;
        }

        if (fDrawRoundRect)
        {
            RoundRect(ps.hdc, rcTarget.left, rcTarget.top,
                rcTarget.right, rcTarget.bottom,
                nEllipseWidth, nEllipseHeight);
            fDrawRectangle = FALSE;
            rcTarget.left = rcTarget.right = 0;
            rcTarget.top = rcTarget.bottom = 0;
        }

        EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE:

        // Convert the client coordinates of the client area  
        // rectangle to screen coordinates and save them in a  
        // rectangle. The rectangle is passed to the ClipCursor  
        // function during WM_LBUTTONDOWN processing.  

        GetClientRect(hWnd, &rcClient);
        ptClientUL.x = rcClient.left;
        ptClientUL.y = rcClient.top;
        ptClientLR.x = rcClient.right;
        ptClientLR.y = rcClient.bottom;
        ClientToScreen(hWnd, &ptClientUL);
        ClientToScreen(hWnd, &ptClientLR);
        SetRect(&rcClient, ptClientUL.x, ptClientUL.y,
            ptClientLR.x, ptClientLR.y);
        return 0;
    case WM_LBUTTONDOWN:

        // Restrict the cursor to the client area.  
        // This ensures that the window receives a matching  
        // WM_LBUTTONUP message.  

        ClipCursor(&rcClient);

        // Save the coordinates of the cursor.  

        pt.x = (LONG)LOWORD(lParam);
        pt.y = (LONG)HIWORD(lParam);

        // If the user chooses one of the filled shapes,  
        // set the appropriate flag to indicate that the  
        // shape is being sized.  

        if (fDrawEllipse)
            fSizeEllipse = TRUE;

        return 0;

    case WM_MOUSEMOVE:

        // If one of the "size" flags is set, draw  
        // the target rectangle as the user drags  
        // the mouse.  

        if ((wParam && MK_LBUTTON)
            && (fSizeEllipse || fSizeRectangle
                || fSizeRoundRect))
        {

            // Set the mixing mode so that the pen color is the  
            // inverse of the background color. The previous  
            // rectangle can then be erased by drawing  
            // another rectangle on top of it.  

            hdc = GetDC(hWnd);
            SetROP2(hdc, R2_NOTXORPEN);

            // If a previous target rectangle exists, erase  
            // it by drawing another rectangle on top.  

            if (!IsRectEmpty(&rcTarget))
            {
                Rectangle(hdc, rcTarget.left, rcTarget.top,
                    rcTarget.right, rcTarget.bottom);
            }

            // Save the coordinates of the target rectangle.  
            // Avoid invalid rectangles by ensuring that the  
            // value of the left coordinate is greater than  
            // that of the right, and that the value of the  
            // bottom coordinate is greater than that of  
            // the top.  

            if ((pt.x < (LONG)LOWORD(lParam)) &&
                (pt.y > (LONG) HIWORD(lParam)))
            {
                SetRect(&rcTarget, pt.x, HIWORD(lParam),
                    LOWORD(lParam), pt.y);
            }

            else if ((pt.x > (LONG)LOWORD(lParam)) &&
                (pt.y > (LONG)HIWORD(lParam)))
            {
                SetRect(&rcTarget, LOWORD(lParam),
                    HIWORD(lParam), pt.x, pt.y);
            }

            else if ((pt.x > (LONG)LOWORD(lParam)) &&
                (pt.y < (LONG)HIWORD(lParam)))
            {
                SetRect(&rcTarget, LOWORD(lParam), pt.y,
                    pt.x, HIWORD(lParam));
            }
            else
            {
                SetRect(&rcTarget, pt.x, pt.y, LOWORD(lParam),
                    HIWORD(lParam));
            }

            // Draw the new target rectangle.  

            Rectangle(hdc, rcTarget.left, rcTarget.top,
                rcTarget.right, rcTarget.bottom);
            ReleaseDC(hWnd, hdc);
        }
        return 0;

    case WM_LBUTTONUP:

        // If one of the "size" flags is TRUE, reset it to FALSE,  
        // and then set the corresponding "draw" flag. Invalidate  
        // the appropriate rectangle and issue a WM_PAINT message.  

        if (fSizeEllipse)
        {
            fSizeEllipse = FALSE;
            fDrawEllipse = TRUE;
        }

        if (fSizeRectangle)
        {
            fSizeRectangle = FALSE;
            fDrawRectangle = TRUE;
        }

        if (fSizeRoundRect)
        {
            fSizeRoundRect = FALSE;
            fDrawRoundRect = TRUE;
        }

        if (fDrawEllipse || fDrawRectangle || fDrawRoundRect)
        {
            InvalidateRect(hWnd, &rcTarget, TRUE);
            UpdateWindow(hWnd);
        }

        // Release the cursor.  

        ClipCursor((LPRECT)NULL);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
