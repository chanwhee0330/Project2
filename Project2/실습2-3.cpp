#include <windows.h>
#include <Windowsx.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"window program 1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int row;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	srand((unsigned int)time(NULL));
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
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	TCHAR temp[100];

	switch (iMessage) {
	case WM_CREATE:
		row = rand() % 7+2;
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		COLORREF colors[100];
		if (row % 2 == 0)
		{
			for (int j = 0; j < row; j++)
			{
				colors[j] = RGB(rand() % 256, rand() % 256, rand() % 256);
			}
		}
		for (int j = 0; j < row; j++)
		{
			for (int i = 1; i <= 9; i++)
			{
				if (row % 2 == 0)
				{
					SetTextColor(hDC, colors[j]);
				}
				else 
				{
					SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
				}
				wsprintf(temp, L"%d * %d = %d", j+2, i, (j+2) * i);
				TextOut(hDC, j*(800/row), i * 20, temp, lstrlen(temp));
			}
		}

		for (int j = row-1; j >= 0; j--) 
		{
			for (int i = 1; i <= 9; i++)
			{
				if (row % 2 == 0)
				{
					SetTextColor(hDC, colors[j]);
				}
				else
				{
					SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
				}
				wsprintf(temp, L"%d * %d = %d", j + 2, i, (j + 2) * i);
				int  x = (row - 1 - j) * (800 / row);
				TextOut(hDC, x, (i * 20)+300, temp, lstrlen(temp));
			}
		}
		wsprintf(temp, L"%dµîºÐ", row);
		TextOut(hDC, 350, 250, temp, lstrlen(temp));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}