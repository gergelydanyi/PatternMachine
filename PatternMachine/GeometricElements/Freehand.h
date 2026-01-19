#pragma once
#include "Shape.h"

namespace PatternMachine
{

class Freehand :
    public Shape
{
public:
    Freehand(Canvas*);
    Freehand(HWND);
    Freehand(Layer*);
    Freehand* Clone() override;
    void StartSizing(POINT) override;
    void Sizing(POINT, POINT) override;
    void StopSizing() override;
    void SetHitRegion() override;
    HRGN SetSegmentHitRegion(Point*, Point*);
    void MoveBy(POINT) override;
    void Draw() override;
    POINT previousPoint;
    POINT currentPoint;
private:
};
}

