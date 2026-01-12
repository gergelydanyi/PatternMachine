#pragma once
#include "framework.h"
#include "MouseInfo.h"
#include "Shape.h"
#include "Layer.h"
#include <vector>

namespace PatternMachine
{

	enum CanvasBehaviour {
		PointingSelection,
		FramingSelection,
		Drawing
	};

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
		// TODO: introduce CanvasOperation class that derives operations which responsible for controlling selecting, moving, editing and all other operations that occurs on Canvas
		bool editingMode = false;
		bool selectionMode = false;
		bool movingMode = false;
		bool copyMode = false;
		CanvasBehaviour behaviour = Drawing;
		RECT selectorRect = { 0, 0, 0, 0 };
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
		void ChangeBehaviour(CanvasBehaviour);
		void SelectHighlightedShapes(bool CtrlPressed);
		void DeleteSelection();
		std::vector<Layer*> layers;

	private:
		Layer* pStage{};
		Layer* pStorage{};
		Layer* pDrawing{};
		Layer* pSelector{};

		void DrawShape();
		void DrawSelectedShapes();
		void DrawSelectorRectangle();
		void DrawHitRegion();
	};

}
