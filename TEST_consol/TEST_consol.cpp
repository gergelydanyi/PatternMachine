// TEST_consol.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "windows.h"

LPWORD lpwAlign(LPWORD lpIn)
{
    LPWORD lpw = lpIn;
    std::cout << "BEFORE alignment - lpw: " << lpw << std::endl;

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

    lpw = (LPWORD)ull;
    std::cout << "AFTER alignment - lpw:  " << lpw << std::endl;

    return (LPWORD)ull;
}

int main()
{
    HGLOBAL hgbl;
    LPDLGTEMPLATE lpdt;
    LPDLGITEMTEMPLATE lpdit;
    LPWORD lpw;
    LPWSTR lpwsz;
    int nchar;

    //int* a;
    //std::cout << "a: " << a << std::endl;

    hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
    if (!hgbl)
        return -1;

    std::cout << "hgbl: " << hgbl << std::endl;
    
    lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);
    
    std::cout << "lpdt: " << lpdt << std::endl;
    
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

    std::cout << "lpw 1st alignment: " << std::endl;
    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
    std::cout << "lpw 2nd alignment: " << std::endl;
    lpw++;
    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE)lpw;
    lpdit->x = 10; lpdit->y = 70;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
