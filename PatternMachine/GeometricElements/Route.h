#pragma once
#include "Shape.h"
#include <vector>

namespace PatternMachine {


class Route :
    public Shape
{
public:
    void StartSizing(POINT);
    void Sizing(POINT, POINT);
    void StopSizing();
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
