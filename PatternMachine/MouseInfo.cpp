#include "MouseInfo.h"

void MouseInfo::On_WM_LBUTTONDOWN(LPARAM lParam)
{
	SetCoordinates(LEFT_BUTTON_DOWN, lParam);
}

void MouseInfo::On_WM_LBUTTONUP(LPARAM lParam)
{
	SetCoordinates(LEFT_BUTTON_UP, lParam);
}

LONG MouseInfo::X()
{
	return GetCoordinates(LEFT_BUTTON_DOWN).x;
}

LONG MouseInfo::Y()
{
	return GetCoordinates(LEFT_BUTTON_DOWN).y;
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