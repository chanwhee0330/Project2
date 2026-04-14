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
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
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

static struct Obstacle {
	int obx, oby;
};
static struct Changecolor {
	int x, y;
	COLORREF color;
};
static struct Miners {
	int x, y;
};

static struct Plus {
	int x, y;
};

static struct Change {
	int x, y;
};
struct Obstacle ob[20];
struct Changecolor cc[20];
struct Miners min[20];
struct Plus pp[20];
struct Change co[20];

bool isCollision(int x, int y, int cell, struct Obstacle ob[])
{
	int gridX = x / cell;
	int gridY = y / cell;

	for (int i = 0; i < 20; i++)
	{
		if (ob[i].obx == gridX && ob[i].oby == gridY)
			return true;
	}
	return false;
}

int isColor(int x, int y, int cell, struct Changecolor cc[])
{
	int gridX = x / cell;
	int gridY = y / cell;

	for (int i = 0; i < 20; i++)
	{
		if (cc[i].x == gridX && cc[i].y == gridY)
			return i;
	}
	return -1;
}

int isMin(int x, int y, int cell, struct Miners min[])
{
	int gridX = x / cell;
	int gridY = y / cell;

	for (int i = 0; i < 20; i++)
	{
		if (min[i].x == gridX && min[i].y == gridY)
			return 1;
	}
	return -1;
}

int isPlus(int x, int y, int cell, struct Plus pp[])
{
	int gridX = x / cell;
	int gridY = y / cell;

	for (int i = 0; i < 20; i++)
	{
		if (pp[i].x == gridX && pp[i].y == gridY)
			return 1;
	}
	return -1;
}

int isChange(int x, int y, int cell, struct Change co[])
{
	int gridX = x / cell;
	int gridY = y / cell;

	for (int i = 0; i < 20; i++)
	{
		if (co[i].x == gridX && co[i].y == gridY)
			return 1;
	}
	return -1;
}

bool isOverlap1(int x, int y, struct Obstacle ob[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (ob[i].obx == x && ob[i].oby == y)
			return true;
	}
	return false;
}

bool isOverlap2(int x, int y, struct Changecolor cc[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (cc[i].x == x && cc[i].y == y)
			return true;
	}
	return false;
}

bool isOverlap3(int x, int y, struct Miners min[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (min[i].x == x && min[i].y == y)
			return true;
	}
	return false;
}

bool isOverlap4(int x, int y, struct Plus pp[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (pp[i].x == x && pp[i].y == y)
			return true;
	}
	return false;
}

bool isOverlap5(int x, int y, struct Change co[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (co[i].x == x && co[i].y == y)
			return true;
	}
	return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH hPen, oldPen;
	int width/*가로*/, height/*세로*/, cell;
	static int p1x=0, p1y=0, p2x=0, p2y=0;
	static int p1size,p1shape,p2size,p2shape,p1count=0,p2count=0,p1ori,p2ori;
	static COLORREF p1rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
	static COLORREF p2rgb = RGB(rand() % 255, rand() % 255, rand() % 255);
	COLORREF obcor = RGB(255, 0, 0);
	GetClientRect(hWnd, &rect);
	width = rect.right;
	height = rect.bottom;
	cell = width / 40;
	static bool turn=false,cheat=false,p1changed=false,p2changed=false;
	static int winner = 0;
	int goalX = 20 * cell;
	int goalY = 39 * cell;
	
	switch (iMessage) {
	case WM_CREATE:
		p1x = 0, p1y = 0;
		p2x = cell * 39, p2y = 0;
		winner = 0;
		for (int i = 0; i < 20; i++)
		{
			while (1)
			{
				int x = rand() % 40;
				int y = rand() % 40;

				// 금지 위치
				if ((x == 0 && y == 0) || (x == 39 && y == 0)||(x==20&&y==39))
					continue;

				// 겹침 체크
				if (isOverlap1(x, y, ob, i))
					continue;

				ob[i].obx = x;
				ob[i].oby = y;
				break;
			}
			while (1)
			{
				int x = rand() % 40;
				int y = rand() % 40;

				// 금지 위치
				if ((x == 0 && y == 0) || (x == 39 && y == 0) || (x == 20 && y == 39))
					continue;

				// 겹침 체크
				if (isOverlap2(x, y, cc, i))
					continue;

				cc[i].x = x;
				cc[i].y = y;
				cc[i].color = RGB(rand() % 255, rand() % 255, rand() % 255);
				break;
			}
			while (1)
			{
				int x = rand() % 40;
				int y = rand() % 40;

				// 금지 위치
				if ((x == 0 && y == 0) || (x == 39 && y == 0) || (x == 20 && y == 39))
					continue;

				// 겹침 체크
				if (isOverlap3(x, y, min, i))
					continue;

				min[i].x = x;
				min[i].y = y;
				break;
			}
			while (1)
			{
				int x = rand() % 40;
				int y = rand() % 40;

				// 금지 위치
				if ((x == 0 && y == 0) || (x == 39 && y == 0) || (x == 20 && y == 39))
					continue;

				// 겹침 체크
				if (isOverlap4(x, y, pp, i))
					continue;

				pp[i].x = x;
				pp[i].y = y;
				break;
			}
			while (1)
			{
				int x = rand() % 40;
				int y = rand() % 40;

				// 금지 위치
				if ((x == 0 && y == 0) || (x == 39 && y == 0) || (x == 20 && y == 39))
					continue;

				// 겹침 체크
				if (isOverlap5(x, y, co, i))
					continue;

				co[i].x = x;
				co[i].y = y;
				break;
			}
		}
		p1shape =rand()%4+1;
		p1size = rand()%5+1;
		p2shape = rand() % 4 + 1;
		p2size = rand() % 5 + 1;
		p1ori = p1shape;
		p2ori = p2shape;
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		for (int i = 0; i <= 40; i++)
		{
			MoveToEx(hDC, 0, i * cell, NULL);   // 시작점
			LineTo(hDC, cell * 40, i * cell);        // 가로선
			MoveToEx(hDC, i * cell, 0, NULL);   // 시작점
			LineTo(hDC, i * cell, cell * 40);        // 세로선
		}
		for (int i = 0; i < 20; i++)
		{
			hPen = CreateSolidBrush(obcor);
			oldPen = (HBRUSH)SelectObject(hDC, hPen);
			Rectangle(hDC, ob[i].obx * cell, ob[i].oby * cell, ob[i].obx * cell + cell, ob[i].oby * cell + cell);
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);

			hPen = CreateSolidBrush(cc[i].color);
			oldPen = (HBRUSH)SelectObject(hDC, hPen);
			Rectangle(hDC, cc[i].x * cell, cc[i].y * cell, cc[i].x * cell + cell, cc[i].y * cell + cell);
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);

			hPen = CreateSolidBrush(RGB(0,0,0));
			oldPen = (HBRUSH)SelectObject(hDC, hPen);
			Rectangle(hDC, min[i].x * cell, min[i].y * cell, min[i].x * cell + cell, min[i].y * cell + cell);
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);

			hPen = CreateSolidBrush(RGB(0, 255, 0));
			oldPen = (HBRUSH)SelectObject(hDC, hPen);
			Rectangle(hDC, pp[i].x* cell, pp[i].y* cell, pp[i].x* cell + cell, pp[i].y* cell + cell);
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);

			hPen = CreateSolidBrush(RGB(0,0, 255));
			oldPen = (HBRUSH)SelectObject(hDC, hPen);
			Rectangle(hDC, co[i].x* cell, co[i].y* cell, co[i].x* cell + cell, co[i].y* cell + cell);
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);

		}

		hPen = CreateSolidBrush(RGB(255, 0, 0));
		oldPen = (HBRUSH)SelectObject(hDC, hPen);
		Ellipse(hDC, cell*20, cell*39, cell*20+cell, cell*39+cell);
		SelectObject(hDC, oldPen);
		DeleteObject(hPen);

		hPen = CreateSolidBrush(p1rgb);
		oldPen = (HBRUSH)SelectObject(hDC, hPen);
		if (p1shape == 1)
		{
			if (p1size == 1)
			{
				int size = 8;
				Ellipse(hDC, p1x + size, p1y + size, p1x + cell - size, p1y + cell - size);
			}
			else if (p1size == 2)
			{
				int size = 4;
				Ellipse(hDC, p1x+size, p1y+size, p1x + cell-size, p1y + cell-size);
			}
			else if (p1size == 3)
			{
				Ellipse(hDC, p1x, p1y, p1x + cell, p1y + cell);
			}
			else if (p1size == 4)
			{
				int size = 4;
				Ellipse(hDC, p1x - size, p1y - size, p1x + cell + size, p1y + cell + size);
			}
			else if (p1size == 5)
			{
				int size = 8;
				Ellipse(hDC, p1x - size, p1y - size, p1x + cell + size, p1y + cell + size);
			}
		}
		else if (p1shape == 2)
		{
			if (p1size == 1)
			{
				int size = 8;
				Rectangle(hDC, p1x + size, p1y + size, p1x + cell - size, p1y + cell - size);
			}
			else if (p1size == 2)
			{
				int size = 8;
				Rectangle(hDC, p1x + size, p1y + size, p1x + cell - size, p1y + cell - size);
			}
			else if (p1size == 3)
			{
				Rectangle(hDC, p1x, p1y, p1x + cell, p1y + cell);
			}
			else if (p1size == 4)
			{
				int size = 4;
				Rectangle(hDC, p1x - size, p1y - size, p1x + cell + size, p1y + cell + size);
			}
			else if (p1size == 5)
			{
				int size = 8;
				Rectangle(hDC, p1x - size, p1y - size, p1x + cell + size, p1y + cell + size);
			}
		}
		else if (p1shape == 3)
		{
			if (p1size == 1)
			{
				int size = 8;
				POINT point[6] = { {p1x + cell / 2,p1y + size},{p1x + size,p1y + cell - size},{p1x + cell - size,p1y + cell - size} };
				Polygon(hDC, point, 3);
			}
			else if (p1size == 2)
			{
				int size = 4;
				POINT point[6] = { {p1x + cell / 2,p1y + size},{p1x + size,p1y + cell - size},{p1x + cell - size,p1y + cell - size} };
				Polygon(hDC, point, 3);
			}
			else if (p1size == 3)
			{
				POINT point[6] = { {p1x + cell / 2,p1y},{p1x,p1y + cell},{p1x + cell,p1y + cell} };
				Polygon(hDC, point, 3);
			}
			else if (p1size == 4)
			{
				int size = 4;
				POINT point[6] = { {p1x + cell / 2,p1y - size},{p1x - size,p1y + cell + size},{p1x + cell + size,p1y + cell + size} };
				Polygon(hDC, point, 3);
			}
			else if (p1size == 5)
			{
				int size = 8;
				POINT point[6] = { {p1x + cell / 2,p1y - size},{p1x - size,p1y + cell + size},{p1x + cell + size,p1y + cell + size} };
				Polygon(hDC, point, 3);
			}
		}
		else if (p1shape == 4)
		{
			if (p1size == 1)
			{
				int size = 4;
				Ellipse(hDC, p1x + size, p1y + 5 + size, p1x + cell - size, p1y + cell - 5 - size);
			}
			else if (p1size == 2)
			{
				int size = 2;
				Ellipse(hDC, p1x+size, p1y + 5+size, p1x + cell-size, p1y + cell - 5-size);
			}
			else if (p1size == 3)
			{
				Ellipse(hDC, p1x, p1y+5, p1x + cell, p1y + cell-5);
			}
			else if (p1size == 4)
			{
				int size = 2;
				Ellipse(hDC, p1x - size, p1y + 5 - size, p1x + cell + size, p1y + cell - 5 + size);
			}
			else if (p1size == 5)
			{
				int size = 4;
				Ellipse(hDC, p1x - size, p1y + 5 - size, p1x + cell + size, p1y + cell - 5 + size);
			}
		}
		SelectObject(hDC, oldPen);
		DeleteObject(hPen);



		hPen = CreateSolidBrush(p2rgb);
		oldPen = (HBRUSH)SelectObject(hDC, hPen);
		if (p2shape == 1)
		{
			if (p2size == 1)
			{
				int size = 8;
				Ellipse(hDC, p2x + size, p2y + size, p2x + cell - size, p2y + cell - size);
			}
			else if (p2size == 2)
			{
				int size = 4;
				Ellipse(hDC, p2x + size, p2y + size, p2x + cell - size, p2y + cell - size);
			}
			else if (p2size == 3)
			{
				Ellipse(hDC, p2x, p2y, p2x + cell, p2y + cell);
			}
			else if (p2size == 4)
			{
				int size = 4;
				Ellipse(hDC, p2x - size, p2y - size, p2x + cell + size, p2y + cell + size);
			}
			else if (p2size == 5)
			{
				int size = 8;
				Ellipse(hDC, p2x - size, p2y - size, p2x + cell + size, p2y + cell + size);
			}
		}
		else if (p2shape == 2)
		{
			if (p2size == 1)
			{
				int size = 8;
				Rectangle(hDC, p2x + size, p2y + size, p2x + cell - size, p2y + cell - size);
			}
			else if (p2size == 2)
			{
				int size = 8;
				Rectangle(hDC, p2x + size, p2y + size, p2x + cell - size, p2y + cell - size);
			}
			else if (p2size == 3)
			{
				Rectangle(hDC, p2x, p2y, p2x + cell, p2y + cell);
			}
			else if (p2size == 4)
			{
				int size = 4;
				Rectangle(hDC, p2x - size, p2y - size, p2x + cell + size, p2y + cell + size);
			}
			else if (p2size == 5)
			{
				int size = 8;
				Rectangle(hDC, p2x - size, p2y - size, p2x + cell + size, p2y + cell + size);
			}
		}
		else if (p2shape == 3)
		{
			if (p2size == 1)
			{
				int size = 8;
				POINT point[6] = { {p2x + cell / 2,p2y + size},{p2x + size,p2y + cell - size},{p2x + cell - size,p2y + cell - size} };
				Polygon(hDC, point, 3);
			}
			else if (p2size == 2)
			{
				int size = 4;
				POINT point[6] = { {p2x + cell / 2,p2y + size},{p2x + size,p2y + cell - size},{p2x + cell - size,p2y + cell - size} };
				Polygon(hDC, point, 3);
			}
			else if (p2size == 3)
			{
				POINT point[6] = { {p2x + cell / 2,p2y},{p2x,p2y + cell},{p2x + cell,p2y + cell} };
				Polygon(hDC, point, 3);
			}
			else if (p2size == 4)
			{
				int size = 4;
				POINT point[6] = { {p2x + cell / 2,p2y - size},{p2x - size,p2y + cell + size},{p2x + cell + size,p2y + cell + size} };
				Polygon(hDC, point, 3);
			}
			else if (p2size == 5)
			{
				int size = 8;
				POINT point[6] = { {p2x + cell / 2,p2y - size},{p2x - size,p2y + cell + size},{p2x + cell + size,p2y + cell + size} };
				Polygon(hDC, point, 3);
			}
		}
		else if (p2shape == 4)
		{
			if (p2size == 1)
			{
				int size = 4;
				Ellipse(hDC, p2x + size, p2y + 5 + size, p2x + cell - size, p2y + cell - 5 - size);
			}
			else if (p2size == 2)
			{
				int size = 2;
				Ellipse(hDC, p2x + size, p2y + 5 + size, p2x + cell - size, p2y + cell - 5 - size);
			}
			else if (p2size == 3)
			{
				Ellipse(hDC, p2x, p2y + 5, p2x + cell, p2y + cell - 5);
			}
			else if (p2size == 4)
			{
				int size = 2;
				Ellipse(hDC, p2x - size, p2y + 5 - size, p2x + cell + size, p2y + cell - 5 + size);
			}
			else if (p2size == 5)
			{
				int size = 4;
				Ellipse(hDC, p2x - size, p2y + 5 - size, p2x + cell + size, p2y + cell - 5 + size);
			}
		}
		SelectObject(hDC, oldPen);
		DeleteObject(hPen);

		if (winner != 0)
		{
			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(0, 0, 0));

			if (winner == 1)
				TextOut(hDC, width / 2 - 80, height / 2, L"P1 WIN!", 7);
			else if (winner == 2)
				TextOut(hDC, width / 2 - 80, height / 2, L"P2 WIN!", 7);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == 'W'&&turn==false||wParam=='W'&&cheat==true)
		{
			if (p1y > 0)
			{
				int nextY = p1y - cell;
				if (!isCollision(p1x, nextY, cell, ob))
				{
					p1y = nextY;
					int idx = isColor(p1x, nextY, cell, cc);
					int miners = isMin(p1x, nextY, cell, min);
					int plus = isPlus(p1x, nextY, cell, pp);
					int change = isChange(p1x, nextY, cell, co);
					if (idx != -1)
					{
						p1rgb = cc[idx].color;
					}
					if (miners == 1)
					{
						if(p1size>1)
							p1size--;
					}
					if (plus == 1)
					{
						if (p1size < 5)
							p1size++;
					}
					if (change == 1)
					{
						int newShpae;
						do {
							newShpae = rand() % 4 + 1;
						} while (newShpae == p1shape);

						p1shape = newShpae;
						p1changed = true;
						p1count = 0;
					}
					if (p1changed == true)
					{
						p1count++;
						if (p1count >= 10)
						{
							p1count = 0;
							p1shape = p1ori;
							p1changed = false;
						}
					}

					if (p1x == goalX && p1y == goalY)
					{
						winner = 1;
					}
					
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			turn = true;
		}
		else if (wParam == 'S'&&turn==false || wParam == 'S' && cheat == true)
		{
			if (p1y < cell * 39)
			{
				int nextY = p1y + cell;
				if (!isCollision(p1x, nextY, cell, ob))
				{
					p1y = nextY;
					int miners = isMin(p1x, nextY, cell, min);
					int idx = isColor(p1x, nextY, cell, cc);
					int plus = isPlus(p1x, nextY, cell, pp);
					int change = isChange(p1x, nextY, cell, co);
					if (idx != -1)
					{
						p1rgb = cc[idx].color;
					}
					if (miners == 1)
					{
						if (p1size > 1)
							p1size--;
					}
					if (plus == 1)
					{
						if (p1size < 5)
							p1size++;
					}
					if (change == 1)
					{
						int newShpae;
						do {
							newShpae = rand() % 4 + 1;
						} while (newShpae == p1shape);
						p1count = 0;
						p1changed = true;
						p1shape = newShpae;
					}
					if (p1changed == true)
					{
						p1count++;
						if (p1count >= 10)
						{
							p1count = 0;
							p1shape = p1ori;
							p1changed = false;
						}
					}

					if (p1x == goalX && p1y == goalY)
					{
						winner = 1;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			turn = true;
		}
		else if (wParam == 'A'&&turn==false || wParam == 'A' && cheat == true)
		{
			if (p1x > 0)
			{
				int nextX = p1x - cell;
				if (!isCollision(nextX, p1y, cell, ob))
				{
					p1x = nextX;
					int idx = isColor(nextX, p1y, cell, cc);
					int miners = isMin(nextX, p1y, cell, min);
					int plus = isPlus(nextX, p1y, cell, pp);
					int change = isChange(nextX, p1y, cell, co);
					if (idx != -1)
					{
						p1rgb = cc[idx].color;
					}
					if (miners == 1)
					{
						if (p1size > 1)
							p1size--;
					}
					if (plus == 1)
					{
						if (p1size < 5)
							p1size++;
					}
					if (change == 1)
					{
						int newShpae;
						do {
							newShpae = rand() % 4 + 1;
						} while (newShpae == p1shape);

						p1shape = newShpae;
						p1changed = true;
						p1count = 0;
					}
					if (p1changed == true)
					{
						p1count++;
						if (p1count >= 10)
						{
							p1count = 0;
							p1shape = p1ori;
							p1changed = false;
						}
					}

					if (p1x == goalX && p1y == goalY)
					{
						winner = 1;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			turn = true;
		}
		else if (wParam == 'D'&&turn==false || wParam == 'D' && cheat == true)
		{
			if (p1x < cell * 39)
			{
				int nextX = p1x + cell;
				if (!isCollision(nextX, p1y, cell, ob))
				{
					p1x = nextX;
					int idx = isColor(nextX, p1y, cell, cc);
					int miners = isMin(nextX, p1y, cell, min);
					int plus = isPlus(nextX, p1y, cell, pp);
					int change = isChange(nextX, p1y, cell, co);
					if (idx != -1)
					{
						p1rgb = cc[idx].color;
					}
					if (miners == 1)
					{
						if (p1size > 1)
							p1size--;
					}
					if (plus == 1)
					{
						if (p1size < 5)
							p1size++;
					}
					if (change == 1)
					{
						int newShpae;
						do {
							newShpae = rand() % 4 + 1;
						} while (newShpae == p1shape);
						p1changed = true;
						p1count = 0;
						p1shape = newShpae;
					}
					if (p1changed == true)
					{
						p1count++;
						if (p1count >= 10)
						{
							p1count = 0;
							p1shape = p1ori;
							p1changed = false;
						}
					}

					if (p1x == goalX && p1y == goalY)
					{
						winner = 1;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			turn = true;
		}
		else if (wParam == 'Q')
		{
			PostQuitMessage(0);
		}
		else if (wParam == VK_ESCAPE)
		{
			cheat = true;
		}
		if (wParam == 'I'&&turn==true)
		{
			if (p2y > 0)
			{
				int nextY = p2y - cell;
				if (!isCollision(p2x, nextY, cell, ob))
				{
					p2y = nextY;
					int idx = isColor(p2x, nextY, cell, cc);
					int miners = isMin(p2x, nextY, cell, min);
					int plus = isPlus(p2x, nextY, cell, pp);
					int change = isChange(p2x, nextY, cell, co);
					if (idx != -1)
					{
						p2rgb = cc[idx].color;
					}
					if (miners == 1)
					{
						if (p2size > 1)
							p2size--;
					}
					if (plus == 1)
					{
						if (p2size < 5)
							p2size++;
					}
					if (change == 1)
					{
						int newShpae;
						do {
							newShpae = rand() % 4 + 1;
						} while (newShpae == p2shape);
						p2changed = true;
						p2count = 0;
						p2shape = newShpae;
					}
					if (p2changed == true)
					{
						p2count++;
						if (p2count >= 5)
						{
							p2count = 0;
							p2shape = p2ori;
							p2changed = false;
						}
					}

					if (p2x == goalX && p2y == goalY)
					{
						winner = 2;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			turn = false;
		}
		else if (wParam == 'K'&&turn==true)
		{
			if (p2y < cell * 39)
			{
				int nextY = p2y + cell;
				if (!isCollision(p2x, nextY, cell, ob))
				{
					p2y = nextY;
					int miners = isMin(p2x, nextY, cell, min);
					int idx = isColor(p2x, nextY, cell, cc);
					int plus = isPlus(p2x, nextY, cell, pp);
					int change = isChange(p2x, nextY, cell, co);
					if (idx != -1)
					{
						p2rgb = cc[idx].color;
					}
					if (miners == 1)
					{
						if (p2size > 1)
							p2size--;
					}
					if (plus == 1)
					{
						if (p2size < 5)
							p2size++;
					}
					if (change == 1)
					{
						int newShpae;
						do {
							newShpae = rand() % 4 + 1;
						} while (newShpae == p2shape);
						p2changed = true;
						p2count = 0;
						p2shape = newShpae;
					}
					if (p2changed == true)
					{
						p2count++;
						if (p2count >= 5)
						{
							p2count = 0;
							p2shape = p2ori;
							p2changed = false;
						}
					}

					if (p2x == goalX && p2y == goalY)
					{
						winner = 2;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			turn = false;
		}
		else if (wParam == 'J'&&turn==true)
		{
			if (p2x > 0)
			{
				int nextX = p2x - cell;
				if (!isCollision(nextX, p2y, cell, ob))
				{
					p2x = nextX;
					int idx = isColor(nextX, p2y, cell, cc);
					int miners = isMin(nextX, p2y, cell, min);
					int plus = isPlus(nextX, p2y, cell, pp);
					int change = isChange(nextX, p2y, cell, co);
					if (idx != -1)
					{
						p2rgb = cc[idx].color;
					}
					if (miners == 1)
					{
						if (p2size > 1)
							p2size--;
					}
					if (plus == 1)
					{
						if (p2size < 5)
							p2size++;
					}
					if (change == 1)
					{
						int newShpae;
						do {
							newShpae = rand() % 4 + 1;
						} while (newShpae == p2shape);
						p2changed = true;
						p2count = 0;
						p2shape = newShpae;
					}
					if (p2changed == true)
					{
						p2count++;
						if (p2count >= 5)
						{
							p2count = 0;
							p2shape = p2ori;
							p2changed = false;
						}
					}

					if (p2x == goalX && p2y == goalY)
					{
						winner = 2;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			turn = false;
		}
		else if (wParam == 'L'&&turn==true)
		{
			if (p2x < cell * 39)
			{
				int nextX = p2x + cell;
				if (!isCollision(nextX, p2y, cell, ob))
				{
					p2x = nextX;
					int idx = isColor(nextX, p2y, cell, cc);
					int miners = isMin(nextX, p2y, cell, min);
					int plus = isPlus(nextX, p2y, cell, pp);
					int change = isChange(nextX, p2y, cell, co);
					if (idx != -1)
					{
						p2rgb = cc[idx].color;
					}
					if (miners == 1)
					{
						if (p2size > 1)
							p2size--;
					}
					if (plus == 1)
					{
						if (p2size < 5)
							p2size++;
					}
					if (change == 1)
					{
						int newShpae;
						do {
							newShpae = rand() % 4 + 1;
						} while (newShpae == p2shape);
						p2changed = true;
						p2count = 0;
						p2shape = newShpae;
					}
					if (p2changed == true)
					{
						p2count++;
						if (p2count >= 5)
						{
							p2count = 0;
							p2shape = p2ori;
							p2changed = false;
						}
					}

					if (p2x == goalX && p2y == goalY)
					{
						winner = 2;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			turn = false;
		}
		else if (wParam == 'R')
		{
			SendMessageW(hWnd, WM_CREATE, 0, 0);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

