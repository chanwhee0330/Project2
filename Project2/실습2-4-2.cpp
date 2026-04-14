#include <windows.h>
#include <Windowsx.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"window program 1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int ranx, rany,ranboxx,ranboxy;
int winx = 800, winy = 600;

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
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, 0, 0, winx, winy, NULL, (HMENU)NULL, hInstance, NULL);
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
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		winx = LOWORD(lParam);
		winy = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		TCHAR charset[] = L"abcdefghigklnmopqrstuvwxyz0123456789";
		int charCount = lstrlen(charset);

		SIZE size;
		GetTextExtentPoint32(hDC, L"A", 1, &size);

		int charW = size.cx;
		int charH = size.cy;

		int boxWidth = rand() % (winx / 2) + 100;
		int boxHeight = rand() % (winy / 2) + 100;

		int startX = rand() % (winx - boxWidth);
		int startY = rand() % (winy - boxHeight);
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		for (int x = startX; x < startX + boxWidth; x += charW)
		{
			wsprintf(temp, L"%c",charset[rand()%charCount]);
			TextOut(hDC, x, startY, temp, 1);
		}
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		for (int x = startX; x < startX + boxWidth; x += charW)
		{
			wsprintf(temp, L"%c", charset[rand() % charCount]);
			TextOut(hDC, x, startY + boxHeight - charH, temp, 1);
		}
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		for (int y = startY; y < startY + boxHeight; y += charH)
		{
			wsprintf(temp, L"%c", charset[rand() % charCount]);
			TextOut(hDC, startX, y, temp, 1);
		}
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		for (int y = startY; y < startY + boxHeight; y += charH)
		{
			wsprintf(temp, L"%c", charset[rand() % charCount]);
			TextOut(hDC, startX + boxWidth - charW, y, temp, 1);
		}
		EndPaint(hWnd, &ps);
		break;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}