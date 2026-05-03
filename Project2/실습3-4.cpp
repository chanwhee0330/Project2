#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <stdbool.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"WindowClass";
LPCTSTR lpszWindowName = L"Random Text Program";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int winx = 1000, winy = 1100;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;

	g_hInst = hInstance;
	srand((unsigned int)time(NULL));

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
		0, 0,
		winx, winy,
		NULL, NULL,
		hInstance, NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

int board[30][30];
int fixed[30][30];
int item[30][30];
int px, py;

int stackType[900];
int stackCount = 0;
bool horizontal = false;

void CheckAndClearLines()
{
	for (int y = 0; y < 30; y++)
	{
		int color = -1;
		bool same = true;

		for (int x = 0; x < 30; x++)
		{
			if (fixed[y][x] < 2 || fixed[y][x] > 5)
			{
				same = false;
				break;
			}

			if (x == 0)
				color = fixed[y][x];
			else if (fixed[y][x] != color)
			{
				same = false;
				break;
			}
		}

		if (same)
		{
			for (int x = 0; x < 30; x++)
				fixed[y][x] = 0;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH hBrush, oldBrush;
	int width, height, cell;

	GetClientRect(hWnd, &rect);
	width = rect.right;
	height = rect.bottom;
	cell = width / 30;
	static int lastDropX = -1;

	static int turn = 4;

	switch (iMessage) {

	case WM_CREATE:
		px = rand() % 30;
		py = 0;
		board[py][px] = 1;

		for (int i = 0; i < 60; i++)
		{
			while (1)
			{
				int x = rand() % 30;
				int y = rand() % 28 + 1;

				if (item[y][x] == 0)
				{
					item[y][x] = 2 + rand() % 4;
					break;
				}
			}
		}

		SetTimer(hWnd, 1, 250, NULL);
		break;
	case WM_TIMER:
	{
		int nextX = px, nextY = py;

		if (turn == 1) { nextX -= 1; nextY += 1; }
		else if (turn == 2) { nextY -= 1; }
		else if (turn == 3) { nextX += 1; nextY += 1; }
		else if (turn == 4) { nextY += 1; }

		if (nextX < 0) nextX = 0;
		if (nextX > 29) nextX = 29;
		if (nextY < 0) nextY = 0;

		bool reachedBottom = false;

		if (nextY > 29)
		{
			nextY = 0;
			reachedBottom = true;
		}

		// 먹기
		if (item[nextY][nextX] >= 2 && item[nextY][nextX] <= 5)
		{
			stackType[stackCount++] = item[nextY][nextX];
			item[nextY][nextX] = 0;
		}

		// 이동
		board[py][px] = 0;
		px = nextX;
		py = nextY;
		board[py][px] = 1;

		// 쌓기
		if (reachedBottom && stackCount > 0)
		{
			if (!horizontal)
			{
				for (int i = 0; i < stackCount; i++)
				{
					int dropY = 29;

					while (dropY >= 0 && fixed[dropY][px] != 0)
						dropY--;

					if (dropY >= 0)
						fixed[dropY][px] = stackType[i];
				}
			}
			else
			{
				for (int i = 0; i < stackCount; i++)
				{
					int x = px + i;
					if (x >= 30) break;

					int dropY = 29;

					while (dropY >= 0 && fixed[dropY][x] != 0)
						dropY--;

					if (dropY >= 0)
						fixed[dropY][x] = stackType[i];
				}
			}

			stackCount = 0;
		}
		CheckAndClearLines();
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		for (int i = 0; i <= 30; i++)
		{
			MoveToEx(hDC, 0, i * cell, NULL);
			LineTo(hDC, cell * 30, i * cell);
			MoveToEx(hDC, i * cell, 0, NULL);
			LineTo(hDC, i * cell, cell * 30);
		}

		for (int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++)
			{
				if (board[i][j] == 1)
				{
					hBrush = CreateSolidBrush(RGB(173, 216, 221));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, j * cell, i * cell, j * cell + cell, i * cell + cell);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
				}
				if (item[i][j] >= 2 && item[i][j] <= 5)
				{
					COLORREF color;
					if (item[i][j] == 2) color = RGB(255, 99, 71);
					else if (item[i][j] == 3) color = RGB(255, 165, 0);
					else if (item[i][j] == 4) color = RGB(30, 144, 255);
					else color = RGB(138, 43, 226);

					hBrush = CreateSolidBrush(color);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Rectangle(hDC, j * cell, i * cell, j * cell + cell, i * cell + cell);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
				}

				// 쌓인 블럭
				else if (fixed[i][j] >= 2 && fixed[i][j] <= 5)
				{
					COLORREF color;
					if (fixed[i][j] == 2) color = RGB(255, 99, 71);
					else if (fixed[i][j] == 3) color = RGB(255, 165, 0);
					else if (fixed[i][j] == 4) color = RGB(30, 144, 255);
					else color = RGB(138, 43, 226);

					hBrush = CreateSolidBrush(color);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Rectangle(hDC, j * cell, i * cell, j * cell + cell, i * cell + cell);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
				}
			}
		}

		for (int i = 0; i < stackCount; i++)
		{
			int tx = px;
			int ty = py;

			if (!horizontal)
				ty = py - (i + 1);
			else
				tx = px + (i + 1);

			if (tx < 0 || tx >= 30 || ty < 0 || ty >= 30)
				continue;

			COLORREF color;
			if (stackType[i] == 2) color = RGB(255, 99, 71);
			else if (stackType[i] == 3) color = RGB(255, 165, 0);
			else if (stackType[i] == 4) color = RGB(30, 144, 255);
			else if (stackType[i] == 5) color = RGB(138, 43, 226);

			hBrush = CreateSolidBrush(color);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			Rectangle(hDC,
				tx * cell,
				ty * cell,
				tx * cell + cell,
				ty * cell + cell);

			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		int bx = x / cell;
		int by = y / cell;

		if (bx < 0 || bx >= 30 || by < 0 || by >= 30)
			break;

		if (item[by][bx] == 0 && fixed[by][bx] == 0 && board[by][bx] == 0)
		{
			item[by][bx] = 2 + rand() % 4;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		int bx = x / cell;
		int by = y / cell;

		if (bx < 0 || bx >= 30 || by < 0 || by >= 30)
			break;

		if (item[by][bx] >= 2 && item[by][bx] <= 5)
		{
			item[by][bx] = 0;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}

	case WM_KEYDOWN:
		if (wParam == 'Q') PostQuitMessage(0);
		else if (wParam == VK_LEFT) turn = 1;
		else if (wParam == VK_UP) turn = 2;
		else if (wParam == VK_RIGHT) turn = 3;
		else if (wParam == VK_DOWN) turn = 4;
		else if (wParam == VK_RETURN) horizontal = !horizontal;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}