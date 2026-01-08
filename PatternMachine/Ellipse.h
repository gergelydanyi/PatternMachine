#pragma once
#include "Shape.h"

namespace PatternMachine
{
    class Canvas;
    class Ellipse :
        public Shape
    {
    public:
        Ellipse(Canvas*);
        Ellipse(HWND);
        Ellipse(Layer* pLayer);
        void StartSizing(POINT) override;
        void Sizing(POINT, POINT) override;
        void StopSizing() override;
        void SetHitRegion() override;
        void MoveBy(POINT) override;
        void Draw() override;
    };
}

