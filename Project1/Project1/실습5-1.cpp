#include <windows.h> 
#include <tchar.h> 
#include <time.h> 
#include <stdbool.h> 
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"WindowClass";
LPCTSTR lpszWindowName = L"Random Text Program";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int winx = 700, winy = 700;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;

	g_hInst = hInstance;
	srand(unsigned int(time(NULL)));

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(
		lpszClass,
		lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0, winx, winy,
		NULL, NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hDC, mDC, hMemDC;
	static BITMAP bmp;
	static HBITMAP hBitmap;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	PAINTSTRUCT ps;
	RECT rect;
	static bool isA = false,isR=false;
	static int x, y, divide = 1,select2=-1,select4=-1,select6=-1;
	GetClientRect(hWnd, &rect);

	switch (iMessage)
	{
	case WM_CREATE:
	{
		hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(102));
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		x = bmp.bmWidth;
		y = bmp.bmHeight;
		break;
	}
	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);
		hMemDC = CreateCompatibleDC(hDC);
		SelectObject(hMemDC, hBitmap);
		hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(hDC, hPen);
		oldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		if (isA == true)
			StretchBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		else if (divide == 1)
			StretchBlt(hDC, 0, 0, x, y, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
		else if (divide == 2)
		{
			
			StretchBlt(hDC, 0, 0, rect.right / 2, rect.bottom, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			StretchBlt(hDC, rect.right / 2, 0, rect.right / 2, rect.bottom, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			if (select2 == 1)
			{
				Rectangle(hDC, 0, 0, rect.right / 2, rect.bottom);
				if(isR==true)
					StretchBlt(hDC, 0, 0, rect.right / 2, rect.bottom, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
			else if (select2 == 2)
			{
				Rectangle(hDC, rect.right/2, 0, rect.right, rect.bottom);
				if(isR==true)
					StretchBlt(hDC, rect.right / 2, 0, rect.right / 2, rect.bottom, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
				
		}
		else if (divide == 4)
		{
			StretchBlt(hDC, 0, 0, rect.right / 2, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			StretchBlt(hDC, rect.right / 2, 0, rect.right / 2, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			StretchBlt(hDC, 0, rect.bottom / 2, rect.right / 2, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			StretchBlt(hDC, rect.right / 2, rect.bottom / 2, rect.right / 2, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			if (select4 == 1)
			{
				Rectangle(hDC, 0, 0, rect.right / 2, rect.bottom/2);
				if (isR == true)
					StretchBlt(hDC, 0, 0, rect.right / 2, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
			else if (select4 == 2)
			{
				Rectangle(hDC, rect.right/2, 0, rect.right, rect.bottom / 2);
				if (isR == true)
					StretchBlt(hDC, rect.right / 2, 0, rect.right / 2, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
			else if (select4 == 3)
			{
				Rectangle(hDC, 0, rect.bottom/2, rect.right / 2, rect.bottom);
				if (isR == true)
					StretchBlt(hDC, 0, rect.bottom / 2, rect.right / 2, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
			else if (select4 == 4)
			{
				Rectangle(hDC, rect.right/2, rect.bottom/2, rect.right, rect.bottom);
				if (isR == true)
					StretchBlt(hDC, rect.right / 2, rect.bottom / 2, rect.right / 2, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
		}
		else if (divide == 6)
		{
			StretchBlt(hDC, 0, 0, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			StretchBlt(hDC, rect.right / 3, 0, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			StretchBlt(hDC, rect.right / 3 * 2, 0, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			StretchBlt(hDC, 0, rect.bottom / 2, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			StretchBlt(hDC, rect.right / 3, rect.bottom / 2, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			StretchBlt(hDC, rect.right / 3 * 2, rect.bottom / 2, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			if (select6 == 1)
			{
				Rectangle(hDC, 0, 0, rect.right/3, rect.bottom / 2);
				if (isR == true)
					StretchBlt(hDC, 0, 0, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
			else if (select6 == 2)
			{
				Rectangle(hDC, rect.right / 3, 0, rect.right / 3*2, rect.bottom / 2);
				if (isR == true)
					StretchBlt(hDC, rect.right / 3, 0, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
			else if (select6 == 3)
			{
				Rectangle(hDC, rect.right / 3 * 2, 0, rect.right, rect.bottom / 2);
				if (isR == true)
					StretchBlt(hDC, rect.right / 3 * 2, 0, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
			else if (select6 == 4)
			{
				Rectangle(hDC, 0, rect.bottom / 2, rect.right / 3, rect.bottom);
				if (isR == true)
					StretchBlt(hDC, 0, rect.bottom / 2, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
			else if (select6 == 5)
			{
				Rectangle(hDC, rect.right / 3, rect.bottom / 2, rect.right / 3 * 2, rect.bottom);
				if (isR == true)
					StretchBlt(hDC, rect.right / 3, rect.bottom / 2, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
			else if (select6 == 6)
			{
				Rectangle(hDC, rect.right / 3 * 2, rect.bottom / 2, rect.right, rect.bottom);
				if (isR == true)
					StretchBlt(hDC, rect.right / 3 * 2, rect.bottom / 2, rect.right / 3, rect.bottom / 2, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, NOTSRCCOPY);
			}
		}
		SelectObject(hDC, oldPen);
		SelectObject(hDC, oldBrush);
		DeleteObject(hPen);
		DeleteDC(hMemDC);

		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY:
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	case WM_CHAR:
	{
		if (wParam == '+')
		{
			x += 10;
			y += 10;
		}
		else if (wParam == '-')
		{
			x -= 10;
			y -= 10;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam == 'Q')
			PostQuitMessage(0);
		else if (wParam == 'A')
		{
			if (isA == true)
				isA = false;
			else if (isA == false)
			{
				isA = true;
				x = bmp.bmWidth;
				y = bmp.bmHeight;
			}
		}
		else if (wParam == '1')
		{
			divide = 1;
		}
		else if (wParam == '2')
		{
			divide = 2;
			select2 = -1;
		}
		else if (wParam == '4')
		{
			divide = 4;
		}
		else if (wParam == '6')
		{
			divide = 6;
		}
		else if (wParam == 'R')
		{
			if (divide == 2)
			{
				if (select2 == 1)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
				else if (select2 == 2)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
			}
			else if (divide == 4)
			{
				if (select4 == 1)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
				else if (select4 == 2)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
				else if (select4 == 3)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
				else if (select4 == 4)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
			}
			else if (divide == 6)
			{
				if (select6 == 1)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
				else if (select6 == 2)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
				else if (select6 == 3)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
				else if (select6 == 4)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
				else if (select6 == 5)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
				else if (select6 == 6)
				{
					if (isR == false)
						isR = true;
					else
						isR = false;
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (divide == 2)
		{
			if (x <= rect.right / 2)
				select2 = 1;
			else
				select2 = 2;
		}
		else if (divide == 4)
		{
			if (x <= rect.right / 2 && y <= rect.bottom / 2)
				select4 = 1;
			else if ((x > rect.right / 2 && y <= rect.bottom / 2))
				select4 = 2;
			else if ((x <= rect.right / 2 && y > rect.bottom / 2))
				select4 = 3;
			else if ((x > rect.right / 2 && y > rect.bottom /2))
				select4 = 4;
		}
		else if (divide == 6)
		{
			if (x <= rect.right / 3 && y <= rect.bottom / 2)
				select6 = 1;
			else if ((x > rect.right / 3 && x <= rect.right / 3 * 2 && y <= rect.bottom / 2))
				select6 = 2;
			else if (x > rect.right / 3 * 2 && y <= rect.bottom / 2)
				select6 = 3;
			else if (x <= rect.right / 3 && y > rect.bottom / 2)
				select6 = 4;
			else if ((x > rect.right / 3 && x <= rect.right / 3 * 2 && y > rect.bottom / 2))
				select6 = 5;
			else if (x > rect.right / 3 * 2 && y > rect.bottom / 2)
				select6 = 6;
		}
		break;
	}
	case WM_MOUSEMOVE:
	{

		break;
	}
	case WM_LBUTTONUP:

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
