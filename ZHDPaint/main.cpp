#include<Windows.h>
#include<vector>
#include"ColorPalette.h"

constexpr int WM_COLORBRUSH = 0x0801;
constexpr int WM_GETCOLORBRUSH = 0x0802;
constexpr int WM_SENDCOLORBRUSH = 0x0803;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK ColorPaletteWndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK PaletteBrushWndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	WNDCLASS wndclass = { };

	wndclass.lpszClassName = L"My Windows App";
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndclass.lpszMenuName = nullptr;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndclass);

	WNDCLASS colorPaletteWndclass = { };
	colorPaletteWndclass.lpszClassName = L"Color Palette";
	colorPaletteWndclass.lpfnWndProc = ColorPaletteWndProc;
	colorPaletteWndclass.hIcon = nullptr;

	RegisterClass(&colorPaletteWndclass);

	WNDCLASS paletteBrush = { };
	paletteBrush.lpszClassName = L"Palette Brush";
	paletteBrush.lpfnWndProc = PaletteBrushWndProc;
	paletteBrush.hIcon = nullptr;
	paletteBrush.cbWndExtra = sizeof(long);

	RegisterClass(&paletteBrush);

	HWND hwnd = CreateWindow(wndclass.lpszClassName, L"My Windows App", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

	

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

//COLORREF DetermineColor(HWND hwnd)
//{
//
//}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static HWND hwndColorPalette;
	HDC hdc;
	PAINTSTRUCT ps;
	HRGN hrgnColorPalette;
	static HPEN hpen;
	switch (message)
	{
	case WM_CREATE:
		hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		hdc = GetDC(hwnd);
		SelectObject(hdc, hpen);
		ReleaseDC(hwnd, hdc);
		hwndColorPalette = CreateWindow(L"Color Palette", nullptr, WS_CHILD | WS_VISIBLE, 500, 0, 200, 100, hwnd, (HMENU)1000, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), nullptr);
		/*hrgnColorPalette = CreateRectRgn(500, 0, 700, 100);
		SetWindowRgn(hwndColorPalette, hrgnColorPalette, true);*/
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd ,&ps);
		return 0;
	case WM_SENDCOLORBRUSH:
		switch (lparam)
		{
		case 0:
			hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			
			break;

		case 1:
			hpen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			break;
		case 2:
			hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
			break;
		}
		return 0;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		SelectObject(hdc, hpen);
		MoveToEx(hdc, LOWORD(lparam), HIWORD(lparam), nullptr);
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_LBUTTONUP:
		hdc = GetDC(hwnd);
		SelectObject(hdc, hpen);
		LineTo(hdc, LOWORD(lparam), HIWORD(lparam));
		ReleaseDC(hwnd, hdc);
		InvalidateRect(hwnd, nullptr, FALSE);
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
	static std::vector<HWND> palettebrushes;
	static std::vector<int> childwindowHMenu;
	int numberOfBrushes = 0;
	HDC hdc;
	HWND childwindowshwnd;
	PAINTSTRUCT ps;
	RECT rect;
	HRGN hrgnColorPalette;
	switch (message)
	{
	case WM_CREATE:
		for (PaletteBrush p : colorpalette.GetColors())
		{
			childwindowHMenu.push_back(1001 + numberOfBrushes);
			CreateWindow(L"Palette Brush", nullptr, WS_CHILD | WS_VISIBLE, 0 + 50 * numberOfBrushes, 0, 50, 50, hwnd, (HMENU)childwindowHMenu[numberOfBrushes], (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), nullptr);
			
			numberOfBrushes++;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	//case WM_LBUTTONDOWN:
	//	return DefWindowProc(hwnd, message, wparam, lparam);
	case WM_GETCOLORBRUSH:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}




LRESULT CALLBACK PaletteBrushWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static int rgbIndex = 0;
	HDC hdc;
	static HWND parentHwnd;
	static HWND parentparentHwnd;
	PAINTSTRUCT ps;
	HBRUSH hbrush;
	PaletteBrush palettebrush;
	RECT rect;
	switch (message)
	{
	case WM_CREATE:
		parentHwnd = GetParent(hwnd);
		SetWindowLongPtr(hwnd, 0, rgbIndex);
		rgbIndex++;
		return 0;
	case WM_LBUTTONDOWN:
		parentparentHwnd = GetParent(parentHwnd);
		SendMessage(parentparentHwnd, WM_SENDCOLORBRUSH, 0, GetWindowLongPtr(hwnd, 0));
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		switch (GetWindowLongPtr(hwnd, 0))
		{
		case 0:
			hbrush = CreateSolidBrush(RGB(255,0,0));
			break;
		case 1:
			hbrush = CreateSolidBrush(RGB(0, 255, 0));
			break;
		case 2:
			hbrush = CreateSolidBrush(RGB(0, 0, 255));
			break;
		default:
			hbrush = CreateSolidBrush(RGB(0, 0, 0));
			break;
		}
		SelectObject(hdc, hbrush);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		EndPaint(hwnd, &ps);
		return 0;
	case WM_COLORBRUSH:
		/*switch (lparam)
		{
		case 0:
			palettebrush.SetPaletteBrush(RGB(255, 0, 0));
			break;
		case 1:
			palettebrush.SetPaletteBrush(RGB(0, 255, 0));
			break;
		case 2:
			palettebrush.SetPaletteBrush(RGB(0, 0, 255));
			break;
		}*/
		return 0;
	}
}