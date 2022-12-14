#pragma once
#include<Windows.h>
#include<windowsx.h>
#include<vector>
#include"Identifiers.h"
#include"ColorPalette.h"
#include"ColorPaletteWndProc.h"
#include"CanvasWndProc.h"

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
	static HWND hwndColorPalette, hwndCanvas;
	HDC hdc;
	PAINTSTRUCT ps;
	static HWND fillOrNotCheckBox;
	static bool drawing = false;
	POINT p;
	switch (message)
	{
	case WM_CREATE:
		hwndColorPalette = CreateWindow(L"Color Palette", nullptr, WS_CHILD | WS_VISIBLE, 500, 0, 200, 100, hwnd, (HMENU)1000, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);
		hwndCanvas = CreateWindow(L"Canvas", nullptr, WS_CHILD | WS_VISIBLE, 5, 125, 1000, 600, hwnd, (HMENU)CANVAS, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);
		CreateWindow(L"BUTTON", L"Line", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 50, 50, hwnd, (HMENU)BTN_LINE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"Rectangle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 60, 10, 100, 50, hwnd, (HMENU)BTN_RECT, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"Ellipse", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 160, 10, 75, 50, hwnd, (HMENU)BTN_ELLIPSE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		fillOrNotCheckBox = CreateWindow(L"BUTTON", L"Fill?", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 250, 10, 50, 50, hwnd, (HMENU)BTN_FILLORNOT, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"Button", L"Square Brush", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 60, 100, 50, hwnd, (HMENU)BTN_SQBRUSH, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"Button", L"Circle Brush", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 110, 60, 100, 50, hwnd, (HMENU)BTN_CIRBRUSH, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 20, 50, 50, hwnd, (HMENU)BTN_DECREASESIZE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 350, 20, 50, 50, hwnd, (HMENU)BTN_INCREASESIZE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);

		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 320, 0, L"Size?", 5);
		EndPaint(hwnd ,&ps);
		return 0;
	case WM_SENDCOLORBRUSH:
		SendMessage(hwndCanvas, message, wparam, lparam);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case BTN_LINE:
			CanvasWndProc(hwndCanvas, message, wparam, lparam);
			break;
		case BTN_RECT:
			CanvasWndProc(hwndCanvas, message, wparam, lparam);
			break;
		case BTN_ELLIPSE:
			CanvasWndProc(hwndCanvas, message, wparam, lparam);
			break;
		case BTN_FILLORNOT:
			CanvasWndProc(hwndCanvas, message, wparam, lparam);
			break;
		case BTN_SQBRUSH:
			CanvasWndProc(hwndCanvas, message, wparam, lparam);
			break;
		case BTN_CIRBRUSH:
			CanvasWndProc(hwndCanvas, message, wparam, lparam);
			break;

		case BTN_INCREASESIZE:
			CanvasWndProc(hwndCanvas, message, wparam, lparam);
			break;
		case BTN_DECREASESIZE:
			CanvasWndProc(hwndCanvas, message, wparam, lparam);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}