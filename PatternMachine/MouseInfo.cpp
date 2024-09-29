#include "MouseInfo.h"
#include <math.h>

void MouseInfo::On_WM_LBUTTONDOWN(LPARAM lParam)
{
	SetCoordinates(LEFT_BUTTON_DOWN, lParam);
	leftButtonPressed = true;
}

void MouseInfo::On_WM_LBUTTONUP(LPARAM lParam)
{
	SetCoordinates(LEFT_BUTTON_UP, lParam);
	leftButtonPressed = false;
}

void MouseInfo::On_WM_RBUTTONDOWN(LPARAM lParam)
{
	SetCoordinates(RIGHT_BUTTON_DOWN, lParam);
	rightButtonPressed = true;
}

void MouseInfo::On_WM_RBUTTONUP(LPARAM lParam)
{
	SetCoordinates(RIGHT_BUTTON_UP, lParam);
	rightButtonPressed = false;
}

void MouseInfo::On_WM_MOUSEMOVE(LPARAM lParam)
{
	SetCoordinates(CURRENT_POSITION, lParam);
	lastlParam = lParam;
}

LONG MouseInfo::X()
{
	return GetCoordinates(CURRENT_POSITION).x;
}

LONG MouseInfo::Y()
{
	return GetCoordinates(CURRENT_POSITION).y;
}

POINT MouseInfo::CurrentPosition()
{
	return GetCoordinates(CURRENT_POSITION);
}

POINT MouseInfo::PreviousPosition()
{
	return GetCoordinates(PREVIOUS_POSITION);
}

POINT MouseInfo::MotionVector()
{
	return { GetCoordinates(CURRENT_POSITION).x - GetCoordinates(PREVIOUS_POSITION).x,
			 GetCoordinates(CURRENT_POSITION).y - GetCoordinates(PREVIOUS_POSITION).y };
}

POINT MouseInfo::LD()
{
	return GetCoordinates(LEFT_BUTTON_DOWN);
}

POINT MouseInfo::LU()
{
	return GetCoordinates(LEFT_BUTTON_UP);
}

POINT MouseInfo::RD()
{
	return GetCoordinates(RIGHT_BUTTON_DOWN);
}

POINT MouseInfo::RU()
{
	return GetCoordinates(RIGHT_BUTTON_UP);
}

POINT MouseInfo::MD()
{
	return GetCoordinates(MIDDLE_BUTTON_DOWN);
}

POINT MouseInfo::MU()
{
	return GetCoordinates(MIDDLE_BUTTON_UP);
}

bool MouseInfo::LeftButtonPressed()
{
	return leftButtonPressed;
}

bool MouseInfo::RightButtonPressed()
{
	return rightButtonPressed;
}

void MouseInfo::SetCoordinates(MouseCoordinates mouseCoordinates, LPARAM lParam)
{
	pt = PointFromLParam(lParam);
	switch (mouseCoordinates)
	{
	case LEFT_BUTTON_DOWN:
	case LEFT_BUTTON_UP:
	case RIGHT_BUTTON_DOWN:
	case RIGHT_BUTTON_UP:
	case MIDDLE_BUTTON_DOWN:
	case MIDDLE_BUTTON_UP:
		coordinates[mouseCoordinates] = PointFromLParam(lParam);
		break;
	case CURRENT_POSITION:
		coordinates[PREVIOUS_POSITION] = coordinates[CURRENT_POSITION];
		coordinates[CURRENT_POSITION] = PointFromLParam(lParam);
		break;
	}
}

POINT MouseInfo::GetCoordinates(MouseCoordinates mouseCoordinates)
{
	return coordinates[mouseCoordinates];
}

POINT MouseInfo::PointFromLParam(LPARAM lParam)
{
	return { (LONG)LOWORD(lParam), (LONG)HIWORD(lParam) };
}