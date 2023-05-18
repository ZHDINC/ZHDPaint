#pragma once
#include<Windows.h>
#include"Identifiers.h"

LRESULT CALLBACK CanvasWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static HDC hdcInMemory;
	HPEN hpen;
	HBRUSH hbrush;
	static int penStyle;
	static COLORREF currentColor;
	static HBITMAP hbitmap;
	BITMAP bitmap;
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
	static RECT oldRect;
	static int currentX, currentY;
	static bool firstPaint = true;
	switch (message)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &oldRect);
		currentColor = RGB(255, 0, 0);
		SendMessage(GetParent(hwnd), WM_BRUSHSIZECHANGE, 0, brushSize);
		return 0;
	case WM_MOUSEMOVE:
		SendMessage(GetParent(hwnd), WM_CANVASMOVE, 0, lparam);
		if (!drawing) return 0;
		hdc = GetDC(hwnd);
		if (wparam & MK_LBUTTON)
		{
			p.x = LOWORD(lparam);
			p.y = HIWORD(lparam);
			hpen = CreatePen(penStyle, brushSize, currentColor);
			hbrush = CreateSolidBrush(currentColor);
			SelectObject(hdc, hpen);
			SelectObject(hdc, hbrush);
			switch (currentTool)
			{
			case 3:
				drawingBrush.left = p.x;
				drawingBrush.top = p.y;
				drawingBrush.right = p.x + brushSize;
				drawingBrush.bottom = p.y + brushSize;
				Rectangle(hdc, drawingBrush.left, drawingBrush.top, drawingBrush.right, drawingBrush.bottom);
				break;
			case 4:
				Ellipse(hdc, p.x, p.y, p.x + brushSize + 1, p.y + brushSize + 1);
				break;
			}
			DeleteObject(hpen);
			DeleteObject(hbrush);
		}
		BitBlt(hdcInMemory, 0, 0, canvasWidth, canvasHeight, hdc, 0, 0, SRCCOPY);
		
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_SIZE:
		canvasWidth = LOWORD(lparam);
		canvasHeight = HIWORD(lparam);
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		if (oldRect.bottom != rect.bottom || oldRect.right != rect.right)
		{
			InvalidateRect(hwnd, &rect, TRUE);
			hbitmap = CreateCompatibleBitmap(hdc, canvasWidth, canvasHeight);
			BitBlt(hdcInMemory, 0, 0, canvasWidth, canvasHeight, hdc, 0, 0, SRCCOPY);
			oldRect = rect;
		}
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		BitBlt(hdcInMemory, 0, 0, canvasWidth, canvasHeight, hdc, 0, 0, SRCCOPY);
		GetClientRect(hwnd, &rect);
		FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		if (firstPaint)
		{
			hdcInMemory = CreateCompatibleDC(hdc);
			hbitmap = CreateCompatibleBitmap(hdc, canvasWidth, canvasHeight);
			BitBlt(hdcInMemory, 0, 0, canvasWidth, canvasHeight, hdc, 0, 0, SRCCOPY);
			firstPaint = false;
		}
		BitBlt(hdc, 0, 0, canvasWidth, canvasHeight, hdcInMemory, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		currentX = LOWORD(lparam);
		currentY = HIWORD(lparam);
		return 0;
	case WM_LBUTTONUP:
		SelectObject(hdcInMemory, hbitmap);
		hdc = GetDC(hwnd);
		if (fillOrNot)
		{
			SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		}
		else
		{
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
		}
		MoveToEx(hdc, currentX, currentY, nullptr);
		hpen = CreatePen(penStyle, brushSize, currentColor);
		SelectObject(hdc, hpen);
		switch (currentTool)
		{
		case 0:
			LineTo(hdc, LOWORD(lparam), HIWORD(lparam));
			break;
		case 1:
			Rectangle(hdc, currentX, currentY, LOWORD(lparam), HIWORD(lparam));
			break;
		case 2:
			Ellipse(hdc, currentX, currentY, LOWORD(lparam), HIWORD(lparam));
			break;
		}
		BitBlt(hdcInMemory, 0, 0, canvasWidth, canvasHeight, hdc, 0, 0, SRCCOPY);
		DeleteObject(hpen);
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
					hdc = GetDC(hwnd);
					SelectObject(hdcInMemory, (HBITMAP)LoadImage(nullptr, ofn.lpstrFile, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE));
					BitBlt(hdc, 0, 0, canvasWidth, canvasHeight, hdcInMemory, 0, 0, SRCCOPY);
					ReleaseDC(hwnd, hdc);
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
		DeleteObject(hdcInMemory);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}