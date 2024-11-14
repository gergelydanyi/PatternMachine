// TODOS:

// DONE: Adding capability of drawing shapes with the following options
//      + let the color of the interior be selectable
//      + can select the color of the frame
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
//      - multithreading (ex. for long drawing operations)
//
//      - Desired working of the colorpicker:
//          - When moving the mouse over the color wheel, while left button is pressed,
//            a small circle appears around the peek of the mouse cursor and three lines
//            are pointing to the red, green and blue section of the wheel. The circle and
//            the lines can be inverted, to be visible on every section of the wheel.
//            On the end of the line the red, green and blue values are indicated ex. in a textbox.
//          - When scrolling with the mouse scroll button on the wheel, the brightness of
//            the color in the middle of the wheel is changed step by step from 0 (black) to 255(white).
//            If the SHIFT key is pressed meanwhile, the value change is increased (ex. change 10 value
//            in one step).
//          - If both the SHIFT key and Left mouse button is pressed, the vertical moving of
//            the mouse can change the value of brigthness
//          - Another option instead of the three lines pointing in the RGB values is one slider
//            from the center of the wheel to the pure color to the side of the wheel.
//          - Additionally the selected color can be shown in another triangle. The color of the vertices
//            of the triangle are the selected color, black and white, which are mixed along the sides
//            and in the inside of the triangle. Practically the triangle is a section of the color cone.
//          - If the Left mouse button is pressed, pressing the SHIFT key locks the slider
//            in the color wheel, and the moving the mouse can change the brightness and
//            saturation of the color in the triangle.


// PatternMachine.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "PatternMachine.h"
#include "ApplicationCore.h"
#include "Canvas.h"
#include <time.h>
#include <string>

// Including is necessary for using common control elements, toolbox and imagelist here.
// (Why is it not included in framework.h? Maybe because it is not part of SDK)
// TODO: consider placing toolbar creation in a separate unit
#include <CommCtrl.h>
#include "shlwapi.h"

// Enable Visual Style
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#pragma endregion

#define MAX_LOADSTRING 100

using namespace PatternMachine;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HIMAGELIST g_hImageList = NULL;
HIMAGELIST g_hImageList2 = NULL;
ApplicationCore appCore; // TODO: do not use a global variable for this

// Logging into textfile:

HANDLE hLogFile;


// Forward declarations of functions included in this code module:
ATOM                RegisterWindowClasses(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, ApplicationCore);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    CanvasChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PenSettingsDialogProcess(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    BrushSettingsDialogProcess(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK ColorPickerDialogProcess(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

HANDLE OpenLog()
{
    return CreateFile(L"LOG.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

void Log(UINT msg, WPARAM wParam, LPARAM lParam)
{}

void Log2(int pointer)
{
    wchar_t wct[120];
    std::wstring ws = std::to_wstring(pointer);

}

void CloseLog()
{
    CloseHandle(hLogFile);
}

#define PACKVERSION(major,minor) MAKELONG(minor,major)

DWORD GetVersion(LPCTSTR lpszDllName)
{
    HINSTANCE hinstDll;
    DWORD dwVersion = 0;

    // For security purposes, LoadLibrary should be provided with a fully qualified 
    // path to the DLL. The lpszDllName variable should be tested to ensure that it 
    // is a fully qualified path before it is used. 
    hinstDll = LoadLibrary(lpszDllName);

    if (hinstDll)
    {
        DLLGETVERSIONPROC pDllGetVersion;
        pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");

        // Because some DLLs might not implement this function, you must test for 
        // it explicitly. Depending on the particular DLL, the lack of a DllGetVersion 
        // function can be a useful indicator of the version. 

        if (pDllGetVersion)
        {
            DLLVERSIONINFO dvi;
            HRESULT hr;

            ZeroMemory(&dvi, sizeof(dvi));
            dvi.cbSize = sizeof(dvi);

            hr = (*pDllGetVersion)(&dvi);

            if (SUCCEEDED(hr))
            {
                dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
            }
        }
        FreeLibrary(hinstDll);
    }
    return dwVersion;
}

HWND CreateSimpleToolBar(HWND hWndParent)
{
    const int ImageListID = 0;
    const int numButtons = 3;
    const int bitmapSize = 16;

    const DWORD buttonStyles = BTNS_AUTOSIZE;

    HWND hWndToolbar;
    hWndToolbar = CreateWindowExW(0L, TOOLBARCLASSNAMEW, NULL, WS_CHILD | TBSTYLE_WRAPABLE | CCS_NOPARENTALIGN, 0, 0, 0, 0, hWndParent, (HMENU)(int)101, hInst, NULL);
    if (hWndToolbar == NULL)
    {
        return NULL;
    }
    g_hImageList = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR16 | ILC_MASK, numButtons, 0);
    SendMessage(hWndToolbar, TB_SETIMAGELIST, (WPARAM)ImageListID, (LPARAM)g_hImageList);
    SendMessage(hWndToolbar, TB_LOADIMAGES, (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);
/*    TBBUTTON tbButtons[numButtons] =
    {
        { MAKELONG(STD_FILENEW,  ImageListID), ID_FILE_NEW,  TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"New" },
        { MAKELONG(STD_FILEOPEN, ImageListID), ID_FILE_OPEN, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"Open"},
        { MAKELONG(STD_FILESAVE, ImageListID), ID_FILE_SAVE, 0,               buttonStyles, {0}, 0, (INT_PTR)L"Save"}
    };*/
    TBBUTTON tbButtons[numButtons] =
    {
        { MAKELONG(STD_FILENEW,  ImageListID), ID_FILE_NEW,  TBSTATE_ENABLED, buttonStyles, {0}, 0, 0},
        { MAKELONG(STD_FILEOPEN, ImageListID), ID_FILE_OPEN, TBSTATE_ENABLED, buttonStyles, {0}, 0, 0},
        { MAKELONG(STD_FILESAVE, ImageListID), ID_FILE_SAVE, TBSTATE_ENABLED, buttonStyles, {0}, 0, 0}
    };
    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);
    SendMessage(hWndToolbar, TB_SETANCHORHIGHLIGHT, (WPARAM)TRUE, 0);
    SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
    ShowWindow(hWndToolbar, TRUE);
    return hWndToolbar;
    //UpdateWindow(hWndToolbar);
}

HWND CreateShapesToolBar(HWND hWndParent)
{
    const int ImageListID = 0;
    const int numButtons = 3;
    const int bitmapSize = 16;

    const DWORD buttonStyles = BTNS_AUTOSIZE;
    HWND hWndToolbar = CreateWindowExW(
        WS_EX_TOOLWINDOW,
        TOOLBARCLASSNAMEW,
        L"Shapes",
        WS_CHILD | WS_OVERLAPPED | WS_VISIBLE /* | TBSTYLE_WRAPABLE */ | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER,
        0, 0, 0, 0,
        hWndParent,
        NULL/*(HMENU)(int)102*/,
        hInst,
        NULL);
    if (hWndToolbar == NULL)
    {
        return NULL;
    }
    g_hImageList = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR24 | ILC_MASK, numButtons, 0);

    HDC clientDC = GetDC(hWndParent);
    HDC memoryDC = CreateCompatibleDC(clientDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(clientDC, bitmapSize * numButtons, bitmapSize);
    HGDIOBJ hOrigBitmap = SelectObject(memoryDC, hBitmap);
    RECT rect = { 0, 0, 48, 16 };
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0) | 0x00000001);
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(memoryDC, hBrush);
    SelectObject(memoryDC, hPen);
    FillRect(memoryDC, &rect, hBrush);
    ::Rectangle(memoryDC, 2, 3, 14, 13);
    MoveToEx(memoryDC, 16 + 2, 3, NULL);
    LineTo(memoryDC, 16 + 4, 5);
    LineTo(memoryDC, 16 + 8, 7);
    Ellipse(memoryDC, 32 + 2, 4, 32 + 14, 12);
    SelectObject(memoryDC, hOrigBitmap);
    if (ImageList_AddMasked(g_hImageList, hBitmap, (COLORREF)0X00000001) == -1)
    {
        return NULL;
    }

    SendMessage(hWndToolbar, TB_SETIMAGELIST, (WPARAM)ImageListID, (LPARAM)g_hImageList);
    TBBUTTON tbButtons[numButtons] =
    {
        { MAKELONG(0, ImageListID), ID_SHAPE_RECTANGLE,  TBSTATE_ENABLED, buttonStyles, {0}, 0, 0},
        { MAKELONG(1, ImageListID), ID_SHAPE_FREEHAND, TBSTATE_ENABLED, buttonStyles, {0}, 0, 0},
        { MAKELONG(2, ImageListID), ID_SHAPE_ROUTE, TBSTATE_ENABLED, buttonStyles, {0}, 0, 0}
    };
    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);
    //ShowWindow(hWndToolbar, TRUE);

    DeleteObject(hBrush);
    DeleteObject(hBitmap);
    ReleaseDC(hWndParent, memoryDC);
    ReleaseDC(hWndParent, clientDC);

    return hWndToolbar;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    srand(time(0)); 
    hLogFile = OpenLog();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PATTERNMACHINE, szWindowClass, MAX_LOADSTRING);
    if (!RegisterWindowClasses(hInstance))
    {
        return FALSE;
    }

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow, appCore))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PATTERNMACHINE));

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
    CloseLog();
    return (int) msg.wParam;
}
ATOM RegisterWindowClasses(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PATTERNMACHINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(150, 150, 150));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PATTERNMACHINE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    if (!RegisterClassExW(&wcex))
{
        return FALSE;
    }
    
    // Canvas window class

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = CanvasChildWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(220, 220, 220));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PATTERNMACHINE);
    wcex.lpszClassName = L"Canvas";
    wcex.hIconSm = 0;

    if (!RegisterClassExW(&wcex))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, ApplicationCore appCore)
{
   hInst = hInstance; // Store instance handle in our global variable
   HMENU hMenu = LoadMenuW(hInstance, MAKEINTRESOURCEW(IDC_PATTERNMACHINE));
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      100, 100, 800, 600, nullptr, hMenu, hInstance, &appCore);
   appCore.mainWindow = hWnd;
   appCore.menuBar = hMenu;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// TODO: put the below code into a separate unit (ex. utility)

LPWORD lpwAlign(LPWORD &lpIn)
{
    ULONG ul;
    ULONGLONG ull;

    ul = (ULONG)lpIn;
    ul++;
    ul >>= 1;
    ul <<= 1;

    ull = (ULONGLONG)lpIn;
    ull++;
    ull >>= 1;
    ull <<= 1;

    return (LPWORD)ull;
}

HWND CreateComboBox(HWND hWndParent)
{
    int xpos = 100;            // Horizontal position of the window.
    int ypos = 100;            // Vertical position of the window.
    int nwidth = 200;          // Width of the window
    int nheight = 200;         // Height of the window

    HWND hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        xpos, ypos, nwidth, nheight, hWndParent, NULL, hInst,
        NULL);
    return hWndComboBox;
}

HWND CreateReBar(HWND hWndOwner, HWND hWndToolbar, HWND hWndComboBox1, HWND hWndComboBox2)
{
    int numButtons = 3;
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);

    HWND hWndRebar = CreateWindowExW(
        WS_EX_TOOLWINDOW,
        REBARCLASSNAME,
        NULL,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | RBS_VARHEIGHT | CCS_NODIVIDER | RBS_BANDBORDERS,
        0, 0, 0, 0,
        hWndOwner,
        NULL/*(HMENU)(int)101*/,
        hInst,
        NULL);
    if (!hWndRebar)
    {
        return NULL;
    }
    REBARINFO rb;
    rb.cbSize = sizeof(REBARINFO);
    rb.fMask = 0;
    rb.himl = 0;
    LRESULT lres = SendMessageW(hWndRebar, RB_SETBARINFO, (WPARAM)0, (LPARAM) & rb);
    //MessageBoxW(hWndOwner, std::to_wstring(lres).c_str(), NULL, NULL);
    wchar_t wct[5];
    std::wstring ws;
    REBARBANDINFO rbBand;
    rbBand.cbSize = sizeof(REBARBANDINFO);
    
    // toolbar must be created here, otherwise it is not aligned in the rebar
    hWndToolbar = CreateShapesToolBar(hWndRebar);
    if (hWndToolbar != NULL)
    {
        rbBand.fMask = RBBIM_STYLE | RBBIM_TEXT | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE;
        rbBand.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;
        DWORD dwBtnSize = (DWORD)SendMessageW(hWndToolbar, TB_GETBUTTONSIZE, 0, 0);
        ws = L"";
        wcscpy_s(wct, ws.c_str());
        rbBand.lpText = wct;
        rbBand.hwndChild = hWndToolbar;
        rbBand.cyChild = LOWORD(dwBtnSize);
        rbBand.cxMinChild = numButtons* HIWORD(dwBtnSize);
        rbBand.cyMinChild = LOWORD(dwBtnSize);
        //rbBand.cyMaxChild = LOWORD(dwBtnSize);
        //rbBand.cyIntegral = 50;
        rbBand.cx = 0;
        SendMessageW(hWndRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
        SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
    } 
    if (hWndComboBox1 != NULL)
    {
        RECT rc;
        GetWindowRect(hWndComboBox1, &rc);
        ws = L"";
        wcscpy_s(wct, ws.c_str());
        rbBand.fMask = RBBIM_STYLE | RBBIM_TEXT | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE;
        rbBand.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;
        rbBand.lpText = wct;
        rbBand.hwndChild = hWndComboBox1;
        rbBand.cxMinChild = 0;
        rbBand.cyMaxChild = rc.bottom - rc.top;
        rbBand.cx = 100;
        SendMessageW(hWndRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
    }

    if (hWndComboBox2 != NULL)
    {
        RECT rc;
        GetWindowRect(hWndComboBox2, &rc);
        ws = L"";
        wcscpy_s(wct, ws.c_str());
        rbBand.fMask = RBBIM_STYLE | RBBIM_TEXT | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE;
        rbBand.fStyle = /*RBBS_BREAK | */RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;
        rbBand.lpText = wct;
        rbBand.hwndChild = hWndComboBox2;
        rbBand.cxMinChild = 0;
        rbBand.cyMaxChild = rc.bottom - rc.top;
        rbBand.cx = 100;
        SendMessageW(hWndRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
    }

    return hWndRebar;
}

INT_PTR CreateColorPickerDialog(HWND hWndParent, ApplicationCore * pAppCore)
    // TODO: replace this code with a resource definition
    // TODO: display only the color wheel without the black background around it
    // TODO: move the OK button on the left side
    // TODO: add Cancel button
    // TODO: add an indicator rectangle which displays the selected color
    // TODO: add RGB and HSL trackbar controls
{
    HGLOBAL hgbl;
    LPDLGTEMPLATE lpdt;
    LPDLGITEMTEMPLATE lpdit;
    LPWORD lpw;
    LPWSTR lpwsz;
    LRESULT ret;
    int nchar;

    hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
    if (!hgbl)
        return -1;

    lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);

    // Define a dialog box.

    lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
    lpdt->cdit = 1;         // Number of controls
    lpdt->x = 10;  lpdt->y = 10;
    lpdt->cx = 300; lpdt->cy = 300;

    lpw = (LPWORD)(lpdt + 1);
    *lpw++ = 0;             // No menu
    *lpw++ = 0;             // Predefined dialog box class (by default)

    lpwsz = (LPWSTR)lpw;
    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "My Dialog", -1, lpwsz, 50);
    lpw += nchar;

    //-----------------------
    // Define an OK button.
    //-----------------------
    lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE)lpw;
    lpdit->x = 5; lpdit->y = 280;
    lpdit->cx = 40; lpdit->cy = 15;
    lpdit->id = IDOK;       // OK button identifier
    lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;
    lpw = (LPWORD)(lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0080;        // Button class
    lpwsz = (LPWSTR)lpw;
    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "OK", -1, lpwsz, 50);
    lpw += nchar;
    *lpw++ = 0;             // No creation data
    GlobalUnlock(hgbl);
    INT_PTR color = DialogBoxIndirectParamW(hInst, (LPDLGTEMPLATE)hgbl, hWndParent, (DLGPROC)ColorPickerDialogProcess, (LPARAM)pAppCore);
    GlobalFree(hgbl);
    return (INT_PTR)color;
}

LRESULT CALLBACK CanvasChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Canvas* pCanvas;
    if (message == WM_CREATE)
    {
        CREATESTRUCTW* pcs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pCanvas = reinterpret_cast<Canvas*>(pcs->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)pCanvas);
    }
    else
    {
        LONG_PTR lp = GetWindowLongPtrW(hWnd, GWLP_USERDATA);
        pCanvas = reinterpret_cast<Canvas*>(lp);
    }

    switch (message)
    {
    case WM_PAINT:
    {
        pCanvas->On_WM_PAINT(wParam, lParam);
    }
        break;
    case WM_LBUTTONDOWN:
    {
        pCanvas->On_WM_LBUTTONDOWN(wParam, lParam);
    }
        break;
    case WM_LBUTTONUP:
    {
        pCanvas->On_WM_LBUTTONUP(wParam, lParam);
    }
        break;
    case WM_MOUSEMOVE:
    {
        pCanvas->On_WM_MOUSEMOVE(wParam, lParam);
    }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ApplicationCore* pAppCore;
    if (message == WM_CREATE)
    {
        CREATESTRUCTW* pcs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pAppCore = reinterpret_cast<ApplicationCore*>(pcs->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)pAppCore);
        // TODO: put toolbar creation in a separate method
        HWND hWndComboBox1 = CreateComboBox(hWnd);
        HWND hWndComboBox2 = CreateComboBox(hWnd);
        HWND hWndReBar = CreateReBar(hWnd, NULL, hWndComboBox1, hWndComboBox2);
        pAppCore->pCanvas = new Canvas(hWnd, hInst);
        //ShowWindow(pAppCore->pCanvas->hWindow, SW_SHOW);
        //UpdateWindow(pAppCore->pCanvas->hWindow);
    }
    else
    {
        LONG_PTR lp = GetWindowLongPtrW(hWnd, GWLP_USERDATA);
        pAppCore = reinterpret_cast<ApplicationCore*>(lp);
    }
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_EDIT_COPY:
                pAppCore->CopyToClipboard();
                break;
            case ID_SHAPE_RECTANGLE:
                pAppCore->SelectShapeType(ShapeType::RectangleShapeType);
                break;
            case ID_SHAPE_LINE:
                pAppCore->SelectShapeType(ShapeType::LineShapeType);
                break;
            case ID_SHAPE_FREEHAND:
                pAppCore->SelectShapeType(ShapeType::FreehandShapeType);
                break;
            case ID_SHAPE_ROUTE:
                pAppCore->SelectShapeType(ShapeType::RouteShapeType);
                break;
            case ID_PEN_SETTINGS:
                DialogBoxParamW(hInst, MAKEINTRESOURCE(IDD_PEN_SETTINGS), hWnd, PenSettingsDialogProcess, (LPARAM)pAppCore);
                break;
            case ID_BRUSH_SETTINGS:
                DialogBoxParamW(hInst, MAKEINTRESOURCE(IDD_BRUSH_SETTINGS), hWnd, BrushSettingsDialogProcess, (LPARAM)pAppCore);
                break;
            case ID_COLOR_BORDER:
            {
                //pAppCore->penColor = 
                    CreateColorPickerDialog(hWnd, pAppCore);
            }
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    //case WM_PAINT:
        //pAppCore->On_WM_PAINT();
        //break;
    case WM_LBUTTONDOWN:
        pAppCore->On_WM_LBUTTONDOWN(lParam);
        break;
    case WM_LBUTTONUP:
        pAppCore->On_WM_LBUTTONUP(lParam);
        break;
    case WM_RBUTTONDOWN:
        pAppCore->On_WM_RBUTTONDOWN(lParam);
        break;
    case WM_RBUTTONUP:
        pAppCore->On_WM_RBUTTONUP(lParam);
        break;
    case WM_MOUSEMOVE:
        pAppCore->On_WM_MOUSEMOVE(lParam);
        break;
    case WM_SIZE:
        pAppCore->On_WM_SIZE();
        break;
    case WM_VSCROLL:
        pAppCore->On_WM_VSCROLL();
        break;
    case WM_HSCROLL:
        pAppCore->On_WM_HSCROLL();
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

BOOL CALLBACK ColorPickerDialogProcess(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: replace ApplicationCore with some other class or struct which is responsible
    // for storing and handling data connected to this dialog. The RGB bitmap created in
    // ApplicationCore can be stored in some other location maybe.
    ApplicationCore* pAppCore;
    //COLORREF* pColor;
    if (message == WM_INITDIALOG)
    {
        pAppCore = reinterpret_cast<ApplicationCore*>(lParam);
        //pColor = reinterpret_cast<COLORREF*>(lParam);
        SetWindowLongPtrW(hwndDlg, GWLP_USERDATA, (LONG_PTR)pAppCore);
        //SetWindowLongPtrW(hwndDlg, GWLP_USERDATA, (LONG_PTR)pColor);
    }
    else
    {
        LONG_PTR lp = GetWindowLongPtrW(hwndDlg, GWLP_USERDATA);
        pAppCore = reinterpret_cast<ApplicationCore*>(lp);
        //pColor = reinterpret_cast<COLORREF*>(lp);
    }
    switch (message)
    {
    case WM_INITDIALOG:
        pAppCore->On_WM_INITDIALOG();
        return TRUE;
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC clientDC = BeginPaint(hwndDlg, &ps);
            RECT clientRect;
            GetClientRect(hwndDlg, &clientRect);
            RECT colorWheelRect = { 0, 0, 512, 512 };
            GdiTransparentBlt(clientDC, 5, 5, colorWheelRect.right - colorWheelRect.left, colorWheelRect.bottom - colorWheelRect.top, pAppCore->RGBDC, 0, 0, 512, 512, 0);
            DeleteDC(clientDC);
            EndPaint(hwndDlg, &ps);
            return TRUE;
        }
        break;
    case WM_MOUSEMOVE:
        return TRUE;
        break;
    case WM_LBUTTONUP:
        {
            // TODO: don't set the penColor, instead return the color which is selected
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            HDC clientDC = GetDC(hwndDlg);
            COLORREF color = GetPixel(clientDC, x, y);
            pAppCore->penColor = color;
            return TRUE;
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hwndDlg, wParam);
            return TRUE;
        case IDOK:
            {
                COLORREF color = pAppCore->penColor;
                EndDialog(hwndDlg, INT_PTR(color));
                return TRUE;
            }
        }
    default:
        return FALSE;
    }
}

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


INT_PTR CALLBACK PenSettingsDialogProcess(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ApplicationCore* pAppCore;
    if (msg == WM_INITDIALOG)
    {
        pAppCore = reinterpret_cast<ApplicationCore*>(lParam);
        SetWindowLongPtrW(hDlg, GWLP_USERDATA, (LONG_PTR)pAppCore);
    }
    else
    {
        LONG_PTR lp = GetWindowLongPtrW(hDlg, GWLP_USERDATA);
        pAppCore = reinterpret_cast<ApplicationCore*>(lp);
    }
    switch (msg)
    {
    case WM_INITDIALOG:
    {
        HWND hLst = GetDlgItem(hDlg, IDC_LIST_SIZE);
        for (int i = 1; i <= 30; ++i)
        {
            SendMessageW(hLst, LB_ADDSTRING, 0, (LPARAM)std::to_wstring(i).c_str());
        }
        hLst = GetDlgItem(hDlg, IDC_LIST_TYPE);
        SendMessageW(hLst, LB_ADDSTRING, 0, (LPARAM)L"Solid");
        SendMessageW(hLst, LB_ADDSTRING, 0, (LPARAM)L"Dash");
        SendMessageW(hLst, LB_ADDSTRING, 0, (LPARAM)L"Dot");
        SendMessageW(hLst, LB_ADDSTRING, 0, (LPARAM)L"Dash-dot");
        SendMessageW(hLst, LB_ADDSTRING, 0, (LPARAM)L"Dash-dot-dot");
        SendMessageW(hLst, LB_ADDSTRING, 0, (LPARAM)L"Null");
        SendMessageW(hLst, LB_ADDSTRING, 0, (LPARAM)L"Inside-frame");
        return (INT_PTR)TRUE;
    }
    break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_CHANGE_COLOR:
        {
            pAppCore->pCanvas->penColor = CreateColorPickerDialog(hDlg, pAppCore);
            HWND hCtl = GetDlgItem(hDlg, IDC_CURRENT_COLOR);
            SendMessageW(hCtl, WM_ENABLE, TRUE, 0);
        }
        break;
        case IDOK:
        case IDCANCEL:
        {
            EndDialog(hDlg, LOWORD(wParam));
            pAppCore->pCanvas->SetActivePen();
            return (INT_PTR)TRUE;
        }
        break;
        case IDC_LIST_SIZE:
            if (HIWORD(wParam) == LBN_SELCHANGE)
            {
                HWND hLst = GetDlgItem(hDlg, IDC_LIST_SIZE);
                pAppCore->pCanvas->penWidth = SendMessageW(hLst, LB_GETCURSEL, 0, 0) + 1;
            }
            break;
        case IDC_LIST_TYPE:
            if (HIWORD(wParam) == LBN_SELCHANGE)
            {
                HWND hLst = GetDlgItem(hDlg, IDC_LIST_TYPE);
                int penType = SendMessageW(hLst, LB_GETCURSEL, 0, 0);
                switch (penType)
                {
                case 0:
                    pAppCore->pCanvas->penStyle = PS_SOLID;
                    break;
                case 1:
                    pAppCore->pCanvas->penStyle = PS_DASH;
                    break;
                case 2:
                    pAppCore->pCanvas->penStyle = PS_DOT;
                    break;
                case 3:
                    pAppCore->pCanvas->penStyle = PS_DASHDOT;
                    break;
                case 4:
                    pAppCore->pCanvas->penStyle = PS_DASHDOTDOT;
                    break;
                case 5:
                    pAppCore->pCanvas->penStyle = PS_NULL;
                    break;
                case 6:
                    pAppCore->pCanvas->penStyle = PS_INSIDEFRAME;
                    break;
                }
            }
            break;
        }
        break;
    case WM_DRAWITEM:
        DRAWITEMSTRUCT* dis = reinterpret_cast<DRAWITEMSTRUCT*>(lParam);
        if (dis->CtlID == IDC_CURRENT_COLOR)
        {
            HDC hDc = dis->hDC;
            HPEN hPen = CreatePen(PS_SOLID, 1, 0);
            HBRUSH hBrush = CreateSolidBrush(pAppCore->penColor);
            HPEN hPenOld = (HPEN)SelectObject(hDc, hPen);
            HBRUSH hBrushOld = (HBRUSH)SelectObject(hDc, hBrush);
            dis->rcItem.left += 1;
            dis->rcItem.right -= 1;
            ::Rectangle(hDc, dis->rcItem.left, dis->rcItem.top, dis->rcItem.right, dis->rcItem.bottom);
            SelectObject(hDc, hPenOld);
            SelectObject(hDc, hBrushOld);
            DeleteObject(hBrush);
            DeleteObject(hPen);
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK BrushSettingsDialogProcess(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ApplicationCore* pAppCore;
    if (msg == WM_INITDIALOG)
    {
        pAppCore = reinterpret_cast<ApplicationCore*>(lParam);
        SetWindowLongPtrW(hDlg, GWLP_USERDATA, (LONG_PTR)pAppCore);
    }
    else
    {
        LONG_PTR lp = GetWindowLongPtrW(hDlg, GWLP_USERDATA);
        pAppCore = reinterpret_cast<ApplicationCore*>(lp);
    }
    switch (msg)
    {
    case WM_INITDIALOG:
    {
        return (INT_PTR)TRUE;
    }
    break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_CHANGE_BR_COLOR:
        {
            pAppCore->pCanvas->brushColor = CreateColorPickerDialog(hDlg, pAppCore);
            HWND hCtl = GetDlgItem(hDlg, IDC_CURRENT_BRUSH_COLOR);
            SendMessageW(hCtl, WM_ENABLE, TRUE, 0);
        }
        break;
        case IDOK:
        case IDCANCEL:
        {
            EndDialog(hDlg, LOWORD(wParam));
            pAppCore->pCanvas->SetActiveBrush();
            return (INT_PTR)TRUE;
        }
        break;
        }
        break;
    case WM_DRAWITEM:
        DRAWITEMSTRUCT* dis = reinterpret_cast<DRAWITEMSTRUCT*>(lParam);
        if (dis->CtlID == IDC_CURRENT_BRUSH_COLOR)
        {
            HDC hDc = dis->hDC;
            HPEN hPen = CreatePen(PS_SOLID, 1, 0);
            HBRUSH hBrush = CreateSolidBrush(pAppCore->brushColor);
            HPEN hPenOld = (HPEN)SelectObject(hDc, hPen);
            HBRUSH hBrushOld = (HBRUSH)SelectObject(hDc, hBrush);
            dis->rcItem.left += 1;
            dis->rcItem.right -= 1;
            ::Rectangle(hDc, dis->rcItem.left, dis->rcItem.top, dis->rcItem.right, dis->rcItem.bottom);
            SelectObject(hDc, hPenOld);
            SelectObject(hDc, hBrushOld);
            DeleteObject(hBrush);
            DeleteObject(hPen);
        }
        break;
    }
    return (INT_PTR)FALSE;
}
