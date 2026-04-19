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


struct Player {
	int x, y;
};

struct Item {
	int x, y, rgb;
	COLORREF color;
	bool first = false;
};

struct Tail {
	int x, y;
	COLORREF color;
};

struct Item item[100];
struct Player p;
struct Tail tail[100];
int tailCount;
int isCollision(int x, int y, int cell, struct Item item[])
{

	for (int i = 0; i < 100; i++)
	{
		if (item[i].x == x && item[i].y == y)
			return i;
	}
	for (int i = 0; i < tailCount; i++)
	{
		if (tail[i].x == x && tail[i].y == y)
			return -2; // tail 구분용
	}
	return -1;
}


bool isOverlap1(int x, int y, struct Item item[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (item[i].x == x && item[i].y == y)
			return true;
	}
	return false;
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
	static int countItem, turn, px, py;

	switch (iMessage) {
	case WM_CREATE:
		countItem = 60;
		turn = 4;
		tailCount = 0;
		p.x = rand() % 30 * cell;
		p.y = 0;
		px = p.x;
		py = p.y;
		for (int i = 0;i < 60;i++)
		{
			while (1)
			{
				int x = rand() % 30;
				int y = rand() % 28 + 1;

				if (isOverlap1(x, y, item, i))
					continue;
				item[i].rgb = rand() % 4;
				if (item[i].rgb == 0)
				{
					item[i].color = RGB(255, 0, 0);
				}
				else if (item[i].rgb == 1)
				{
					item[i].color = RGB(0, 255, 0);
				}
				else if (item[i].rgb == 2)
				{
					item[i].color = RGB(0, 0, 255);
				}
				else if (item[i].rgb == 3)
				{
					item[i].color = RGB(255, 255, 102);
				}
				item[i].x = x * cell;
				item[i].y = y * cell;
				break;
			}
		}
		SetTimer(hWnd, 1, 200, NULL);
		break;
	case WM_TIMER:
	{
		int dx = 0, dy = 0;

		if (turn == 1) dx = -cell;
		else if (turn == 2) dy = -cell;
		else if (turn == 3) dx = cell;
		else if (turn == 4) dy = cell;

		int nx = px + dx;
		int ny = py + dy;

		// 벽 체크
		if (nx < 0 || nx >= cell * 30 || ny < 0 || ny >= cell * 30)
			break;

		// 충돌 체크
		int idx = isCollision(nx, ny, cell, item);

		if (idx != -1)
		{
			//꼬리 추가 (플레이어 아래쪽 기준)
			tail[tailCount].x = nx;
			tail[tailCount].y = ny;
			tail[tailCount].color = item[idx].color;
			tailCount++;

			// 아이템 제거 (화면 밖으로 보내기)
			item[idx].x = -100;
			item[idx].y = -100;
		}

		// 플레이어 이동
		px = nx;
		py = ny;
		p.x = px;
		p.y = py;

		bool hitBottom = false;

		for (int i = 0; i < tailCount; i++)
		{
			if (tail[i].y >= cell * 29)
			{
				hitBottom = true;
				break;
			}
		}

		if (hitBottom)
		{
			// 👉 꼬리는 그대로 두고
			// 👉 플레이어만 맨 위로 이동
			py = 0;            // 맨 위

			p.x = px;
			p.y = py;

			// 👉 꼬리는 더 이상 따라오지 않게 해야 함
			// (중요: 아래 코드 제거 or 조건 처리)
		}

		if (idx == -2)
		{
			// 꼬리 충돌 → 이동 막기
			break;
		}

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

		hBrush = CreateSolidBrush(RGB(224, 255, 255)); // 플레이어 그리기 
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, p.x, p.y, p.x + cell, p.y + cell);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		for (int i = 0;i < countItem;i++)
		{
			hBrush = CreateSolidBrush(item[i].color);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Rectangle(hDC, item[i].x, item[i].y, item[i].x + cell, item[i].y + cell);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}

		for (int i = 0; i < tailCount; i++)
		{
			hBrush = CreateSolidBrush(tail[i].color);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Rectangle(hDC, tail[i].x, tail[i].y,
				tail[i].x + cell, tail[i].y + cell);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}

		EndPaint(hWnd, &ps);
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
		else if (wParam == VK_RIGHT)
		{
			turn = 3;
		}
		else if (wParam == VK_UP)
		{
			turn = 2;
		}
		else if (wParam == VK_DOWN)
		{
			turn = 4;
		}
		break;
	}
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

