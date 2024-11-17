#pragma once
#include "Shape.h"

namespace PatternMachine
{

class Freehand :
    public Shape
{
public:
    void StartSizing(POINT);
    void Sizing(POINT, POINT);
    void StopSizing();
    POINT previousPoint;
    POINT currentPoint;
private:
};
}

