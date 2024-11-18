#pragma once
#include "framework.h"

namespace PatternMachine
{

	class Pen
	{
	public:
		COLORREF color;
		int width;
		int style;
		HPEN handler;
	};

}