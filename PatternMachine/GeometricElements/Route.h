#pragma once
#include "Shape.h"
#include <vector>

namespace PatternMachine {


class Route :
    public Shape
{
public:
    Route(Canvas*);
    Route(HWND);
    Route(Layer* pLayer);
    Route(const Route& route);
    Route* Clone() override;
    void StartSizing(POINT) override;
    void Sizing(POINT, POINT) override;
    void StopSizing() override;
    void SetHitRegion() override;
    HRGN SetSegmentHitRegion(Point*, Point*);
    void MoveBy(POINT) override;
    void Draw() override;
    std::vector<POINT> routePoints;
    POINT endPoint;
private:
    POINT startPoint;
    void CalculateRoute();
    void CalculateRoute2();
    double Distance(POINT, POINT);
    void SetSubRoute(POINT, POINT);
};

}
