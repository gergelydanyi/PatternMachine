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
	void On_WM_RBUTTONDOWN(LPARAM);
	void On_WM_RBUTTONUP(LPARAM);
	void On_WM_MOUSEMOVE(LPARAM);
	// This is only for DEBUG
	LPARAM lastlParam;
	// This is only for DEBUG
	LONG X();
	LONG Y();
	POINT CurrentPosition();
	POINT PreviousPosition();
	POINT MotionVector();
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
	bool leftButtonPressed = false;
	bool rightButtonPressed = false;
	void SetCoordinates(MouseCoordinates, LPARAM);
	POINT PointFromLParam(LPARAM);
};

