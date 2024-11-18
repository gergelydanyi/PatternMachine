#pragma once
#include "framework.h"
#include "Tool.h"

namespace PatternMachine
{

	class Pen : public Tool
	{
	public:
		COLORREF color;
		int width;
		int style;
		HPEN handler;
	};

}