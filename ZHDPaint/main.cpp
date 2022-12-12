#include<Windows.h>
#include<windowsx.h>
#include<vector>
#include"ColorPalette.h"

constexpr int WM_COLORBRUSH = 0x0801,
 			WM_GETCOLORBRUSH = 0x0802,
 			WM_SENDCOLORBRUSH = 0x0803,
 			BTN_LINE = 1050,
 			BTN_RECT = 1051,
 			BTN_ELLIPSE = 1052,
 			BTN_SQBRUSH = 1055,
 			BTN_CIRBRUSH = 1056,
 			BTN_FILLORNOT = 1500,
 			BTN_DECREASESIZE = 1053,
 			BTN_INCREASESIZE = 1054;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK ColorPaletteWndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
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

bool DetermineIfValidDrawCall(LPARAM lparam, RECT rect, int brushSize)
{
	if (!((LOWORD(lparam) > rect.left) && ((LOWORD(lparam) + brushSize) < rect.right))) return false;
	if (!((HIWORD(lparam) > rect.top) && ((HIWORD(lparam) + brushSize) < rect.bottom))) return false;
	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static HWND hwndColorPalette;
	HDC hdc;
	PAINTSTRUCT ps;
	static constexpr int drawingClientRectTopBorder = 125, drawingClientRectLeftBorder = 5;
	static int drawingClientRectRightBorder, drawingClientRectBottomBorder;
	static HWND fillOrNotCheckBox;
	static HPEN hpen;
	static HBRUSH hbrush;
	static int currentX, currentY;
	static int currentTool = 0;
	static bool fillOrNot = false;
	static int brushSize = 1;
	static bool drawing = false;
	RECT drawingBrush;
	static RECT drawingClientRect;
	POINT p;
	switch (message)
	{
	case WM_CREATE:
		currentX = drawingClientRectLeftBorder;
		currentY = drawingClientRectTopBorder;
		hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		hbrush = CreateSolidBrush(RGB(255, 0, 0));
		hdc = GetDC(hwnd);
		SelectObject(hdc, hpen);
		ReleaseDC(hwnd, hdc);
		hwndColorPalette = CreateWindow(L"Color Palette", nullptr, WS_CHILD | WS_VISIBLE, 500, 0, 200, 100, hwnd, (HMENU)1000, (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE), nullptr);
		CreateWindow(L"BUTTON", L"Line", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 10, 50, 50, hwnd, (HMENU)BTN_LINE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"Rectangle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 60, 10, 100, 50, hwnd, (HMENU)BTN_RECT, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"Ellipse", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 160, 10, 75, 50, hwnd, (HMENU)BTN_ELLIPSE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		fillOrNotCheckBox = CreateWindow(L"BUTTON", L"Fill?", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 250, 10, 50, 50, hwnd, (HMENU)BTN_FILLORNOT, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"Button", L"Square Brush", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 60, 100, 50, hwnd, (HMENU)BTN_SQBRUSH, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"Button", L"Circle Brush", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 110, 60, 100, 50, hwnd, (HMENU)BTN_CIRBRUSH, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 20, 50, 50, hwnd, (HMENU)BTN_DECREASESIZE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);
		CreateWindow(L"BUTTON", L"+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 350, 20, 50, 50, hwnd, (HMENU)BTN_INCREASESIZE, ((LPCREATESTRUCT)lparam)->hInstance, nullptr);

		return 0;
	case WM_SIZE:
		drawingClientRectRightBorder = LOWORD(lparam) - 5;
		drawingClientRectBottomBorder = HIWORD(lparam) - 5;
		drawingClientRect.top = drawingClientRectTopBorder;
		drawingClientRect.left = drawingClientRectLeftBorder;
		drawingClientRect.right = drawingClientRectRightBorder;
		drawingClientRect.bottom = drawingClientRectBottomBorder;
		hdc = GetDC(hwnd);
		FillRect(hdc, &drawingClientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 320, 0, L"Size?", 5);
		EndPaint(hwnd ,&ps);
		return 0;
	case WM_SENDCOLORBRUSH:
		hpen = CreatePen(PS_SOLID, brushSize, lparam);
		hbrush = CreateSolidBrush(lparam);
		return 0;
	case WM_LBUTTONDOWN:
		if(!DetermineIfValidDrawCall(lparam, drawingClientRect, brushSize)) return 0;
		currentX = LOWORD(lparam);
		currentY = HIWORD(lparam);
		return 0;
	case WM_MOUSEMOVE:
		if (!DetermineIfValidDrawCall(lparam, drawingClientRect, brushSize)) return 0;
		if (!drawing) return 0;
		hdc = GetDC(hwnd);
		if (wparam & MK_LBUTTON)
		{
			p.x = LOWORD(lparam);
			p.y = HIWORD(lparam);
			switch (currentTool)
			{
			case 3:
				drawingBrush.left = p.x;
				drawingBrush.top = p.y;
				drawingBrush.right = p.x + brushSize;
				drawingBrush.bottom = p.y + brushSize;
				FillRect(hdc, &drawingBrush, hbrush);
				break;
			case 4:
				SelectObject(hdc, hpen);
				SelectObject(hdc, hbrush);
				Ellipse(hdc, p.x, p.y, p.x + brushSize + 1, p.y + brushSize + 1);
				break;
			}
		}
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_LBUTTONUP:
		if (!DetermineIfValidDrawCall(lparam, drawingClientRect, brushSize)) return 0;
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
		case 3:
			break;
		}
		ReleaseDC(hwnd, hdc);
		InvalidateRect(hwnd, nullptr, FALSE);
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
			if (!Button_GetCheck(fillOrNotCheckBox))
			{
				fillOrNot = true;
				Button_SetCheck(fillOrNotCheckBox, TRUE);
			}
			else
			{
				fillOrNot = false;
				Button_SetCheck(fillOrNotCheckBox, FALSE);
			}
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
			break;
		case BTN_DECREASESIZE:
			brushSize > 1 ? brushSize-- : brushSize = 1;
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}



LRESULT CALLBACK ColorPaletteWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static ColorPalette colorpalette;
	int xValue = 0;
	int numberOfColors = 0;
	int numberOfBrushes = 0;
	HDC hdc;
	HWND childwindowshwnd;
	HWND parentWnd;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH hbrush;
	switch (message)
	{
	case WM_PAINT:
		numberOfBrushes = 0;
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
		numberOfColors = colorpalette.GetColors().size();
		for (PaletteBrush p : colorpalette.GetColors())
		{
			hbrush = CreateSolidBrush(p.GetPaletteBrush());
			SelectObject(hdc, hbrush);
			Rectangle(hdc, rect.left + 50 * numberOfBrushes, rect.top, rect.right, rect.bottom);
			numberOfBrushes++;
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		parentWnd = GetParent(hwnd);
		xValue = LOWORD(lparam);
		if (xValue < 50)
		{
			SendMessage(parentWnd, WM_SENDCOLORBRUSH, 0, colorpalette.GetSpecificIndex(0));
		}
		else if (xValue > 50 && xValue < 100)
		{
			SendMessage(parentWnd, WM_SENDCOLORBRUSH, 0, colorpalette.GetSpecificIndex(1));
		}
		else if (xValue > 100)
		{
			SendMessage(parentWnd, WM_SENDCOLORBRUSH, 0, colorpalette.GetSpecificIndex(2));
		}
		return 0;
	case WM_GETCOLORBRUSH:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}