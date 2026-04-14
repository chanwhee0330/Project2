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

static struct Player {
	int x, y,shape,size;
	COLORREF RGB;
};

struct Player p[10];
struct Player temp[10];

bool isCollision(int x, int y, int cell, struct Player p[])
{
	int gridX = x / cell;
	int gridY = y / cell;

	for (int i = 0; i < 20; i++)
	{
		if (p[i].x == gridX && p[i].y == gridY)
			return true;
	}
	return false;
}

bool isOverlap1(int x, int y, struct Player p[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (p[i].x == x && p[i].y == y)
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
	int width/*가로*/, height/*세로*/, cell;
	GetClientRect(hWnd, &rect);
	width = rect.right;
	height = rect.bottom;
	cell = width / 40;
	static int number,select=-1;
	static bool isC=false;
	static bool isselect = false;


	switch (iMessage) {
	case WM_CREATE:
		hDC = BeginPaint(hWnd, &ps);
		for (int i = 0; i <= 40; i++)
		{
			MoveToEx(hDC, 0, i * cell, NULL);   // 시작점
			LineTo(hDC, cell * 40, i * cell);        // 가로선
			MoveToEx(hDC, i * cell, 0, NULL);   // 시작점
			LineTo(hDC, i * cell, cell * 40);        // 세로선
		}
		EndPaint(hWnd, &ps);
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
		for (int i = 0; i < number; i++)
		{
			if (p[i].shape == 0)
			{
					hBrush = CreateSolidBrush(p[i].RGB);
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, p[i].x * cell-p[i].size, p[i].y * cell-p[i].size, p[i].x * cell + cell+p[i].size, p[i].y * cell + cell+p[i].size);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
				
			}
			else if (p[i].shape == 1)
			{
				hBrush = CreateSolidBrush(p[i].RGB);
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				POINT point[6] = { {(p[i].x*cell)+cell/2,p[i].y*cell-p[i].size},{p[i].x * cell-p[i].size,p[i].y * cell + cell+p[i].size},{p[i].x * cell + cell+p[i].size,p[i].y * cell + cell+p[i].size}};
				Polygon(hDC, point, 3);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}
			else if (p[i].shape == 2)
			{
				hBrush = CreateSolidBrush(p[i].RGB);
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, p[i].x*cell-p[i].size, p[i].y * cell-p[i].size, p[i].x * cell + cell+p[i].size, p[i].y * cell + cell+p[i].size);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}
		}

		if (select >= 0)
		{
			if (p[select].shape == 0)
			{
				hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hDC, hPen);
				hBrush = CreateSolidBrush(p[select].RGB);
				HBRUSH oldBrush = (HBRUSH)SelectObject(hDC,hBrush);
				Ellipse(hDC, p[select].x * cell - p[select].size, p[select].y * cell - p[select].size, p[select].x * cell + cell + p[select].size, p[select].y * cell + cell + p[select].size);
				SelectObject(hDC, oldPen);
				SelectObject(hDC, oldBrush);
				DeleteObject(hPen);
			}
			else if (p[select].shape == 1)
			{
				hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hDC, hPen);
				hBrush = CreateSolidBrush(p[select].RGB);
				HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				POINT point[6] = { {(p[select].x * cell) + cell / 2,p[select].y * cell-p[select].size},{p[select].x * cell-p[select].size,p[select].y * cell + cell+p[select].size},{p[select].x * cell + cell+p[select].size,p[select].y * cell + cell+p[select].size}};
				Polygon(hDC, point, 3);
				SelectObject(hDC, oldPen);
				SelectObject(hDC, oldBrush);
				DeleteObject(hPen);
			}
			else if (p[select].shape == 2)
			{
				hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hDC, hPen);
				hBrush = CreateSolidBrush(p[select].RGB);
				HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, p[select].x * cell-p[select].size, p[select].y * cell-p[select].size, p[select].x * cell + cell+p[select].size, p[select].y * cell + cell+p[select].size);
				SelectObject(hDC, oldPen);
				SelectObject(hDC, oldBrush);
				DeleteObject(hPen);
			}
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == '+')
		{
			if (select >= 0 && select < number)
			{
				if (p[select].size < 3)
					p[select].size++;
			}
		}
		else if (wParam == '-')
		{
			if (select >= 0 && select < number)
			{
				if (p[select].size > -2)
					p[select].size--;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		if (wParam == 'Q')
		{
			PostQuitMessage(0);
		}
		else if (wParam == 'E')
		{
			p[number].shape = 0;
			for (int i = 0; i < number; i++)
			{
				while (1)
				{
					int x = rand() % 40;
					int y = rand() % 40;
					p[number].RGB = RGB(rand() % 255, rand() % 255, rand() % 255);

					if (isOverlap1(x, y, p, i))
						continue;

					p[number].size = 0;
					p[number].x = x;
					p[number].y = y;
					break;
				}
			}

			if (isselect == true)
			{
				select = number;
				isselect = false;
			}
			else
			{
				select++; //테두리 설정
			}
			if (select > 8)
				select = 9;
			if(number<11) //도형 개수 설정
				number++;
			if (number == 11)
			{
				for (int i = 0; i < 9; i++)
				{
					p[i] = p[i + 1];
				}
				number = 10;
				for (int i = 0; i < 10; i++)
				{
					while (1)
					{
						int x = rand() % 40;
						int y = rand() % 40;
						p[9].RGB = RGB(rand() % 255, rand() % 255, rand() % 255);

						if (isOverlap1(x, y, p, i))
							continue;
						p[9].size = 0;
						p[9].shape = 0;
						p[9].x = x;
						p[9].y = y;
						break;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'T')
		{
			p[number].shape = 1;
			for (int i = 0; i < number; i++)
			{
				while (1)
				{
					int x = rand() % 40;
					int y = rand() % 40;
					p[number].RGB = RGB(rand() % 255, rand() % 255, rand() % 255);

					if (isOverlap1(x, y, p, i))
						continue;

					p[number].size = 0;
					p[number].x = x;
					p[number].y = y;
					break;
				}
			}

			if (isselect == true)
			{
				select = number;
				isselect = false;
			}
			else
			{
				select++; //테두리 설정
			}
			if (select > 8)
				select = 9;
			if (number < 11) //도형 개수 설정
				number++;
			if (number == 11)
			{
				for (int i = 0; i < 9; i++)
				{
					p[i] = p[i + 1];
				}
				number = 10;
				for (int i = 0; i < 10; i++)
				{
					while (1)
					{
						int x = rand() % 40;
						int y = rand() % 40;
						p[9].RGB = RGB(rand() % 255, rand() % 255, rand() % 255);

						if (isOverlap1(x, y, p, i))
							continue;
						p[9].size = 0;
						p[9].shape = 1;
						p[9].x = x;
						p[9].y = y;
						break;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'R')
		{
			p[number].shape = 2;
			for (int i = 0; i < number; i++)
			{
				while (1)
				{
					int x = rand() % 40;
					int y = rand() % 40;
					p[number].RGB = RGB(rand() % 255, rand() % 255, rand() % 255);

					if (isOverlap1(x, y, p, i))
						continue;

					p[number].size = 0;
					p[number].x = x;
					p[number].y = y;
					break;
				}
			}

			if (isselect == true)
			{
				select = number;
				isselect = false;
			}
			else
			{
				select++; //테두리 설정
			}
			if (select > 8)
				select = 9;
			if (number < 11) //도형 개수 설정
				number++;
			if (number == 11)
			{
				for (int i = 0; i < 9; i++)
				{
					p[i] = p[i + 1];
				}
				number = 10;
				for (int i = 0; i < 10; i++)
				{
					while (1)
					{
						int x = rand() % 40;
						int y = rand() % 40;
						p[9].RGB = RGB(rand() % 255, rand() % 255, rand() % 255);

						if (isOverlap1(x, y, p, i))
							continue;
						
						p[9].size = 0;
						p[9].shape = 2;
						p[9].x = x;
						p[9].y = y;
						break;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'D')
		{
			for (int i = select; i < 9; i++)
			{
				p[i] = p[i + 1];
			}
			select--;
			number--;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'C')
		{
			if (isC == false)
			{
				for (int i = 0; i < number; i++)
				{
					temp[i] = p[i];
				}
					int targetshape = p[select].shape;
					for (int i = 0; i < number; i++)
					{
						if (p[i].shape == targetshape)
						{
							int ran = 0;
							while (1)
							{
								ran = rand() % 3;
								if (ran == targetshape)
									continue;
								break;
							}
							p[i].shape = ran;
							p[i].RGB = p[select].RGB;
						}
					}
				isC = true;
			}
			else if (isC == true)
			{
				for (int i = 0; i < number; i++)
				{
					p[i] = temp[i];
				}
				isC = false;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'P')
		{
			number = 0;
			select = -1;
			isC = false;
			isselect = false;

			for (int i = 0; i < 10; i++)
			{
				p[i].x = 0;
				p[i].y = 0;
				p[i].shape = 0;
				p[i].size = 0;
				p[i].RGB = RGB(0, 0, 0);
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '1')
		{
			select = 0;
			isselect = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '2')
		{
			select = 1;
			isselect = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '3')
		{
			select = 2;
			isselect = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '4')
		{
			select = 3;
			isselect = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '5')
		{
			select = 4;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '6')
		{
			select = 5;
			isselect = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '7')
		{
			select = 6;
			isselect = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '8')
		{
			select = 7;
			isselect = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '9')
		{
			select = 8;
			isselect = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '0')
		{
			select = 9;
			isselect = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_UP)
		{
			if (p[select].y > 0)
			{
				p[select].y=p[select].y -1;
			}
			else if (p[select].y == 0)
			{
				p[select].y = 39;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_DOWN)
		{
			if (p[select].y < 39)
			{
				p[select].y = p[select].y + 1;
			}
			else if (p[select].y == 39)
			{
				p[select].y = 0;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_LEFT)
		{
			if (p[select].x > 0)
			{
				p[select].x = p[select].x - 1;
			}
			else if (p[select].x == 0)
			{
				p[select].x = 39;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == VK_RIGHT)
		{
			if (p[select].x < 39)
			{
				p[select].x = p[select].x + 1;
			}
			else if (p[select].x == 39)
			{
				p[select].x = 0;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}


