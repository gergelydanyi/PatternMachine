#pragma once
#include "framework.h"
#include "MouseInfo.h"
#include "Rectangle.h"
#include "Shape.h"
#include "Layer.h"
#include "Line.h"
#include <vector>

namespace PatternMachine
{

	class Canvas
	{
	public:
		MouseInfo mouse;
		HWND hWindow{};

		HPEN activePen{};
		HBRUSH activeBrush{};

		COLORREF penColor;
		int penWidth;
		int penStyle;
		COLORREF brushColor;

		std::vector<Shape*> shapes;
		std::vector<Shape*> selectedShapes;
		Shape* pActiveShape;
		ShapeType selectedShapeType = LineShapeType;
		bool editingMode = false;
		bool selectionMode = true;
		bool movingMode = false;

		Canvas(HWND, HINSTANCE);
		void SetupLayers();
		void On_WM_PAINT(WPARAM, LPARAM);
		void On_WM_LBUTTONDOWN(WPARAM, LPARAM);
		void On_WM_LBUTTONUP(WPARAM, LPARAM);
		void On_WM_MOUSEMOVE(WPARAM, LPARAM);
		void SetActivePen();
		void SetActiveBrush();
		Shape& ActiveShape();
		void NewShape();
		void SelectHighlightedShapes();

	private:
		std::vector<Layer*> layers;
		Layer* pStage{};
		Layer* pStorage{};
		Layer* pDrawing{};

		void DrawShape();
		void DrawSelectedShapes();
		void DrawHitRegion();
	};

}
