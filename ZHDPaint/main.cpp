#pragma once
#include<Windows.h>
#include<windowsx.h>
#include<vector>
#include"Identifiers.h"
#include"ColorPalette.h"
#include"ColorPaletteWndProc.h"
#include"CanvasWndProc.h"
#include<string>
#include<exception>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
	WNDCLASS wndclass = { };
	wndclass.lpszClassName = L"ZHD Paint";
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.lpszMenuName = nullptr;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndclass);

	WNDCLASS colorPaletteWndclass = { };
	colorPaletteWndclass.lpszClassName = L"Color Palette";
	colorPaletteWndclass.lpfnWndProc = ColorPaletteWndProc;
	colorPaletteWndclass.hIcon = nullptr;

	RegisterClass(&colorPaletteWndclass);

	WNDCLASS canvasWndclass = { };
	canvasWndclass.lpszClassName = L"Canvas";
	canvasWndclass.lpfnWndProc = CanvasWndProc;
	canvasWndclass.hIcon = nullptr;

	RegisterClass(&canvasWndclass);

	HWND hwnd = CreateWindow(wndclass.lpszClassName, L"ZHD Paint", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

	

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static HWND hwndColorPalette, hwndCanvas, fillOrNotCheckBox, hwndXSize, hwndYSize;
	HDC hdc;
	PAINTSTRUCT ps;
	static int canvasX = 0, canvasY = 0, canvasWidth = 1000, canvasHeight = 600, newWidth = 0, newHeight = 0;
	static RECT coordinatesRect;
	static std::wstring coordinatesString;
	static RECT canvasRect;
	static bool newSize = false;
	wchar_t sXBuffer[20], sYBuffer[20];
	int countXBuffer, countYBuffer;
	std::wstring textXSize, textYSize;
	LPARAM resizingLparam;
	switch (message)
	{
	case WM_CREATE:
		hwndColorPalette = CreateWindow(L"Color Palette", nullptr, WS_CHILD | WS_VISIBLE, 500, 0, 200, 100, hwnd, (HMENU)1000, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);
		hwndCanvas = CreateWindow(L"Canvas", nullptr, WS_CHILD | WS_VISIBLE, 5, 125, canvasWidth, canvasHeight, hwnd, (HMENU)CANVAS, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);
		CreateWindow(L"BUTTON", L"Line", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 50, 50, hwnd, (HMENU)BTN_LINE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"Rectangle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 60, 10, 100, 50, hwnd, (HMENU)BTN_RECT, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"Ellipse", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 160, 10, 75, 50, hwnd, (HMENU)BTN_ELLIPSE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		fillOrNotCheckBox = CreateWindow(L"BUTTON", L"Fill?", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 250, 10, 50, 50, hwnd, (HMENU)BTN_FILLORNOT, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"Button", L"Square Brush", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 60, 100, 50, hwnd, (HMENU)BTN_SQBRUSH, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"Button", L"Circle Brush", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 110, 60, 100, 50, hwnd, (HMENU)BTN_CIRBRUSH, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 20, 50, 50, hwnd, (HMENU)BTN_DECREASESIZE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 350, 20, 50, 50, hwnd, (HMENU)BTN_INCREASESIZE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		hwndXSize = CreateWindow(L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | ES_LEFT, 800, 40, 50, 20, hwnd, (HMENU)ES_XSIZE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		hwndYSize = CreateWindow(L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | ES_LEFT, 870, 40, 50, 20, hwnd, (HMENU)ES_YSIZE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"Resize", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 920, 40, 50, 20, hwnd, (HMENU)BTN_RESIZE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		coordinatesRect = { 800, 20, 800 + 100, 20 + 20 };
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 320, 0, L"Size?", 5);
		coordinatesString = L"(" + std::to_wstring(canvasX) + L", " + std::to_wstring(canvasY) + L")";
		TextOut(hdc, 800, 20, coordinatesString.c_str(), coordinatesString.size());
		TextOut(hdc, 780, 40, L"X:", 2);
		TextOut(hdc, 850, 40, L"Y:", 2);
		EndPaint(hwnd ,&ps);
		return 0;
	case WM_SENDCOLORBRUSH:
		CanvasWndProc(hwndCanvas, message, wparam, lparam);
		return 0;
	case WM_MOUSEMOVE:
		if (wparam & MK_LBUTTON)
		{
			GetClientRect(hwndCanvas, &canvasRect);
			newWidth = LOWORD(lparam) + 5;
			newHeight = HIWORD(lparam) + 125;
			if (LOWORD(lparam) == canvasRect.right + 1)
			{
				newSize = true;
			}
			if (HIWORD(lparam) == canvasRect.bottom + 1)
			{
				newSize = true;
			}
		}
		return 0;
	case WM_LBUTTONUP:
		SendMessage(hwndCanvas, WM_SIZE, 0, newWidth & newHeight);
		newSize = false;
		return 0;
	case WM_CANVASMOVE:
		canvasX = LOWORD(lparam);
		canvasY = HIWORD(lparam);
		InvalidateRect(hwnd, &coordinatesRect, true);
		return 0;
	case WM_COMMAND:
		if (wparam == BTN_RESIZE)
		{
			countXBuffer = SendMessage(hwndXSize, EM_GETLINE, 0, (LPARAM)sXBuffer);
			countYBuffer = SendMessage(hwndYSize, EM_GETLINE, 0, (LPARAM)sYBuffer);
			if (countXBuffer && countYBuffer)
			{
				for (int i = 0; i < countXBuffer; i++)
				{
					textXSize += sXBuffer[i];
				}
				for (int i = 0; i < countYBuffer; i++)
				{
					textYSize += sYBuffer[i];
				}
				newWidth = std::stoi(textXSize);
				newHeight = std::stoi(textYSize);
				MoveWindow(hwndCanvas, 5, 125, newWidth, newHeight, true);
			}
		}
		if (wparam == BTN_FILLORNOT)
		{
			if (!Button_GetCheck(fillOrNotCheckBox))
			{
				Button_SetCheck(fillOrNotCheckBox, TRUE);
			}
			else
			{
				Button_SetCheck(fillOrNotCheckBox, FALSE);
			}
		}
		CanvasWndProc(hwndCanvas, message, wparam, lparam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}