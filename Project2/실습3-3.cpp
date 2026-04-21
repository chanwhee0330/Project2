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

static struct Player {
	int x, y, shape, size;
	COLORREF RGB;
};

static struct Obstacle {
	int x, y, size;
	COLORREF RGB;
};

static struct Food {
	int x, y, size, move, tail, end; //move는 이동 방법
	bool moving1, moving2,active;		// moving1은 이동방법대로 이동하는 지 moving2는 꼬리상태인지
	COLORREF RGB;
};

struct Player p;
struct Obstacle ob[20];
struct Food f[20];
int tail[20];
int tailCount = 0;

int isCollision(int x, int y,int prevPx,int prevPy, int cell, struct Food f[])
{

	for (int i = 0; i < 20; i++)
	{
		if (f[i].x == x && f[i].y == y)
			return i;

		if (f[i].x == prevPx && f[i].y == prevPy)
			return i;
	}
	return -1;
}

int isCollision1(int x, int y, int cell, struct Obstacle ob[])
{
	for (int i = 0; i < 20; i++)
	{
		if (ob[i].x == x && ob[i].y == y)
			return -1;
	}
	return 1;
}

bool isOverlap1(int x, int y, struct Food f[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (f[i].x == x && f[i].y == y)
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
	cell = width / 40;
	static int speed = 250, turn = 1,obCount,Ax,Ay,jump=0;
	static bool isA=false,jump1=false,jump2=false,jump3=false,jump4=false;
	static int answer;
	switch (iMessage) {
	case WM_CREATE:
		p.x = rand() % 40 * cell;
		p.y = rand() % 40 * cell;
		p.RGB = RGB(255, 0, 0);
		p.shape = 1;
		for (int i = 0;i < 20;i++)
		{
			while (1)
			{
				int x = rand() % 40;
				int y = rand() % 40;
				f[i].RGB = RGB(rand() % 255, rand() % 255, rand() % 255);
				f[i].move = -1;
				f[i].size = 0;
				f[i].end = 1;
				f[i].moving1 = false;
				f[i].moving2 = false;
				f[i].active = true;
				if (x == p.x && y == p.y)
					continue;

				if (isOverlap1(x, y, f, i))
					continue;

				f[i].x = x * cell;
				f[i].y = y * cell;
				break;
			}
		}

		break;
	case WM_TIMER:
	{
		// 1. 이전 위치 저장
		int prevX = p.x;
		int prevY = p.y;
		int obx = isCollision1(prevX, prevY, cell, ob);

		// 2. 방향에 따라 이동
		if (isA == false)
		{
			if (turn == 1)         p.x += cell;   // 오른쪽
			else if (turn == 2)    p.x -= cell;   // 왼쪽
			else if (turn == 3)    p.y -= cell;   // 위
			else if (turn == 4)    p.y += cell;   // 아래

			if (obx == -1)
			{
				p.x += cell;
				p.y += cell;
			}

			// 3. 경계 처리
			if (p.x >= cell * 40)
			{
				p.x = cell * 39;
				p.y += cell;
				if (p.y >= cell * 40) p.y = 0;
				turn = 2;
			}
			else if (p.x < 0)
			{
				p.x = 0;
				p.y += cell;
				if (p.y >= cell * 40) p.y = 0;
				turn = 1;
			}

			if (p.y < 0)
			{
				p.y = 0;
				p.x += cell;
				if (p.x >= cell * 40) p.x = 0;
				turn = 4;
			}
			else if (p.y >= cell * 40)
			{
				p.y = cell * 39;
				p.x += cell;
				if (p.x >= cell * 40) p.x = 0;
				turn = 3;
			}
		}
		else if (isA == true)
		{
			if (turn == 1)         p.x += cell;   // 오른쪽
			else if (turn == 2)    p.x -= cell;   // 왼쪽
			if (p.x >= cell * 40)
			{
				p.x = cell * 39;
				p.y += cell;
				if (p.y >= cell * 40) p.y = 0;
				turn = 2;
			}
			else if (p.x < 0)
			{
				p.x = 0;
				p.y += cell;
				if (p.y >= cell * 40) p.y = 0;
				turn = 1;
			}
		}
		if (jump1 == true)
		{
			jump++;
			if (jump == 1)
			{
				turn = 3;
			}
			else if (jump == 2)
			{
				turn = 1;
			}
			else if (jump == 4)
			{
				turn = 4;
			}
			else if (jump == 5)
			{
				turn = 1;
			}
			else if(jump>5)
			{
				jump = 0;
				jump1 = false;
			}
		}
		else if (jump2 == true)
		{
			jump++;
			if (jump == 1)
			{
				turn = 3;
			}
			else if (jump == 2)
			{
				turn = 2;
			}
			else if (jump == 4)
			{
				turn = 4;
			}
			else if (jump == 5)
			{
				turn = 2;
			}
			else if (jump > 5)
			{
				jump = 0;
				jump2 = false;
			}
		}
		else if (jump3 == true)
		{
			jump++;
			if (jump == 1)
			{
				turn = 1;
			}
			else if (jump == 2)
			{
				turn = 3;
			}
			else if (jump == 4)
			{
				turn = 2;
			}
			else if (jump == 5)
			{
				turn = 3;
			}
			else if (jump > 5)
			{
				jump = 0;
				jump3 = false;
			}
		}
		else if (jump4 == true)
		{
			jump++;
			if (jump == 1)
			{
				turn = 2;
			}
			else if (jump == 2)
			{
				turn = 4;
			}
			else if (jump == 4)
			{
				turn = 1;
			}
			else if (jump == 5)
			{
				turn = 4;
			}
			else if (jump > 5)
			{
				jump = 0;
				jump4 = false;
			}
		}

		// 4. 충돌 처리 (한 번만)
		int idx = isCollision(p.x, p.y,prevX,prevY,cell, f);

		if (idx != -1)
		{
			// ⭐ A 모드: 무조건 꼬리로
			if (isA)
			{
				for (int i = 0;i < 20;i++)
				{
					f[i].moving1 = true;
					f[i].moving2 = false;
					f[i].move = -1;
				}
				if (tailCount < 20)
				{
					f[idx].moving1 = false;
					f[idx].moving2 = true;
					f[idx].move = -1;
					p.RGB = f[idx].RGB;
					tail[tailCount] = idx;
					f[idx].size = tailCount;
					tailCount++;
				}
			}
			else
			{
				// 기존 로직 유지
				if (f[idx].moving1 == false)
				{
					int nx, ny;
					while (1)
					{
						nx = rand() % 40;
						ny = rand() % 40;

						if (nx * cell == p.x && ny * cell == p.y)
							continue;

						if (isOverlap1(nx, ny, f, 20))
							continue;

						break;
					}

					f[idx].x = nx * cell;
					f[idx].y = ny * cell;

					f[idx].moving1 = true;
					f[idx].moving2 = false;
					f[idx].move = rand() % 5;
					p.RGB = f[idx].RGB;
				}
				else if (f[idx].moving1 == true && f[idx].moving2 == false)
				{
					f[idx].moving1 = false;
					f[idx].moving2 = true;

					if (tailCount < 20)
					{
						tail[tailCount] = idx;
						f[idx].size = tailCount;
						tailCount++;
					}
				}
			}
		}

		// 5. 꼬리 이동 (한 번만)
		int tx = prevX;
		int ty = prevY;

		for (int i = 0; i < tailCount; i++)
		{
			int t = tail[i];
			f[t].RGB = p.RGB;
			int tempX = f[t].x;
			int tempY = f[t].y;

			f[t].x = tx;
			f[t].y = ty;

			tx = tempX;
			ty = tempY;
		}

		for (int i = 0;i < 20;i++) // 아이템 움직임
		{
			if (f[i].moving1 == true)
			{
				if (f[i].move == 0)
				{
					if (f[i].end == 1)
					{
						f[i].x += cell;
						if (f[i].x >= cell * 39)
							f[i].end = 2;
					}
					else if (f[i].end == 2)
					{
						f[i].x -= cell;
						if (f[i].x <= 0)
							f[i].end = 1;
					}
				}
				else if (f[i].move == 1)
				{
					if (f[i].end == 1)
					{
						f[i].y += cell;
						if (f[i].y >= cell * 39)
							f[i].end = 2;
					}
					else if (f[i].end == 2)
					{
						f[i].y -= cell;
						if (f[i].y <= 0)
							f[i].end = 1;
					}
				}
				else if (f[i].move == 2)
				{
					if (f[i].x + cell <= cell * 39 && f[i].end == 1)
					{
						f[i].x += cell;
						f[i].end++;
					}
					else if (f[i].end == 2 && f[i].y + cell <= cell * 39)
					{
						f[i].y += cell;
						f[i].end++;
					}
					else if (f[i].end == 3 && f[i].x + cell > 0)
					{
						f[i].x -= cell;
						f[i].end++;
					}
					else if (f[i].end == 4 && f[i].y + cell > 0)
					{
						f[i].y -= cell;
						f[i].end = 1;
					}
				}
				else if (f[i].move == 4)
				{
					if (f[i].end == 1)
					{
						if (f[i].size < 5)
							f[i].size++;
						else
							f[i].end = 2;
					}
					else if (f[i].end == 2)
					{
						if (f[i].size > -5)
							f[i].size--;
						else
							f[i].end = 1;
					}

				}
			}
		}

		if (isA && p.x == Ax && p.y == Ay)
		{
			isA = false;

			speed = 250;
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, speed, NULL);
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		for (int i = 0; i <= 40; i++)
		{
			MoveToEx(hDC, 0, i * cell, NULL);   // 시작점
			LineTo(hDC, cell * 40, i * cell);        // 가로선
			MoveToEx(hDC, i * cell, 0, NULL);   // 시작점
			LineTo(hDC, i * cell, cell * 40);        // 세로선
		}

		hBrush = CreateSolidBrush(p.RGB); // 플레이어 그리기 
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		if(p.shape==1)
			Ellipse(hDC, p.x, p.y, p.x + cell, p.y + cell);
		if (p.shape == 2)
		{
			POINT point[3]{ {p.x+cell/2,p.y},{p.x,p.y+cell},{p.x+cell,p.y+cell}};
			Polygon(hDC, point, 3);
		}
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		for (int i = 0;i < 20;i++)
		{
			if (f[i].active == true)
			{
				hBrush = CreateSolidBrush(f[i].RGB); // 먹이 그리기
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, f[i].x + f[i].size, f[i].y + f[i].size, f[i].x + cell - f[i].size, f[i].y + cell - f[i].size);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}
		}

		for (int i = 0;i < obCount;i++)
		{
			hBrush = CreateSolidBrush(RGB(255,0,0)); // 장애물 그리기
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Rectangle(hDC, ob[i].x, ob[i].y, ob[i].x + cell, ob[i].y + cell);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == '+')
		{
			if(speed>=50)
				speed -= 50;
		}
		else if (wParam == '-')
		{
			if(speed<500)
				speed += 50;
		}
		KillTimer(hWnd, 1);
		SetTimer(hWnd, 1, speed, NULL);

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP)
		{
			if(turn!=4)
				turn = 3;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_DOWN)
		{
			if(turn!=3)
				turn = 4;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_LEFT)
		{
			if(turn!=1)
				turn = 2;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_RIGHT)
		{
			if(turn!=2)
				turn = 1;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'S')
		{
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, speed, NULL);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'T')
		{
			if (tailCount > 0)
			{
				int oldPx = p.x;
				int oldPy = p.y;

				// 1. 첫 꼬리
				int first = tail[0];

				// 2. 플레이어를 A로 이동
				p.x = f[first].x;
				p.y = f[first].y;
				p.RGB = f[first].RGB;

				// 3. 꼬리 좌표 한 칸씩 앞으로 (핵심)
				for (int i = 0; i < tailCount - 1; i++)
				{
					int cur = tail[i];
					int next = tail[i + 1];

					f[cur].x = f[next].x;
					f[cur].y = f[next].y;
				}

				// 4. 마지막 꼬리에 기존 플레이어 위치 넣기
				int last = tail[tailCount - 1];
				f[last].x = oldPx;
				f[last].y = oldPy;

				// 5. 꼬리 상태 강제 고정 (움직이는거 방지)qk
				for (int i = 0; i < tailCount; i++)
				{
					int t = tail[i];
					f[t].moving1 = false;
					f[t].moving2 = true;
					f[t].move = -1;
				}
			}


			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'A')
		{
			isA = !isA;
			if (isA)
			{
				turn = 1;
				Ax = p.x;
				Ay = p.y;
				speed = 10;
			}
			else
			{
				speed = 250;
			}

			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, speed, NULL);
		}
		else if (wParam == 'J')
		{
			if (turn == 1)//오른쪽 방향
			{
				jump1 = true;
			}
			else if (turn == 2)
			{
				jump2 = true;
			}
			else if (turn == 3)
			{
				jump3 = true;
			}
			else if (turn == 4)
			{
				jump4 = true;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_RBUTTONDOWN:
		if (obCount < 20)
		{
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);

			ob[obCount].x = (mx / cell) * cell;
			ob[obCount].y = (my / cell) * cell;
			obCount++;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		int mx = LOWORD(lParam);
		int my = HIWORD(lParam);

		if (mx > p.x && (turn == 3||turn==4))
		{
			turn = 1;
		}
		else if (mx < p.x && (turn == 3 || turn == 4))
		{
			turn = 2;
		}
		else if (my > p.y && (turn == 2 || turn == 1))
		{
			turn = 4;
		}
		else if (my < p.y && (turn == 2 || turn == 1))
		{
			turn = 3;
		}

		if (mx >= p.x && mx <= p.x + cell && my >= p.y && my <= p.y + cell)
		{
			if (p.shape == 1)
			{
				p.shape = 2;
				for (int i = 0; i < tailCount; i++)
				{
					int t = tail[i];
					f[t].active = false;
					f[t].moving1 = false;
					f[t].moving2 = false;
					f[t].size = 0;
				}
				tailCount = 0;
			}
			else
				p.shape = 1;
		}

		int cx = (mx / cell) * cell;
		int cy = (my / cell) * cell;

		for (int i = 0; i < tailCount; i++)
		{
			int idx = tail[i];
			if (f[idx].x == cx && f[idx].y == cy) 
			{
				for (int j = i; j < tailCount;j++)
				{
					int del = tail[j];
					f[del].moving1 = true;
					f[del].moving2 = false;
					f[del].move = rand() % 5;
					f[del].size = 0;

				}
				tailCount = i;
				break;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}


