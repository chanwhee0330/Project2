#include <windows.h>		//--- 윈도우 헤더 파일
#include <tchar.h>
#include <time.h>
#include "resource.h"

#define marginX 16
#define marginY 39



HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"5-3";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


int  WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_  LPSTR lpszCmdParam, _In_  int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	//--- 크기 변경 가능 (기존 (1024, 768))
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 900 + marginX, 900 + marginY, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

struct Photo {
	int x, y;
};
struct Photo photo[5];
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, hMemDC,mDC;
	static HBITMAP hBitmap;
	RECT rc;
	static int startX, startY, endX, endY;
	static int left, top, right, bottom, width, height, bmpX, bmpY, bmpW, bmpH;
	static bool isDrag = false,mode=false,isF=false,isI=false,isH=false,isV=false;
	static int size = 0,pCount=0;
	GetClientRect(hWnd,&rc);
	switch (iMessage)
	{
		case WM_CREATE:
		{
			srand((unsigned)time(NULL));
			hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			break;
		}
		case WM_PAINT:
		{
			hDC = BeginPaint(hWnd, &ps);
			mDC = CreateCompatibleDC(hDC);

			HBITMAP hBackBitmap = CreateCompatibleBitmap(hDC, rc.right, rc.bottom);
			HBITMAP oldBack = (HBITMAP)SelectObject(mDC, hBackBitmap);
			hMemDC = CreateCompatibleDC(hDC);
			HBITMAP oldBmp = (HBITMAP)SelectObject(hMemDC, hBitmap);

			StretchBlt(mDC, 0, 0, rc.right, rc.bottom,hMemDC, 0, 0, 200, 200, SRCCOPY);

			if (isDrag)
			{
				HPEN pen = CreatePen(PS_DOT, 1, RGB(0, 50, 0));
				HPEN oldPen = (HPEN)SelectObject(mDC, pen);
				HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, GetStockObject(NULL_BRUSH));

				Rectangle(mDC, startX, startY, endX, endY);

				SelectObject(mDC, oldBrush);
				SelectObject(mDC, oldPen);
				DeleteObject(pen);
			}
			if (mode)
			{
				left = min(startX, endX);
				top = min(startY, endY);
				right = max(startX, endX);
				bottom = max(startY, endY);

				width = right - left;
				height = bottom - top;
				bmpX = left * 200 / rc.right;
				bmpY = top * 200 / rc.bottom;
				bmpW = width * 200 / rc.right;
				bmpH = height * 200 / rc.bottom;

				int drawX = bmpX;
				int drawY = bmpY;
				int drawW = bmpW;
				int drawH = bmpH;

				if (isH)
				{
					drawX = bmpX + bmpW;
					drawW = -bmpW;
				}

				if (isV)
				{
					drawY = bmpY + bmpH;
					drawH = -bmpH;
				}

				if (size > 0)
				{
					int oldW = bmpW;
					int oldH = bmpH;

					bmpW /= (size + 1);
					bmpH /= (size + 1);

					bmpX += (oldW - bmpW) / 2;
					bmpY += (oldH - bmpH) / 2;
				}
				else if (size < 0)
				{
					bmpW *= (-size + 1);
					bmpH *= (-size + 1);
				}
				HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				HPEN oldPen = (HPEN)SelectObject(mDC, pen);
				HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, GetStockObject(NULL_BRUSH));
				Rectangle(mDC, startX, startY, endX, endY);
				if(isF)
					StretchBlt(mDC, 0, 0,rc.right, rc.bottom, hMemDC, drawX, drawY, drawW, drawH, SRCCOPY);
				else if(isF==false)
					StretchBlt(mDC, 0, 0,width, height, hMemDC, drawX, drawY, drawW, drawH, SRCCOPY);
				if(isI)
					StretchBlt(mDC, 0, 0, width, height, hMemDC, drawX, drawY, drawW, drawH, NOTSRCCOPY);
				SelectObject(mDC,oldBrush);
				SelectObject(mDC, oldPen);
				DeleteObject(pen);

				for (int i = 0;i < pCount;i++)
				{
					StretchBlt(mDC, photo[i].x, photo[i].y, width/2, height/2, hMemDC, drawX, drawY, drawW, drawH, SRCCOPY);
				}
			}
			BitBlt(hDC, 0, 0, rc.right, rc.bottom, mDC, 0, 0, SRCCOPY);

			SelectObject(hMemDC, oldBmp);
			DeleteDC(hMemDC);

			SelectObject(mDC, oldBack);
			DeleteObject(hBackBitmap);
			DeleteDC(mDC);

			EndPaint(hWnd, &ps);
			break;
		}
		case WM_KEYDOWN:
		{
			if (wParam == 'Q')
			{
				DeleteObject(hBitmap);
				PostQuitMessage(0);
			}
			else if (wParam == 'E')
			{
				size++;
				if (size > 3)
					size = 0;
			}
			else if (wParam == 'B')
			{
				size=0;
			}
			else if(wParam=='S')
			{
				size--;
				if (size < -3)
					size = 0;
			}
			else if (wParam == 'F')
			{
				if (isF)
					isF = false;
				else
					isF = true;
			}
			else if (wParam == 'I')
			{
				if (isI)
					isI = false;
				else
					isI = true;
			}
			else if (wParam == VK_UP)
			{
				startY-=5;
				endY-=5;
			}
			else if (wParam == VK_LEFT)
			{
				startX-=5;
				endX-=5;
			}
			else if (wParam == VK_DOWN)
			{
				startY+=5;
				endY+=5;
			}
			else if (wParam == VK_RIGHT)
			{
				startX+=5;
				endX+=5;
			}
			else if (wParam == 'H')
			{
				isH = !isH;
			}
			else if (wParam == 'V')
			{
				isV = !isV;
			}
			else if (wParam == 'P')
			{
				if (pCount > 4)
					pCount = 0;
				int x = rand() % 900;
				int y = rand() % 900;
				photo[pCount].x = x;
				photo[pCount].y = y;
				pCount++;
			}
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		case WM_DESTROY:
		{
			DeleteObject(hBitmap);
			PostQuitMessage(0);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			if (mode == false)
			{
				isDrag = true;
				startX = LOWORD(lParam);
				startY = HIWORD(lParam);
				endX = startX;
				endY = startY;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (isDrag == true)
			{
				endX = LOWORD(lParam);
				endY = HIWORD(lParam);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			isDrag = false;
			mode = true;
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
