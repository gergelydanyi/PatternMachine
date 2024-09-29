#pragma once
#include "Shape.h"

namespace PatternMachine
{

class Freehand :
    public Shape
{
public:
    bool isEditing();
    void StartSizing(POINT);
    void Sizing(POINT, POINT);
    void StopSizing();
    HWND mainWindow;
    POINT previousPoint;
    POINT currentPoint;
private:
};
}

