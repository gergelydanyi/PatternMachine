#pragma once
#include "Shape.h"

namespace PatternMachine
{
    class Canvas;
    class Rectangle :
        public Shape
    {
    public:
        Rectangle(Canvas*);
        Rectangle(HWND);
        Rectangle(Layer* pLayer);
        void StartSizing(POINT) override;
        void Sizing(POINT, POINT) override;
        void StopSizing() override;
        void SetHitRegion() override;
        void MoveBy(POINT) override;
        void Draw() override;
    };

}