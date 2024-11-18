#pragma once
#include "framework.h"
#include "Tool.h"

namespace PatternMachine
{

	class Brush : public Tool
	{
		COLORREF color;
		HBRUSH handler;
	};

}