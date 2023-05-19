#pragma once
#include<Windows.h>
#include"DrawingList.h"
#include"Identifiers.h"

LRESULT CALLBACK CanvasWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static DrawingList listOfGDIObjectsToDraw;
	HPEN hpen;
	HBRUSH hbrush;
	static int penStyle;
	static COLORREF currentColor;
	HDC hdc;
	PAINTSTRUCT ps;
	static bool drawing = false;
	POINT p;
	static int currentTool = 0;
	static bool fillOrNot = false;
	static int brushSize = 1;
	static HWND fillOrNotCheckBox;
	static int canvasWidth = 1000, canvasHeight = 600;
	RECT drawingBrush;
	RECT rect;
	static int currentX, currentY;
	static bool firstPaint = true;
	switch (message)
	{
	case WM_CREATE:
		currentColor = RGB(255, 0, 0);
		SendMessage(GetParent(hwnd), WM_BRUSHSIZECHANGE, 0, brushSize);
		return 0;
	case WM_MOUSEMOVE:
		SendMessage(GetParent(hwnd), WM_CANVASMOVE, 0, lparam);
		if (!drawing) return 0;
		if (wparam & MK_LBUTTON)
		{
			p.x = LOWORD(lparam);
			p.y = HIWORD(lparam);
			drawingBrush.left = p.x;
			drawingBrush.top = p.y;
			drawingBrush.right = p.x + brushSize;
			drawingBrush.bottom = p.y + brushSize;
			switch (currentTool)
			{
			case 3:
				listOfGDIObjectsToDraw.addDrawingOperation(drawingBrush.left, drawingBrush.top, drawingBrush.right, drawingBrush.bottom, 3, currentColor);
				break;
			case 4:
				listOfGDIObjectsToDraw.addDrawingOperation(drawingBrush.left, drawingBrush.top, drawingBrush.right, drawingBrush.bottom, 4, currentColor);
				break;
			}
			InvalidateRect(hwnd, 0, FALSE);
		}
		return 0;
	case WM_SIZE:
		canvasWidth = LOWORD(lparam);
		canvasHeight = HIWORD(lparam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		listOfGDIObjectsToDraw.runDrawingOperations(hdc);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		currentX = LOWORD(lparam);
		currentY = HIWORD(lparam);
		return 0;
	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);
		if (fillOrNot)
		{
			SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		}
		else
		{
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
		}
		switch (currentTool)
		{
		case 0:
			listOfGDIObjectsToDraw.addDrawingOperation(currentX, currentY, LOWORD(lparam), HIWORD(lparam), 0, currentColor);
			break;
		case 1:
			listOfGDIObjectsToDraw.addDrawingOperation(currentX, currentY, LOWORD(lparam), HIWORD(lparam), 1, currentColor);
			break;
		case 2:
			listOfGDIObjectsToDraw.addDrawingOperation(currentX, currentY, LOWORD(lparam), HIWORD(lparam), 2, currentColor);
			break;
		}
		InvalidateRect(hwnd, 0, FALSE);
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_SENDCOLORBRUSH:
		currentColor = lparam;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case BTN_LINE:
			drawing = false;
			currentTool = 0;
			break;
		case BTN_RECT:
			drawing = false;
			currentTool = 1;
			break;
		case BTN_ELLIPSE:
			drawing = false;
			currentTool = 2;
			break;
		case BTN_FILLORNOT:
			fillOrNot = !fillOrNot;
			break;
		case BTN_SQBRUSH:
			drawing = true;
			currentTool = 3;
			break;
		case BTN_CIRBRUSH:
			drawing = true;
			currentTool = 4;
			break;

		case BTN_INCREASESIZE:
			brushSize++;
			SendMessage(GetParent(hwnd), WM_BRUSHSIZECHANGE, 0, brushSize);
			break;
		case BTN_DECREASESIZE:
			brushSize > 1 ? brushSize-- : brushSize = 1;
			SendMessage(GetParent(hwnd), WM_BRUSHSIZECHANGE, 0, brushSize);
			break;
		case BTN_LOAD:
			{
				char filestring[256];
				OPENFILENAME ofn = { };
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFile = (LPWSTR)filestring;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(filestring);
				ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
				bool loadFileResult = GetOpenFileName(&ofn);
				if (loadFileResult)
				{
					
				}
				break;
			}
		case BTN_SAVE:
			{
				char filestring[256];
				OPENFILENAME sfn = { };
				sfn.lStructSize = sizeof(OPENFILENAME);
				sfn.hwndOwner = hwnd;
				sfn.lpstrFile = (LPWSTR)filestring;
				sfn.lpstrFile[0] = '\0';
				sfn.nMaxFile = sizeof(filestring);
				bool saveFileResult = GetSaveFileName(&sfn);
				if (saveFileResult)
				{
					// HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);

				}
				break;
			}
		}
		SendMessage(GetParent(hwnd), WM_GETCOLORBRUSH, 0, 0);
		return 0;
	case WM_DESTROY:
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}