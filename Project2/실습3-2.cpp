#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"WindowClass";
LPCTSTR lpszWindowName = L"Windowprgraming";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int winx = 1000, winy = 1100;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;

	g_hInst = hInstance;

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

struct Part {
	int x, y, shape, speed, angle, mx, my,target;
	double tx, ty;
	bool turn, isShape = false,check=false;

};

struct Part part[4];
struct Part temp;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	RECT rect;
	static int r, g, b;
	COLORREF setcolor = RGB(r, g, b);
	static int ASIZE = 100, BSIZE = 200, CSIZE = 12, DSIZE = 8, select = 0,checkx,checky;
	static bool isRclick = false, isSpeed = false;
	GetClientRect(hWnd, &rect);
	switch (iMsg)
	{
	case WM_CREATE:
		part[0].x = rect.right / 4;
		part[0].y = rect.bottom / 4;
		part[1].x = rect.right / 4 * 3;
		part[1].y = rect.bottom / 4;
		part[2].x = rect.right / 4;
		part[2].y = rect.bottom / 4 * 3;
		part[3].x = rect.right / 4 * 3;
		part[3].y = rect.bottom / 4 * 3;
		for (int i = 0; i < 4; i++)
		{
			part[i].shape = 0;
			part[i].speed = 1;
			if (i % 2 == 0)
				part[i].turn = true;
			else
				part[i].turn = false;
			SetTimer(hWnd, i, 10, NULL);
		}
		for (int i = 0;i < 4;i++)
		{
			part[i].mx = part[i].x + ASIZE;
			part[i].my = part[i].y - ASIZE;
			part[i].tx = part[i].x;
			part[i].ty = part[i].y - ASIZE;
			part[i].target = 1;
		}
		SetTimer(hWnd, 5, 500, NULL);
		break;
	case WM_TIMER:
		if (wParam == 5)
		{
			r = rand()%255;
			g = rand() % 255;
			b = rand() % 255;
		}
		for (int i = 0;i < 4;i++)
		{
			if (part[i].shape == 0)
			{
				if (part[i].turn == true)
					part[i].angle += part[i].speed;
				else if (part[i].turn == false)
					part[i].angle -= part[i].speed;
			}
			else if (part[i].shape == 1)
			{
				if (part[i].mx >= part[i].x + ASIZE) // 오른쪽 벽
				{
					part[i].mx = part[i].x + ASIZE;
					if (part[i].turn == true)
						part[i].my += part[i].speed;
					else if (part[i].turn == false)
						part[i].my -= part[i].speed;
				}

				if (part[i].my >= part[i].y + ASIZE) // 아래 벽
				{
					part[i].my = part[i].y + ASIZE;
					if (part[i].turn == true)
						part[i].mx -= part[i].speed;
					else if (part[i].turn == false)
						part[i].mx += part[i].speed;
				}

				if (part[i].mx <= part[i].x - ASIZE) // 왼쪽 벽
				{
					part[i].mx = part[i].x - ASIZE;
					if (part[i].turn == true)
						part[i].my -= part[i].speed;
					else if (part[i].turn == false)
						part[i].my += part[i].speed;
				}

				if (part[i].my <= part[i].y - ASIZE) // 위쪽 벽
				{
					part[i].my = part[i].y - ASIZE;
					if (part[i].turn == true)
						part[i].mx += part[i].speed;
					else if (part[i].turn == false)
						part[i].mx -= part[i].speed;
				}
			}
			else if (part[i].shape == 2)
			{
				POINT tri[3] = {{part[i].x, part[i].y - ASIZE}, {part[i].x - (int)(ASIZE * sqrt(3) / 2), part[i].y + ASIZE / 2}, {part[i].x + (int)(ASIZE * sqrt(3) / 2), part[i].y + ASIZE / 2}};

				int dx = tri[part[i].target].x - part[i].tx;
				int dy = tri[part[i].target].y - part[i].ty;

				double dist = sqrt(dx * dx + dy * dy);

				if (dist == 0)
				{
					if (part[i].turn == true)
						part[i].target = (part[i].target + 1) % 3;
					else
						part[i].target = (part[i].target + 2) % 3;

					continue;
				}

				if (dist < part[i].speed)
				{
					part[i].tx = tri[part[i].target].x;
					part[i].ty = tri[part[i].target].y;

					if (part[i].turn == true)
						part[i].target = (part[i].target + 1) % 3;
					else
						part[i].target = (part[i].target + 2) % 3;
				}
				else
				{
					part[i].tx += (part[i].speed * dx / dist);
					part[i].ty += (part[i].speed * dy / dist);
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		hBrush = CreateSolidBrush(setcolor);
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		for (int i = 0; i < 4; i++) //4분면에 작은 빨간 원 그리기 
		{
			if (part[i].check == true) SetROP2(hDC, R2_MASKNOTPEN);
			else SetROP2(hDC, R2_COPYPEN);
			Ellipse(hDC, part[i].x - CSIZE, part[i].y - CSIZE, part[i].x + CSIZE, part[i].y + CSIZE);
		}
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hDC, hPen);
		oldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		for (int i = 0; i < 4; i++) //  궤도 그리기 
		{
			if (part[i].check == true) SetROP2(hDC, R2_MASKNOTPEN);
			else SetROP2(hDC, R2_COPYPEN);

			if (part[i].shape == 0)
				Ellipse(hDC, part[i].x - ASIZE, part[i].y - ASIZE, part[i].x + ASIZE, part[i].y + ASIZE);
			else if (part[i].shape == 1)
				Rectangle(hDC, part[i].x - ASIZE, part[i].y - ASIZE, part[i].x + ASIZE, part[i].y + ASIZE);
			else if (part[i].shape == 2)
			{
				POINT point[6] = { {part[i].x,part[i].y - ASIZE},{part[i].x - (int)(ASIZE * sqrt(3) / 2),part[i].y + ASIZE / 2},{part[i].x + (int)(ASIZE * sqrt(3) / 2),part[i].y + ASIZE / 2} };
				Polygon(hDC, point, 3);
			}
		}
		SelectObject(hDC, oldBrush);
		SelectObject(hDC, oldPen);
		DeleteObject(hPen);

		hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0)); //셀렉된 네모 그리기 
		oldPen = (HPEN)SelectObject(hDC, hPen);
		oldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
		if (part[select].check == true) SetROP2(hDC, R2_MASKNOTPEN);
		else SetROP2(hDC, R2_COPYPEN);
		Rectangle(hDC, part[select].x - BSIZE, part[select].y - BSIZE, part[select].x + BSIZE, part[select].y + BSIZE);
		SelectObject(hDC, oldPen);
		SelectObject(hDC, oldBrush);
		DeleteObject(hPen);


		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		for (int i = 0; i < 4; i++) //돌아가는 도형 그리기 
		{
			if (part[i].check == true) SetROP2(hDC, R2_MASKNOTPEN);
			else SetROP2(hDC, R2_COPYPEN);
			if (part[i].shape == 0)
			{
				int r = ASIZE;
				double rad = part[i].angle * 3.141592 / 180.0;
				int x = part[i].x + (int)(r * cos(rad));
				int y = part[i].y + (int)(r * sin(rad));

				if (part[i].isShape == false)
					Ellipse(hDC, x - DSIZE, y - DSIZE, x + DSIZE, y + DSIZE);
				else if (part[i].isShape == true)
					Rectangle(hDC, x - DSIZE, y - DSIZE, x + DSIZE, y + DSIZE);
			}
			else if (part[i].shape == 1)
			{
				if (part[i].isShape == false)
					Ellipse(hDC, part[i].mx - DSIZE, part[i].my - DSIZE, part[i].mx + DSIZE, part[i].my + DSIZE);
				else if (part[i].isShape == true)
					Rectangle(hDC, part[i].mx - DSIZE, part[i].my - DSIZE, part[i].mx + DSIZE, part[i].my + DSIZE);
			}
			else if (part[i].shape == 2)
			{
				if (part[i].isShape == false)
					Ellipse(hDC, (int)part[i].tx - DSIZE, (int)part[i].ty - DSIZE, (int)part[i].tx + DSIZE, (int)part[i].ty + DSIZE);
				else if (part[i].isShape == true)
					Rectangle(hDC, (int)part[i].tx - DSIZE, (int)part[i].ty - DSIZE, (int)part[i].tx + DSIZE, (int)part[i].ty + DSIZE);
			}
		}
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		if (isSpeed == false)
		{
			part[select].speed++;
			if (part[select].speed >= 6)
			{
				isSpeed = true;
			}
		}
		else if (isSpeed == true)
		{
			part[select].speed--;
			if (part[select].speed <= 1)
				isSpeed = false;
		}
		break;
	case WM_RBUTTONDOWN:
		if (isRclick == false)
		{
			temp.x = part[select].x;
			temp.y = part[select].y;
			part[select].x = LOWORD(lParam);
			part[select].y = HIWORD(lParam);
			isRclick = true;
		}
		else
		{
			part[select].x = temp.x;
			part[select].y = temp.y;
			isRclick = false;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_RBUTTONDBLCLK:
		checkx = LOWORD(lParam);
		checky = HIWORD(lParam);
		for (int i = 0;i < 4;i++)
		{
			if (checkx >= part[i].x - BSIZE && checkx <= part[i].x + BSIZE && checky >= part[i].y - BSIZE && checky <= part[i].y + BSIZE)
			{
				if (part[i].check == true)
				{
					part[i].check = false;
				}
				else
					part[i].check = true;
			}
		}
		break;
	case WM_MOUSEMOVE:

		break;
	case WM_CHAR:

		break;
	case WM_KEYDOWN:
		if (wParam == '1')
		{
			select = 0;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '2')
		{
			select = 1;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '3')
		{
			select = 2;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == '4')
		{
			select = 3;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'E')
		{
			part[select].shape = 0;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'R')
		{
			part[select].shape = 1;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'T')
		{
			part[select].shape = 2;
			part[select].tx = part[select].x;
			part[select].ty = part[select].y - ASIZE;
			part[select].target = 1;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'C')
		{
			if (part[select].turn == true)
				part[select].turn = false;
			else
				part[select].turn = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'M')
		{
			if (part[select].isShape == true) part[select].isShape = false;
			else if (part[select].isShape == false) part[select].isShape = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'Q')
		{
			PostQuitMessage(0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}