#pragma once
#include<Windows.h>
#include<vector>

struct Point
{
	int x, y;
	Point(int x, int y) : x{ x }, y{ y } { };
};

struct PointsAndFunction
{
	Point p1, p2;
	int function;
	COLORREF color;
	PointsAndFunction(Point p1, Point p2, int function, COLORREF color) : p1{ p1 }, p2{ p2 }, function{ function }, color{ color } { };
};

class DrawingList
{
	HPEN hpen;
	HBRUSH hbrush;
	std::vector<PointsAndFunction> list;
public:
	DrawingList() { };
	void addDrawingOperation(int x0, int y0, int x1, int y1, int function, COLORREF color)
	{
		list.push_back(PointsAndFunction(Point(x0, y0), Point(x1, y1), function, color));
	}
	void runDrawingOperations(HDC& hdc)
	{
		for (PointsAndFunction p : list)
		{
			hpen = CreatePen(PS_SOLID, 1, p.color);
			hbrush = CreateSolidBrush(p.color);
			SelectObject(hdc, hpen);
			SelectObject(hdc, hbrush);
			switch (p.function)
			{
			case 0:
				MoveToEx(hdc, p.p1.x, p.p1.y, nullptr);
				LineTo(hdc, p.p2.x, p.p2.y);
				break;
			case 1:
				Rectangle(hdc, p.p1.x, p.p1.y, p.p2.x, p.p2.y);
				break;
			case 2:
				Ellipse(hdc, p.p1.x, p.p1.y, p.p2.x, p.p2.y);
				break;
			case 3:
				Rectangle(hdc, p.p1.x, p.p1.y, p.p2.x, p.p2.y);
				break;
			case 4:
				Ellipse(hdc, p.p1.x, p.p1.y, p.p2.x, p.p2.y);
				break;
			}
			DeleteObject(hbrush);
			DeleteObject(hpen);
		}
	}
};

//case BTN_LINE:
//	drawing = false;
//	currentTool = 0;
//	break;
//case BTN_RECT:
//	drawing = false;
//	currentTool = 1;
//	break;
//case BTN_ELLIPSE:
//	drawing = false;
//	currentTool = 2;
//	break;
//case BTN_FILLORNOT:
//	fillOrNot = !fillOrNot;
//	break;
//case BTN_SQBRUSH:
//	drawing = true;
//	currentTool = 3;
//	break;
//case BTN_CIRBRUSH:
//	drawing = true;
//	currentTool = 4;