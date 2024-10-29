// DONE: Move the below list before the winmain function in the application

// TODOS:

// TODO: Adding capability of drawing shapes with the following options
//      - let the color of the interior be selectable
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
#include <time.h>
#include <string>>

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
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                RegisterCanvasClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, ApplicationCore);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PenSettingsDialogProcess(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK ColorPickerDialogProcess(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

HANDLE OpenLog()
{
    return CreateFile(L"LOG.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

void Log(UINT msg, WPARAM wParam, LPARAM lParam)
{
    wchar_t wct[120];
    std::wstring ws = std::to_wstring(msg);
    switch (msg)
    {
    case WM_SIZE:
        ws = L"WM_SIZE";
        break;
    case WM_CREATE:
        ws = L"WM_CREATE";
        break;
    case WM_COMMAND:
        ws = L"WM_COMMAND";
        break;
    case WM_PAINT:
        ws = L"WM_PAINT";
        break;
    case WM_LBUTTONDOWN:
        ws = L"WM_LBUTTONDOWN";
        break;
    case WM_LBUTTONUP:
        ws = L"WM_LBUTTONUP";
        break;
    case WM_RBUTTONDOWN:
        ws = L"WM_RBUTTONDOWN";
        break;
    case WM_RBUTTONUP:
        ws = L"WM_RBUTTONUP";
        break;
    case WM_MOUSEMOVE:
        ws = L"WM_MOUSEMOVE";
        break;
    case WM_KEYDOWN:
        ws = L"WM_KEYDOWN";
        break;
    case WM_VSCROLL:
        ws = L"WM_VSCROLL";
        break;
    case WM_HSCROLL:
        ws = L"WM_HSCROLL";
        break;
    case WM_DESTROY:
        ws = L"WM_DESTROY";
        break;
    case WM_ACTIVATEAPP:
        ws = L"WM_ACTIVATEAPP";
        break;
    case WM_CANCELMODE:
        ws = L"WM_CANCELMODE";
        break;
    case WM_CHILDACTIVATE:
        ws = L"WM_CHILDACTIVATE";
        break;
    case WM_CLOSE:
        ws = L"WM_CLOSE";
        break;
    case WM_COMPACTING:
        ws = L"WM_COMPACTING";
        break;
    case WM_DPICHANGED:
        ws = L"WM_DPICHANGED";
        break;
    case WM_ENABLE:
        ws = L"WM_ENABLE";
        break;
    case WM_ENTERSIZEMOVE:
        ws = L"WM_ENTERSIZEMOVE";
        break;
    case WM_EXITSIZEMOVE:
        ws = L"WM_EXITSIZEMOVE";
        break;
    case WM_GETICON:
        ws = L"WM_GETICON";
        break;
    case WM_GETMINMAXINFO:
        ws = L"WM_GETMINMAXINFO";
        break;
    case WM_INPUTLANGCHANGE:
        ws = L"WM_INPUTLANGCHANGE";
        break;
    case WM_INPUTLANGCHANGEREQUEST:
        ws = L"WM_INPUTLANGCHANGEREQUEST";
        break;
    case WM_MOVE:
        ws = L"WM_MOVE";
        break;
    case WM_MOVING:
        ws = L"WM_MOVING";
        break;
    case WM_NCACTIVATE:
        ws = L"WM_NCACTIVATE";
        break;
    case WM_NCCALCSIZE:
        ws = L"WM_NCCALCSIZE";
        break;
    case WM_NCCREATE:
        ws = L"WM_NCCREATE";
        break;
    case WM_NCDESTROY:
        ws = L"WM_NCDESTROY";
        break;
    case WM_NULL:
        ws = L"WM_NULL";
        break;
    case WM_QUERYDRAGICON:
        ws = L"WM_QUERYDRAGICON";
        break;
    case WM_QUERYOPEN:
        ws = L"WM_QUERYOPEN";
        break;
    case WM_QUIT:
        ws = L"WM_QUIT";
        break;
    case WM_SHOWWINDOW:
        ws = L"WM_SHOWWINDOW";
        break;
    case WM_SIZING:
        ws = L"WM_SIZING";
        break;
    case WM_STYLECHANGED:
        ws = L"WM_STYLECHANGED";
        break;
    case WM_STYLECHANGING:
        ws = L"WM_STYLECHANGING";
        break;
    case WM_THEMECHANGED:
        ws = L"WM_THEMECHANGED";
        break;
    case WM_USERCHANGED:
        ws = L"WM_USERCHANGED";
        break;
    case WM_WINDOWPOSCHANGED:
        ws = L"WM_WINDOWPOSCHANGED";
        break;
    case WM_WINDOWPOSCHANGING:
        ws = L"WM_WINDOWPOSCHANGING";
        break;
    case WM_SETFOCUS:
        ws = L"WM_SETFOCUS";
        break;
    case WM_KILLFOCUS:
        ws = L"WM_KILLFOCUS";
        break;
    case WM_SETREDRAW:
        ws = L"WM_SETREDRAW";
        break;
    case WM_SETTEXT:
        ws = L"WM_SETTEXT";
        break;
    case WM_GETTEXT:
        ws = L"WM_GETTEXT";
        break;
    case WM_GETTEXTLENGTH:
        ws = L"WM_GETTEXTLENGTH";
        break;
    case WM_QUERYENDSESSION:
        ws = L"WM_QUERYENDSESSION";
        break;
    case WM_ENDSESSION:
        ws = L"WM_ENDSESSION";
        break;
    case WM_ERASEBKGND:
        ws = L"WM_ERASEBKGND";
        break;
    case WM_SYSCOLORCHANGE:
        ws = L"WM_SYSCOLORCHANGE";
        break;
    case WM_WININICHANGE:
        ws = L"WM_WININICHANGE";
        break;
    case WM_DEVMODECHANGE:
        ws = L"WM_DEVMODECHANGE";
        break;
    case WM_FONTCHANGE:
        ws = L"WM_FONTCHANGE";
        break;
    case WM_TIMECHANGE:
        ws = L"WM_TIMECHANGE";
        break;
    case WM_SETCURSOR:
        ws = L"WM_SETCURSOR";
        break;
    case WM_MOUSEACTIVATE:
        ws = L"WM_MOUSEACTIVATE";
        break;
    case WM_QUEUESYNC:
        ws = L"WM_QUEUESYNC";
        break;
    case WM_PAINTICON:
        ws = L"WM_PAINTICON";
        break;
    case WM_ICONERASEBKGND:
        ws = L"WM_ICONERASEBKGND";
        break;
    case WM_NEXTDLGCTL:
        ws = L"WM_NEXTDLGCTL";
        break;
    case WM_SPOOLERSTATUS:
        ws = L"WM_SPOOLERSTATUS";
        break;
    case WM_DRAWITEM:
        ws = L"WM_DRAWITEM";
        break;
    case WM_MEASUREITEM:
        ws = L"WM_MEASUREITEM";
        break;
    case WM_DELETEITEM:
        ws = L"WM_DELETEITEM";
        break;
    case WM_VKEYTOITEM:
        ws = L"WM_VKEYTOITEM";
        break;
    case WM_CHARTOITEM:
        ws = L"WM_CHARTOITEM";
        break;
    case WM_SETFONT:
        ws = L"WM_SETFONT";
        break;
    case WM_GETFONT:
        ws = L"WM_GETFONT";
        break;
    case WM_SETHOTKEY:
        ws = L"WM_SETHOTKEY";
        break;
    case WM_GETHOTKEY:
        ws = L"WM_GETHOTKEY";
        break;
    case WM_COMPAREITEM:
        ws = L"WM_COMPAREITEM";
        break;
    case WM_GETOBJECT:
        ws = L"WM_GETOBJECT";
        break;
    case WM_COMMNOTIFY:
        ws = L"WM_COMMNOTIFY";
        break;
    case WM_NOTIFY:
        ws = L"WM_NOTIFY";
        break;
    case WM_TCARD:
        ws = L"WM_TCARD";
        break;
    case WM_HELP:
        ws = L"WM_HELP";
        break;
    case WM_NOTIFYFORMAT:
        ws = L"WM_NOTIFYFORMAT";
        break;
    case WM_CONTEXTMENU:
        ws = L"WM_CONTEXTMENU";
        break;
    case WM_DISPLAYCHANGE:
        ws = L"WM_DISPLAYCHANGE";
        break;
    case WM_SETICON:
        ws = L"WM_SETICON";
        break;
    case WM_NCHITTEST:
        ws = L"WM_NCHITTEST";
        break;
    case WM_NCPAINT:
        ws = L"WM_NCPAINT";
        break;
    case WM_GETDLGCODE:
        ws = L"WM_GETDLGCODE";
        break;
    case WM_SYNCPAINT:
        ws = L"WM_SYNCPAINT";
        break;
    case WM_NCMOUSEMOVE:
        ws = L"WM_NCMOUSEMOVE";
        break;
    case WM_NCLBUTTONDOWN:
        ws = L"WM_NCLBUTTONDOWN";
        break;
    case WM_NCLBUTTONUP:
        ws = L"WM_NCLBUTTONUP";
        break;
    case WM_NCLBUTTONDBLCLK:
        ws = L"WM_NCLBUTTONDBLCLK";
        break;
    case WM_NCRBUTTONDOWN:
        ws = L"WM_NCRBUTTONDOWN";
        break;
    case WM_NCRBUTTONUP:
        ws = L"WM_NCRBUTTONUP";
        break;
    case WM_NCRBUTTONDBLCLK:
        ws = L"WM_NCRBUTTONDBLCLK";
        break;
    case WM_NCMBUTTONDOWN:
        ws = L"WM_NCMBUTTONDOWN";
        break;
    case WM_NCMBUTTONUP:
        ws = L"WM_NCMBUTTONUP";
        break;
    case WM_NCMBUTTONDBLCLK:
        ws = L"WM_NCMBUTTONDBLCLK";
        break;
    case WM_NCXBUTTONDOWN:
        ws = L"WM_NCXBUTTONDOWN";
        break;
    case WM_NCXBUTTONUP:
        ws = L"WM_NCXBUTTONUP";
        break;
    case WM_NCXBUTTONDBLCLK:
        ws = L"WM_NCXBUTTONDBLCLK";
        break;
    case WM_INPUT_DEVICE_CHANGE:
        ws = L"WM_INPUT_DEVICE_CHANGE";
        break;
    case WM_INPUT:
        ws = L"WM_INPUT";
        break;
    case WM_KEYUP:
        ws = L"WM_KEYUP";
        break;
    case WM_CHAR:
        ws = L"WM_CHAR";
        break;
    case WM_DEADCHAR:
        ws = L"WM_DEADCHAR";
        break;
    case WM_SYSKEYDOWN:
        ws = L"WM_SYSKEYDOWN";
        break;
    case WM_SYSKEYUP:
        ws = L"WM_SYSKEYUP";
        break;
    case WM_SYSCHAR:
        ws = L"WM_SYSCHAR";
        break;
    case WM_SYSDEADCHAR:
        ws = L"WM_SYSDEADCHAR";
        break;
    case WM_UNICHAR:
        ws = L"WM_UNICHAR";
        break;
    case WM_IME_STARTCOMPOSITION:
        ws = L"WM_IME_STARTCOMPOSITION";
        break;
    case WM_IME_ENDCOMPOSITION:
        ws = L"WM_IME_ENDCOMPOSITION";
        break;
    case WM_IME_COMPOSITION:
        ws = L"WM_IME_COMPOSITION";
        break;
    case WM_INITDIALOG:
        ws = L"WM_INITDIALOG";
        break;
    case WM_SYSCOMMAND:
        ws = L"WM_SYSCOMMAND";
        break;
    case WM_TIMER:
        ws = L"WM_TIMER";
        break;
    case WM_INITMENU:
        ws = L"WM_INITMENU";
        break;
    case WM_INITMENUPOPUP:
        ws = L"WM_INITMENUPOPUP";
        break;
    case WM_GESTURE:
        ws = L"WM_GESTURE";
        break;
    case WM_GESTURENOTIFY:
        ws = L"WM_GESTURENOTIFY";
        break;
    case WM_MENUSELECT:
        ws = L"WM_MENUSELECT";
        break;
    case WM_MENUCHAR:
        ws = L"WM_MENUCHAR";
        break;
    case WM_ENTERIDLE:
        ws = L"WM_ENTERIDLE";
        break;
    case WM_MENURBUTTONUP:
        ws = L"WM_MENURBUTTONUP";
        break;
    case WM_MENUDRAG:
        ws = L"WM_MENUDRAG";
        break;
    case WM_MENUGETOBJECT:
        ws = L"WM_MENUGETOBJECT";
        break;
    case WM_UNINITMENUPOPUP:
        ws = L"WM_UNINITMENUPOPUP";
        break;
    case WM_MENUCOMMAND:
        ws = L"WM_MENUCOMMAND";
        break;
    case WM_ACTIVATE:
        ws = L"WM_ACTIVATE";
        break;
    case WM_POWER:
        ws = L"WM_POWER";
        break;
    case WM_COPYDATA:
        ws = L"WM_COPYDATA";
        break;
    case WM_CANCELJOURNAL:
        ws = L"WM_CANCELJOURNAL";
        break;
    case WM_CHANGEUISTATE:
        ws = L"WM_CHANGEUISTATE";
        break;
    case WM_UPDATEUISTATE:
        ws = L"WM_UPDATEUISTATE";
        break;
    case WM_QUERYUISTATE:
        ws = L"WM_QUERYUISTATE";
        break;
    case WM_CTLCOLORMSGBOX:
        ws = L"WM_CTLCOLORMSGBOX";
        break;
    case WM_CTLCOLOREDIT:
        ws = L"WM_CTLCOLOREDIT";
        break;
    case WM_CTLCOLORLISTBOX:
        ws = L"WM_CTLCOLORLISTBOX";
        break;
    case WM_CTLCOLORBTN:
        ws = L"WM_CTLCOLORBTN";
        break;
    case WM_CTLCOLORDLG:
        ws = L"WM_CTLCOLORDLG";
        break;
    case WM_CTLCOLORSCROLLBAR:
        ws = L"WM_CTLCOLORSCROLLBAR";
        break;
    case WM_CTLCOLORSTATIC:
        ws = L"WM_CTLCOLORSTATIC";
        break;
    case WM_LBUTTONDBLCLK:
        ws = L"WM_LBUTTONDBLCLK";
        break;
    case WM_RBUTTONDBLCLK:
        ws = L"WM_RBUTTONDBLCLK";
        break;
    case WM_MBUTTONDOWN:
        ws = L"WM_MBUTTONDOWN";
        break;
    case WM_MBUTTONUP:
        ws = L"WM_MBUTTONUP";
        break;
    case WM_MBUTTONDBLCLK:
        ws = L"WM_MBUTTONDBLCLK";
        break;
    case WM_MOUSEWHEEL:
        ws = L"WM_MOUSEWHEEL";
        break;
    case WM_XBUTTONDOWN:
        ws = L"WM_XBUTTONDOWN";
        break;
    case WM_XBUTTONUP:
        ws = L"WM_XBUTTONUP";
        break;
    case WM_XBUTTONDBLCLK:
        ws = L"WM_XBUTTONDBLCLK";
        break;
    case WM_MOUSEHWHEEL:
        ws = L"WM_MOUSEHWHEEL";
        break;
    case WM_PARENTNOTIFY:
        ws = L"WM_PARENTNOTIFY";
        break;
    case WM_ENTERMENULOOP:
        ws = L"WM_ENTERMENULOOP";
        break;
    case WM_EXITMENULOOP:
        ws = L"WM_EXITMENULOOP";
        break;
    case WM_NEXTMENU:
        ws = L"WM_NEXTMENU";
        break;
    case WM_CAPTURECHANGED:
        ws = L"WM_CAPTURECHANGED";
        break;
    case WM_POWERBROADCAST:
        ws = L"WM_POWERBROADCAST";
        break;
    case WM_DEVICECHANGE:
        ws = L"WM_DEVICECHANGE";
        break;
    case WM_MDICREATE:
        ws = L"WM_MDICREATE";
        break;
    case WM_MDIDESTROY:
        ws = L"WM_MDIDESTROY";
        break;
    case WM_MDIACTIVATE:
        ws = L"WM_MDIACTIVATE";
        break;
    case WM_MDIRESTORE:
        ws = L"WM_MDIRESTORE";
        break;
    case WM_MDINEXT:
        ws = L"WM_MDINEXT";
        break;
    case WM_MDIMAXIMIZE:
        ws = L"WM_MDIMAXIMIZE";
        break;
    case WM_MDITILE:
        ws = L"WM_MDITILE";
        break;
    case WM_MDICASCADE:
        ws = L"WM_MDICASCADE";
        break;
    case WM_MDIICONARRANGE:
        ws = L"WM_MDIICONARRANGE";
        break;
    case WM_MDIGETACTIVE:
        ws = L"WM_MDIGETACTIVE";
        break;
    case WM_MDISETMENU:
        ws = L"WM_MDISETMENU";
        break;
    case WM_DROPFILES:
        ws = L"WM_DROPFILES";
        break;
    case WM_MDIREFRESHMENU:
        ws = L"WM_MDIREFRESHMENU";
        break;
    case WM_POINTERDEVICECHANGE:
        ws = L"WM_POINTERDEVICECHANGE";
        break;
    case WM_POINTERDEVICEINRANGE:
        ws = L"WM_POINTERDEVICEINRANGE";
        break;
    case WM_POINTERDEVICEOUTOFRANGE:
        ws = L"WM_POINTERDEVICEOUTOFRANGE";
        break;
    case WM_TOUCH:
        ws = L"WM_TOUCH";
        break;
    case WM_NCPOINTERUPDATE:
        ws = L"WM_NCPOINTERUPDATE";
        break;
    case WM_NCPOINTERDOWN:
        ws = L"WM_NCPOINTERDOWN";
        break;
    case WM_NCPOINTERUP:
        ws = L"WM_NCPOINTERUP";
        break;
    case WM_POINTERUPDATE:
        ws = L"WM_POINTERUPDATE";
        break;
    case WM_POINTERDOWN:
        ws = L"WM_POINTERDOWN";
        break;
    case WM_POINTERUP:
        ws = L"WM_POINTERUP";
        break;
    case WM_POINTERENTER:
        ws = L"WM_POINTERENTER";
        break;
    case WM_POINTERLEAVE:
        ws = L"WM_POINTERLEAVE";
        break;
    case WM_POINTERACTIVATE:
        ws = L"WM_POINTERACTIVATE";
        break;
    case WM_POINTERCAPTURECHANGED:
        ws = L"WM_POINTERCAPTURECHANGED";
        break;
    case WM_TOUCHHITTESTING:
        ws = L"WM_TOUCHHITTESTING";
        break;
    case WM_POINTERWHEEL:
        ws = L"WM_POINTERWHEEL";
        break;
    case WM_POINTERHWHEEL:
        ws = L"WM_POINTERHWHEEL";
        break;
    case DM_POINTERHITTEST:
        ws = L"DM_POINTERHITTEST";
        break;
    case WM_POINTERROUTEDTO:
        ws = L"WM_POINTERROUTEDTO";
        break;
    case WM_POINTERROUTEDAWAY:
        ws = L"WM_POINTERROUTEDAWAY";
        break;
    case WM_POINTERROUTEDRELEASED:
        ws = L"WM_POINTERROUTEDRELEASED";
        break;
    case WM_IME_SETCONTEXT:
        ws = L"WM_IME_SETCONTEXT";
        break;
    case WM_IME_NOTIFY:
        ws = L"WM_IME_NOTIFY";
        break;
    case WM_IME_CONTROL:
        ws = L"WM_IME_CONTROL";
        break;
    case WM_IME_COMPOSITIONFULL:
        ws = L"WM_IME_COMPOSITIONFULL";
        break;
    case WM_IME_SELECT:
        ws = L"WM_IME_SELECT";
        break;
    case WM_IME_CHAR:
        ws = L"WM_IME_CHAR";
        break;
    case WM_IME_REQUEST:
        ws = L"WM_IME_REQUEST";
        break;
    case WM_IME_KEYDOWN:
        ws = L"WM_IME_KEYDOWN";
        break;
    case WM_IME_KEYUP:
        ws = L"WM_IME_KEYUP";
        break;
    case WM_MOUSEHOVER:
        ws = L"WM_MOUSEHOVER";
        break;
    case WM_MOUSELEAVE:
        ws = L"WM_MOUSELEAVE";
        break;
    case WM_NCMOUSEHOVER:
        ws = L"WM_NCMOUSEHOVER";
        break;
    case WM_NCMOUSELEAVE:
        ws = L"WM_NCMOUSELEAVE";
        break;
    case WM_WTSSESSION_CHANGE:
        ws = L"WM_WTSSESSION_CHANGE";
        break;
    case WM_TABLET_FIRST:
        ws = L"WM_TABLET_FIRST";
        break;
    case WM_TABLET_LAST:
        ws = L"WM_TABLET_LAST";
        break;
    case WM_DPICHANGED_BEFOREPARENT:
        ws = L"WM_DPICHANGED_BEFOREPARENT";
        break;
    case WM_DPICHANGED_AFTERPARENT:
        ws = L"WM_DPICHANGED_AFTERPARENT";
        break;
    case WM_GETDPISCALEDSIZE:
        ws = L"WM_GETDPISCALEDSIZE";
        break;
    case WM_CUT:
        ws = L"WM_CUT";
        break;
    case WM_COPY:
        ws = L"WM_COPY";
        break;
    case WM_PASTE:
        ws = L"WM_PASTE";
        break;
    case WM_CLEAR:
        ws = L"WM_CLEAR";
        break;
    case WM_UNDO:
        ws = L"WM_UNDO";
        break;
    case WM_RENDERFORMAT:
        ws = L"WM_RENDERFORMAT";
        break;
    case WM_RENDERALLFORMATS:
        ws = L"WM_RENDERALLFORMATS";
        break;
    case WM_DESTROYCLIPBOARD:
        ws = L"WM_DESTROYCLIPBOARD";
        break;
    case WM_DRAWCLIPBOARD:
        ws = L"WM_DRAWCLIPBOARD";
        break;
    case WM_PAINTCLIPBOARD:
        ws = L"WM_PAINTCLIPBOARD";
        break;
    case WM_VSCROLLCLIPBOARD:
        ws = L"WM_VSCROLLCLIPBOARD";
        break;
    case WM_SIZECLIPBOARD:
        ws = L"WM_SIZECLIPBOARD";
        break;
    case WM_ASKCBFORMATNAME:
        ws = L"WM_ASKCBFORMATNAME";
        break;
    case WM_CHANGECBCHAIN:
        ws = L"WM_CHANGECBCHAIN";
        break;
    case WM_HSCROLLCLIPBOARD:
        ws = L"WM_HSCROLLCLIPBOARD";
        break;
    case WM_QUERYNEWPALETTE:
        ws = L"WM_QUERYNEWPALETTE";
        break;
    case WM_PALETTEISCHANGING:
        ws = L"WM_PALETTEISCHANGING";
        break;
    case WM_PALETTECHANGED:
        ws = L"WM_PALETTECHANGED";
        break;
    case WM_HOTKEY:
        ws = L"WM_HOTKEY";
        break;
    case WM_PRINT:
        ws = L"WM_PRINT";
        break;
    case WM_PRINTCLIENT:
        ws = L"WM_PRINTCLIENT";
        break;
    case WM_APPCOMMAND:
        ws = L"WM_APPCOMMAND";
        break;
    case WM_CLIPBOARDUPDATE:
        ws = L"WM_CLIPBOARDUPDATE";
        break;
    case WM_DWMCOMPOSITIONCHANGED:
        ws = L"WM_DWMCOMPOSITIONCHANGED";
        break;
    case WM_DWMNCRENDERINGCHANGED:
        ws = L"WM_DWMNCRENDERINGCHANGED";
        break;
    case WM_DWMCOLORIZATIONCOLORCHANGED:
        ws = L"WM_DWMCOLORIZATIONCOLORCHANGED";
        break;
    case WM_DWMWINDOWMAXIMIZEDCHANGE:
        ws = L"WM_DWMWINDOWMAXIMIZEDCHANGE";
        break;
    case WM_DWMSENDICONICTHUMBNAIL:
        ws = L"WM_DWMSENDICONICTHUMBNAIL";
        break;
    case WM_DWMSENDICONICLIVEPREVIEWBITMAP:
        ws = L"WM_DWMSENDICONICLIVEPREVIEWBITMAP";
        break;
    case WM_GETTITLEBARINFOEX:
        ws = L"WM_GETTITLEBARINFOEX";
        break;
    case WM_HANDHELDFIRST:
        ws = L"WM_HANDHELDFIRST";
        break;
    case WM_HANDHELDLAST:
        ws = L"WM_HANDHELDLAST";
        break;
    case WM_AFXFIRST:
        ws = L"WM_AFXFIRST";
        break;
    case WM_AFXLAST:
        ws = L"WM_AFXLAST";
        break;
    case WM_PENWINFIRST:
        ws = L"WM_PENWINFIRST";
        break;
    case WM_PENWINLAST:
        ws = L"WM_PENWINLAST";
        break;
    case WM_APP:
        ws = L"WM_APP";
        break;
    case WM_USER:
        ws = L"WM_USER";
        break;
    }
    wcscpy_s(wct, ws.c_str());
    wcsncat_s(wct, L"                         ", 25 - wcslen(wct));
    wcscat_s(wct, std::to_wstring(wParam).c_str());
    wcsncat_s(wct, L"             ", 38 - wcslen(wct));
    wcscat_s(wct, std::to_wstring(lParam).c_str());
    wcscat_s(wct, L"\n");
    DWORD dwBytesToWrite = wcslen(wct) * 2;
    DWORD dwBytesWritten = 0;
    WriteFile(hLogFile, wct, dwBytesToWrite, &dwBytesWritten, NULL);
}

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
    Rectangle(memoryDC, 2, 3, 14, 13);
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
    MyRegisterClass(hInstance);
    if (!RegisterCanvasClass(hInstance))
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

BOOL RegisterCanvasClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = ChildWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(255, 150, 150));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PATTERNMACHINE);
    wcex.lpszClassName = L"Canvas";
    wcex.hIconSm = 0;

    if (!RegisterClassExW(&wcex))
    {
        return FALSE;
    }
    return TRUE;
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PATTERNMACHINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.hbrBackground  = CreateSolidBrush(RGB(150, 150, 150));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PATTERNMACHINE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, ApplicationCore appCore)
{
   hInst = hInstance; // Store instance handle in our global variable
   HMENU hMenu = LoadMenuW(hInstance, MAKEINTRESOURCEW(IDC_PATTERNMACHINE));
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
}

HWND CreateCanvas(HWND hWndParent, HINSTANCE hInstance)
{
    HWND hCanvas = CreateWindowW(
        L"Canvas",
        (LPCTSTR)NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_SIZEBOX,
        //WS_OVERLAPPEDWINDOW,
        50, 50, 500, 300,
        hWndParent,
        (HMENU)(int)1005,
        hInstance,
        0);
    return hCanvas;
}

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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
        HWND hCanvas = CreateCanvas(hWnd, hInst);
        if (!hCanvas) PostQuitMessage(0);
        ShowWindow(hCanvas, SW_SHOW);
        UpdateWindow(hCanvas);
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
            // DONE: replace these many cases with a dialog box, which gets the width of the pen
            /*case ID_PEN_1:
                pAppCore->penWidth = 1;
                break;
            case ID_PEN_2:
                pAppCore->penWidth = 2;
                break;
            case ID_PEN_3:
                pAppCore->penWidth = 3;
                break;
            case ID_PEN_4:
                pAppCore->penWidth = 4;
                break;
            case ID_PEN_5:
                pAppCore->penWidth = 5;
                break;
            case ID_PEN_6:
                pAppCore->penWidth = 6;
                break;
            case ID_PEN_7:
                pAppCore->penWidth = 7;
                break;
            case ID_PEN_8:
                pAppCore->penWidth = 8;
                break;
            case ID_PEN_9:
                pAppCore->penWidth = 9;
                break;
            case ID_PEN_10:
                pAppCore->penWidth = 10;
                break;
            case ID_PEN_11:
                pAppCore->penWidth = 11;
                break;
            case ID_PEN_12:
                pAppCore->penWidth = 12;
                break;
            case ID_PEN_13:
                pAppCore->penWidth = 13;
                break;
            case ID_PEN_14:
                pAppCore->penWidth = 14;
                break;
            case ID_PEN_15:
                pAppCore->penWidth = 15;
                break;
            case ID_PEN_16:
                pAppCore->penWidth = 16;
                break;
            case ID_PEN_17:
                pAppCore->penWidth = 17;
                break;
            case ID_PEN_18:
                pAppCore->penWidth = 18;
                break;
            case ID_PEN_19:
                pAppCore->penWidth = 19;
                break;
            case ID_PEN_20:
                pAppCore->penWidth = 20;
                break;
            case ID_PEN_21:
                pAppCore->penWidth = 21;
                break;
            case ID_PEN_22:
                pAppCore->penWidth = 22;
                break;
            case ID_PEN_23:
                pAppCore->penWidth = 23;
                break;
            case ID_PEN_24:
                pAppCore->penWidth = 24;
                break;
            case ID_PEN_25:
                pAppCore->penWidth = 25;
                break;
            case ID_PEN_26:
                pAppCore->penWidth = 26;
                break;
            case ID_PEN_27:
                pAppCore->penWidth = 27;
                break;
            case ID_PEN_28:
                pAppCore->penWidth = 28;
                break;
            case ID_PEN_29:
                pAppCore->penWidth = 29;
                break;
            case ID_PEN_30:
                pAppCore->penWidth = 30;
                break;*/
            case ID_PEN_SETTINGS:
                DialogBoxParamW(hInst, MAKEINTRESOURCE(IDD_PEN_SETTINGS), hWnd, PenSettingsDialogProcess, (LPARAM)pAppCore);
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
    case WM_PAINT:
        pAppCore->On_WM_PAINT();
        break;
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
        // DONE: remove this case, because it is now implemented in a menu, connected with an accelerator
    case WM_KEYDOWN:
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
            COLORREF color = pAppCore->penColor;// RGB(0, 255, 255);
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
            CreateColorPickerDialog(hDlg, pAppCore);
            HWND hCtl = GetDlgItem(hDlg, IDC_CURRENT_COLOR);
            SendMessageW(hCtl, WM_ENABLE, TRUE, 0);
        }
            break;
        case IDOK:
        case IDCANCEL:
        {
            EndDialog(hDlg, LOWORD(wParam));
            pAppCore->SetActivePen();
            return (INT_PTR)TRUE;
        }
        break;
        case IDC_LIST_SIZE:
            if (HIWORD(wParam) == LBN_SELCHANGE)
            {
                HWND hLst = GetDlgItem(hDlg, IDC_LIST_SIZE);
                pAppCore->penWidth = SendMessageW(hLst, LB_GETCURSEL, 0, 0) + 1;
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
                    pAppCore->penStyle = PS_SOLID;
                    break;
                case 1:
                    pAppCore->penStyle = PS_DASH;
                    break;
                case 2:
                    pAppCore->penStyle = PS_DOT;
                    break;
                case 3:
                    pAppCore->penStyle = PS_DASHDOT;
                    break;
                case 4:
                    pAppCore->penStyle = PS_DASHDOTDOT;
                    break;
                case 5:
                    pAppCore->penStyle = PS_NULL;
                    break;
                case 6:
                    pAppCore->penStyle = PS_INSIDEFRAME;
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
            Rectangle(hDc, dis->rcItem.left, dis->rcItem.top, dis->rcItem.right, dis->rcItem.bottom);
            SelectObject(hDc, hPenOld);
            SelectObject(hDc, hBrushOld);
            DeleteObject(hBrush);
            DeleteObject(hPen);
        }
        break;
    }
    return (INT_PTR)FALSE;
}
