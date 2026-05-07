	#include <windows.h> 
	#include <tchar.h> 
	#include <time.h> 
	#include <stdbool.h> 
	#include "resource.h"
#define PHOTOSIZE 200
	HINSTANCE g_hInst;
	LPCTSTR lpszClass = L"WindowClass";
	LPCTSTR lpszWindowName = L"Random Text Program";

	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	int winx = 700, winy = 700;

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
		WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
		WndClass.lpszClassName = lpszClass;
		WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		RegisterClassEx(&WndClass);

		hWnd = CreateWindow(
			lpszClass,
			lpszWindowName,
			WS_OVERLAPPEDWINDOW,
			0, 0, winx, winy,
			NULL, NULL,
			hInstance,
			NULL
		);

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		while (GetMessage(&Message, 0, 0, 0))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		return (int)Message.wParam;
	}

	struct Photo {
		int x, y,position;
	};

	struct Position {
		int x, y;
	};

	struct Photo photo3[9];
	struct Position position3[9];
	static int photo = 1, divide = 0, divpos3 = 0, emptyIndex = -1;
	void shuffle3()
	{
		for (int i = 8; i > 0; i--)
		{
			int j = rand() % (i + 1);

			// position3[i] <-> position3[j] swap
			struct Position temp = position3[i];
			position3[i] = position3[j];
			position3[j] = temp;
		}
	}
	int findPieceIndex(int x, int y, RECT rect)
	{
		int dw = rect.right / 3;
		int dh = rect.bottom / 3;

		for (int i = 0; i < 9; i++)
		{
			if (i == emptyIndex) continue;

			int px = position3[i].x;
			int py = position3[i].y;

			if (x >= px && x < px + dw &&
				y >= py && y < py + dh)
			{
				return i;  // 👉 이 위치에 있는 조각
			}
		}
		return -1;
	}

	void getGridPos(int x, int y, int* col, int* row, RECT rect)
	{
		int dw = rect.right / 3;
		int dh = rect.bottom / 3;

		*col = x / dw;
		*row = y / dh;
	}
	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		HDC hDC, mDC, hMemDC;
		static BITMAP bmp1, bmp2;
		static HBITMAP hBitmap1, hBitmap2;
		HPEN hPen, oldPen;
		HBRUSH hBrush, oldBrush;
		PAINTSTRUCT ps;
		RECT rect;
		static bool isA = false, isR = false, isF = false;
		static int startGridX, startGridY;
		static bool isDragging = false;
		static int dragIndex = -1;
		static int startX, startY;
		GetClientRect(hWnd, &rect);

		switch (iMessage)
		{
		case WM_CREATE:
		{
			hBitmap1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			GetObject(hBitmap1, sizeof(BITMAP), &bmp1);
			hBitmap2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(103));
			GetObject(hBitmap2, sizeof(BITMAP), &bmp2);
			for (int i = 0;i < 9;i++)
			{
				photo3[i].x = PHOTOSIZE / 3 * (i % 3);
				photo3[i].y = PHOTOSIZE / 3 * (i / 3);
				photo3[i].position = i;
				position3[i].x = rect.right / 3 * (i % 3);
				position3[i].y = rect.bottom / 3 * (i / 3);
			}
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case ID_PHOTO_1:
				photo = 1;
				break;
			case ID_PHOTO_2:
				photo = 2;
				break;
			case ID_DIVIDE_3:
				divide = 3;
				shuffle3();
				break;
			case ID_DIVIDE_4:
				break;
			case ID_DIVIDE_5:
				break;
			case ID_GAME_START:
				if (divide == 3)
				{
					emptyIndex = rand() % 9;
				}
				break;
			case ID_GAME_SHOW:
				break;
			case ID_GAME_END:
				break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		case WM_PAINT:
		{
			hDC = BeginPaint(hWnd, &ps);
			hMemDC = CreateCompatibleDC(hDC);
			if (photo == 1)
			{
				SelectObject(hMemDC, hBitmap1);
				if (divide == 3)
				{
					if (isF == true)
					{
						StretchBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, bmp1.bmWidth, bmp1.bmHeight, SRCCOPY);
					}
					else
					{
						for (int i = 0;i < 9;i++)
						{
							if (i == emptyIndex)continue;
							StretchBlt(hDC, position3[i].x, position3[i].y, rect.right / 3, rect.bottom / 3, hMemDC, photo3[i].x, photo3[i].y, PHOTOSIZE / 3, PHOTOSIZE / 3, SRCCOPY);
						}
					}
				}
				else
				{
					StretchBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, bmp1.bmWidth, bmp1.bmHeight, SRCCOPY);
				}
			}
			else
			{
				SelectObject(hMemDC, hBitmap2);
				StretchBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, bmp2.bmWidth, bmp2.bmHeight, SRCCOPY);
			}
			DeleteDC(hMemDC);

			EndPaint(hWnd, &ps);
			break;
		}

		case WM_DESTROY:
			DeleteObject(hBitmap1);
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		{
			if (wParam == 'Q')
				PostQuitMessage(0);
			else if (wParam == '1')
				photo = 1;
			else if (wParam == '2')
				photo = 2;
			else if (wParam == 'S')
			{
				if (divide == 3)
				{
					emptyIndex = rand() % 9;
				}
			}
			else if (wParam == 'F')
			{
				if (isF)
					isF = false;
				else
					isF = true;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		case WM_LBUTTONDOWN:
		{
			if (emptyIndex == -1) break;

			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			int clickIndex = findPieceIndex(x, y, rect);
			if (clickIndex == -1) break;

			int cx, cy, ex, ey;

			getGridPos(position3[clickIndex].x, position3[clickIndex].y, &cx, &cy, rect);
			getGridPos(position3[emptyIndex].x, position3[emptyIndex].y, &ex, &ey, rect);

			if ((abs(cx - ex) == 1 && cy == ey) ||
				(abs(cy - ey) == 1 && cx == ex))
			{
				isDragging = true;
				dragIndex = clickIndex;
				startX = x;
				startY = y;
			}
			getGridPos(position3[clickIndex].x, position3[clickIndex].y, &startGridX, &startGridY, rect);
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (!isDragging) break;

			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			int dx = x - startX;
			int dy = y - startY;

			int dw = rect.right / 3;
			int dh = rect.bottom / 3;

			int cx, cy, ex, ey;

			// 🔥 현재 위치 기준으로 계산
			getGridPos(position3[dragIndex].x, position3[dragIndex].y, &cx, &cy, rect);
			getGridPos(position3[emptyIndex].x, position3[emptyIndex].y, &ex, &ey, rect);

			int curX = position3[dragIndex].x;
			int curY = position3[dragIndex].y;

			int originX = cx * dw;
			int originY = cy * dh;

			int emptyX = ex * dw;
			int emptyY = ey * dh;

			if (cy == ey) // 좌우
			{
				dy = 0;

				int minX = originX;
				int maxX = emptyX;

				if (minX > maxX)
				{
					int t = minX;
					minX = maxX;
					maxX = t;
				}

				curX += dx;

				if (curX < minX) curX = minX;
				if (curX > maxX) curX = maxX;

				position3[dragIndex].x = curX;
			}
			else if (cx == ex) // 상하
			{
				dx = 0;

				int minY = originY;
				int maxY = emptyY;

				if (minY > maxY)
				{
					int t = minY;
					minY = maxY;
					maxY = t;
				}

				curY += dy;

				if (curY < minY) curY = minY;
				if (curY > maxY) curY = maxY;

				position3[dragIndex].y = curY;
			}

			startX = x;
			startY = y;

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		case WM_LBUTTONUP:
		{
			if (!isDragging) break;

			int dw = rect.right / 3;
			int dh = rect.bottom / 3;

			int ex = emptyIndex % 3;
			int ey = emptyIndex / 3;

			int cx, cy;

			getGridPos(position3[dragIndex].x, position3[dragIndex].y, &cx, &cy, rect);
			getGridPos(position3[emptyIndex].x, position3[emptyIndex].y, &ex, &ey, rect);

			// 기준 거리
			int moved = 0;

			if (cx == ex) // 상하
				moved = abs(position3[dragIndex].y - (startGridY * dh));
			else
				moved = abs(position3[dragIndex].x - (startGridX * dw));

			if (moved > dh / 2 || moved > dw / 2)
			{
				// 👉 swap
				struct Position temp = position3[dragIndex];
				position3[dragIndex] = position3[emptyIndex];
				position3[emptyIndex] = temp;

				emptyIndex = dragIndex;
			}
			else
			{
				// 👉 원위치 복구
				position3[dragIndex].x = (dragIndex % 3) * dw;
				position3[dragIndex].y = (dragIndex / 3) * dh;
			}

			isDragging = false;
			dragIndex = -1;

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		}
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}