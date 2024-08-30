#pragma once
#include "framework.h"

enum MouseCoordinates
{
	CURRENT_POSITION = 0,
	PREVIOUS_POSITION = 1,
	LEFT_BUTTON_DOWN = 2,
	LEFT_BUTTON_UP = 3,
	RIGHT_BUTTON_DOWN = 4,
	RIGHT_BUTTON_UP = 5,
	MIDDLE_BUTTON_DOWN = 6,
	MIDDLE_BUTTON_UP = 7
};

class MouseInfo
{
public:
	void On_WM_LBUTTONDOWN(LPARAM);
	void On_WM_LBUTTONUP(LPARAM);
	LONG X();
	LONG Y();
	POINT LD();
	POINT LU();
	POINT RD();
	POINT RU();
	POINT MD();
	POINT MU();
	bool LeftButtonPressed();
	bool RightButtonPressed();
	POINT GetCoordinates(MouseCoordinates);
private:
	POINT pt;
	POINT coordinates[8] = { -1 };
	void SetCoordinates(MouseCoordinates, LPARAM);
	POINT PointFromLParam(LPARAM);
};

