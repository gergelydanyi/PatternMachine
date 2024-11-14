#pragma once
#include "framework.h"
#include <vector>

namespace PatternMachine
{

class Shape
{
public:
	POINT anchor;
	RECT rect = { 0, 0, 100, 100 };
	virtual RECT BoundingRectangle();
	virtual bool isEditing();
	virtual void StartSizing(POINT);
	virtual void Sizing(POINT, POINT);
	virtual void StopSizing();
	bool isSizing = false;
	bool isMoving = false;
	bool isDrawn = true;
private:
	std::vector<POINT> vertices;
	POINT topmostVertex;
	POINT rightmostVertex;
	POINT bottommostVertex;
	POINT leftmostVertex;
};

}

