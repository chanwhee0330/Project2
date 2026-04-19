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
int px, py;

int stackType[900];
int stackCount = 0;
bool horizontal = false;
void CheckAndClearLines()
{
	for (int y = 0; y < 30; y++)
	{
		int first = board[y][0];

		if (first < 2 || first > 5)
			continue;

		bool same = true;

		for (int x = 1; x < 30; x++)
		{
			if (board[y][x] != first)
			{
				same = false;
				break;
			}
		}

		// 🔥 같은 색으로 꽉 찬 줄
		if (same)
		{
			for (int x = 0; x < 30; x++)
				board[y][x] = 0;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	int width, height, cell;
	GetClientRect(hWnd, &rect);
	width = rect.right;
	height = rect.bottom;
	cell = width / 30;
	static int turn = 4, tailCount = 0;

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

				if (board[y][x] == 0)   // 이 위치만 검사
				{
					board[y][x] = 2 + rand() % 4;
					break;  // 반드시 탈출
				}
			}
		}
		SetTimer(hWnd, 1, 250, NULL);
		break;
	case WM_TIMER:
	{
		int nextX = px, nextY = py;

		if (turn == 1) nextX -= 1;
		else if (turn == 2) nextY -= 1;
		else if (turn == 3) nextX += 1;
		else if (turn == 4) nextY += 1;

		// 🔥 워프 먼저 체크
		bool warped = false;
		if (nextY > 29)
		{
			nextY = 0;
			warped = true;
		}
		if (nextY < 0) nextY = 29;
		if (nextX < 0) nextX = 0;
		if (nextX > 29) nextX = 29;

		// 🔥 아이템 먹기 → 스택에 저장
		if (board[nextY][nextX] >= 2 && board[nextY][nextX] <= 5)
		{
			stackType[stackCount++] = board[nextY][nextX];
			board[nextY][nextX] = 0;
		}

		// 🔥 이동
		board[py][px] = 0;
		px = nextX;
		py = nextY;
		board[py][px] = 1;

		// 🔥 워프 순간 → 스택을 바닥에 떨어뜨림
		if (warped)
		{
			if (!horizontal)
			{
				// 🔥 세로 상태 → 아래로 쌓기
				for (int i = 0; i < stackCount; i++)
				{
					int y = 29 - i;
					if (y < 0) break;

					if (board[y][px] == 0)
						board[y][px] = stackType[i];
				}
			}
			else
			{
				// 🔥 가로 상태 → 오른쪽으로 쌓기 (바닥 기준)
				for (int i = 0; i < stackCount; i++)
				{
					int x = px + i;
					int y = 29;

					if (x >= 30) break;

					if (board[y][x] == 0)
						board[y][x] = stackType[i];
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
			MoveToEx(hDC, 0, i * cell, NULL);   // 시작점
			LineTo(hDC, cell * 30, i * cell);        // 가로선
			MoveToEx(hDC, i * cell, 0, NULL);   // 시작점
			LineTo(hDC, i * cell, cell * 30);        // 세로선
		}
		for (int i = 0;i < 30;i++)
		{
			for (int j = 0;j < 30;j++)
			{
				if (board[i][j] == 1)
				{
					hBrush = CreateSolidBrush(RGB(173, 216, 221));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, j * cell, i * cell, j * cell + cell, i * cell + cell);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

				}
				else if (board[i][j] >= 2 && board[i][j] <= 5)
				{
					COLORREF color;
					if (board[i][j] == 2) color = RGB(255, 99, 71);
					else if (board[i][j] == 3) color = RGB(255, 165, 0);
					else if (board[i][j] == 4) color = RGB(30, 144, 255);
					else if (board[i][j] == 5) color = RGB(138, 43, 226);
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
			{
				// 🔥 위로 쌓기
				ty = py - (i + 1);
			}
			else
			{
				// 🔥 오른쪽으로 쌓기
				tx = px + (i + 1);
			}

			// 화면 밖 방지
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
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam == 'Q')
		{
			PostQuitMessage(0);
		}
		else if (wParam == VK_LEFT)
		{
			turn = 1;
		}
		else if (wParam == VK_UP)
		{
			turn = 2;
		}
		else if (wParam == VK_RIGHT)
		{
			turn = 3;
		}
		else if (wParam == VK_DOWN)
		{
			turn = 4;
		}
		else if (wParam == VK_RETURN)
		{
			horizontal = !horizontal; // 토글
		}
		else if (wParam == 'R')
		{
			KillTimer(hWnd, 1);

			// 🔥 1. 보드 초기화
			for (int y = 0; y < 30; y++)
				for (int x = 0; x < 30; x++)
					board[y][x] = 0;

			// 🔥 2. 꼬리/스택 초기화
			stackCount = 0;

			// 🔥 3. 플레이어 재설정
			px = rand() % 30;
			py = 0;
			board[py][px] = 1;

			// 🔥 4. 아이템 재생성
			for (int i = 0; i < 60; i++)
			{
				while (1)
				{
					int x = rand() % 30;
					int y = rand() % 28 + 1;

					if (board[y][x] == 0)
					{
						board[y][x] = 2 + rand() % 4;
						break;
					}
				}
			}

			SetTimer(hWnd, 1, 250, NULL);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (board[y / cell][x / cell] == 0)
		{
			board[y / cell][x / cell] = 2 + rand() % 4;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (board[y / cell][x / cell] >= 2 && board[y / cell][x / cell] <= 5)
		{
			board[y / cell][x / cell] = 0;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}

	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

