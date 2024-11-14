#pragma once
#include "Shape.h"

namespace PatternMachine
{

class Rectangle :
    public Shape
{
public:
    //RECT BoundingRectangle();
    void StartSizing(POINT);
    void Sizing(POINT, POINT);
    void StopSizing();
    HWND mainWindow;
};

}
