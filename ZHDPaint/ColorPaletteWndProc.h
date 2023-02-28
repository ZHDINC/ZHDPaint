#pragma once
#include<Windows.h>
#include"ColorPalette.h"
#include"Identifiers.h"

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
		/*for (PaletteBrush p : colorpalette.GetColors())
		{
			hbrush = CreateSolidBrush(p.GetPaletteBrush());
			SelectObject(hdc, hbrush);
			Rectangle(hdc, rect.left + 50 * numberOfBrushes, rect.top, rect.right, rect.bottom);
			numberOfBrushes++;
		}*/
		colorpalette.DrawColorPalette(hdc);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		parentWnd = GetParent(hwnd);
		SendMessage(parentWnd, WM_SENDCOLORBRUSH, 0, colorpalette.HitCheck(lparam).GetPaletteBrush());
		return 0;
	case WM_GETCOLORBRUSH:
		parentWnd = GetParent(hwnd);
		SendMessage(parentWnd, WM_SENDCOLORBRUSH, 0, colorpalette.GetLastColor());
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}