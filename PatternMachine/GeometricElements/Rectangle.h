#pragma once
#include "Shape.h"

namespace PatternMachine
{

class Rectangle :
    public Shape
{
public:
    //RECT BoundingRectangle();
    Rectangle();
    void StartSizing(POINT) override;
    void Sizing(POINT, POINT) override;
    void StopSizing() override;
    void SetHitRegion() override;
    void MoveBy(POINT) override;
};

}
