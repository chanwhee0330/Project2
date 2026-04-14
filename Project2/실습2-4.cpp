#include <windows.h>
#include <Windowsx.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"window program 1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int row,random;
int winx=800, winy=600;

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
		if (winx < 100)
			winx = 100;
		if (winy < 100)
			winy = 100;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		row = rand() % 10 + 2;
		random = rand() % row;
		hDC = BeginPaint(hWnd, &ps);
		wsprintf(temp, L"winx : %d / winy= : %d / win/row : %d / row : %d", winx,winy,winx/row, (winx / row) + (1 * 30)+ row);
		TextOut(hDC, 300, 300, temp, lstrlen(temp));
		for (int i = 1; i <= 9; i++)
		{
			SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
			wsprintf(temp, L"%d * %d = %d", row, i, row * i);
			TextOut(hDC, ((winx / row)*random)+i*10, (i * 20), temp, lstrlen(temp));
		}
		EndPaint(hWnd, &ps);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}