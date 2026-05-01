	#include <windows.h> 
	#include <tchar.h> 
	#include <time.h> 
	#include <stdbool.h> 
	#include "resource2.h" 

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

	struct Block {
		int x, y;
		bool hit;
		COLORREF RGB;
	};

	struct Row {
		struct Block block[10];
		int way;
		int starty;
		COLORREF RGB;
	};

	struct Row row[3];

	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		HDC hDC, mDC;
		HBITMAP hBitmap;
		PAINTSTRUCT ps;
		HBRUSH hBrush, oldBrush;
		HPEN hPen, oldPen;
		RECT rect;
		static SYSTEMTIME curTime, startTime;
		static TCHAR str[100],str2[100];
		static RECT player;
		static int x, ballX, ballY, ballSpeedx, ballSpeedy, ballspeed = 1, countHit = 0,playTime;
		static bool isClick = false, isT = false,isP=false;
		GetClientRect(hWnd, &rect);

		switch (iMessage)
		{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case ID_MENU_START:
				KillTimer(hWnd, 2);
				SetTimer(hWnd, 2, 10, NULL);
				break;
			case ID_MENU_PAUSE:
				if (isP == false)
				{
					KillTimer(hWnd, 1);
					KillTimer(hWnd, 2);
					isP = true;
				}
				else
				{
					SetTimer(hWnd, 1, 10, NULL);
					SetTimer(hWnd, 2, 10, NULL);
					isP = false;
				}
				break;
			case ID_MENU_TIME:
				if (isT == false)
				{
					isT = true;
				}
				else
				{
					isT = false;
				}
				break;
			case ID_MENU_RESET:
				for (int i = 0;i < 3;i++)
				{
					if (i % 2 == 0)
					{
						row[i].way = -1;
					}
					else
						row[i].way = 1;
					row[i].starty = i * 40;
					row[0].RGB = RGB(200, 100, 100);
					row[1].RGB = RGB(100, 200, 100);
					row[2].RGB = RGB(100, 100, 200);
					for (int j = 0;j < 10;j++)
					{
						row[i].block[j].y = row[i].starty;
						row[i].block[j].x = rect.left + j * 80 + 100;
						row[i].block[j].RGB = row[i].RGB;
						row[i].block[j].hit = false;
					}
				}
				ballX = rect.right / 2 - 40;
				ballY = 700;
				ballSpeedy = 5;
				ballSpeedx = 0;
				ballspeed = 1;
				countHit = 0;
				playTime = 0;
				isClick = false, isT = false, isP = false;
				x = rect.right / 2 - 100;
				player = { x,900,x + 200,950 };
				SetTimer(hWnd, 1, 10, NULL);
				SetTimer(hWnd, 3, 1000, NULL);
				GetLocalTime(&startTime);
				break;
			case ID_SPEED_MINERS:
				if (ballspeed > 1)
					ballspeed--;
				break;
			case ID_SPEED_PLUS:
				if (ballspeed <= 3)
					ballspeed++;
				break;
			case ID_MENU_QUIT:
			{
				int answer = MessageBox(
					hWnd,
					L"프로그램을 종료하시겠습니까?",
					L"끝내기 선택",
					MB_YESNO
				);

				if (answer == IDYES)
					PostQuitMessage(0);
			}
			break;
			}
			break;

		case WM_CREATE:
			for (int i = 0;i < 3;i++)
			{
				if (i % 2 == 0)
				{
					row[i].way = -1;
				}
				else
					row[i].way = 1;
				row[i].starty = i * 40;
				row[0].RGB = RGB(200, 100, 100);
				row[1].RGB = RGB(100, 200, 100);
				row[2].RGB = RGB(100, 100, 200);
				for (int j = 0;j < 10;j++)
				{
					row[i].block[j].y = row[i].starty;
					row[i].block[j].x = rect.left + j * 80 + 100;
					row[i].block[j].RGB = row[i].RGB;
					row[i].block[j].hit = false;
				}
			}
			ballX = rect.right / 2 - 40;
			ballY = 700;
			ballSpeedy = 5;
			ballSpeedx = 0;
			ballspeed = 1;
			countHit = 0;
			playTime = 0;
			isClick = false, isT = false, isP = false;
			x = rect.right / 2 - 100;
			player = { x,900,x + 200,950 };
			SetTimer(hWnd, 1, 10, NULL);
			SetTimer(hWnd, 3, 1000, NULL);
			GetLocalTime(&startTime);
			break;
		case WM_TIMER:
		{
			if (wParam == 1)
			{
				for (int i = 0;i < 3;i++)
				{
					if (row[i].block[0].x <= 0)
						row[i].way *= -1;
					else if (row[i].block[9].x + 80 >= rect.right)
						row[i].way *= -1;
					for (int j = 0;j < 10;j++)
					{
						if (row[i].block[j].hit == false)
							row[i].block[j].x += row[i].way;
						else
						{
							row[i].block[j].x += row[i].way;
							row[i].block[j].y += 40;
							row[i].block[j].RGB = RGB(rand() % 255, rand() % 255, rand() % 255);
						}
					}
				}
			}

			if (wParam == 2)
			{
				ballY += ballSpeedy * ballspeed;
				ballX += ballSpeedx * ballspeed;
				// 좌우 벽
				if (ballX <= 0 || ballX + 40 >= rect.right)
				{
					ballSpeedx *= -1;
				}

				// 위쪽
				if (ballY <= 0)
				{
					ballSpeedy *= -1;
				}

				if (ballY + 40 >= player.top &&
					ballY <= player.bottom &&
					ballX + 40 >= player.left &&
					ballX <= player.right)
				{
					ballSpeedy *= -1;

					int center = (player.left + player.right) / 2;
					int ballCenter = ballX + 20;

					int diff = ballCenter - center;

					ballSpeedx = diff / 10; // 조절 포인트
				}

				bool hit = false;

				for (int i = 0; i < 3 && !hit; i++)
				{
					for (int j = 0; j < 10 && !hit; j++)
					{
						if (ballY + 40 >= row[i].block[j].y &&
							ballY <= row[i].block[j].y + 40 &&
							ballX + 40 >= row[i].block[j].x &&
							ballX <= row[i].block[j].x + 80 && row[i].block[j].hit == false)
						{
							ballSpeedy *= -1;
							row[i].RGB = RGB(rand() % 255, rand() % 255, rand() % 255);
							row[i].block[j].hit = true;
							hit = true;
							countHit++;
						}
					}
				}

				if (ballY >= rect.bottom)
				{
					ballX = (player.right - player.left) / 2 + player.left;
					ballY = 700;
					ballSpeedy = 5;
					ballSpeedx = 0;
				}
			}
			if (wParam == 3)
			{
				GetLocalTime(&curTime);
				playTime++;
				wsprintf(str, TEXT("Start Time :  %d:%d:%d World Time :  %d:%d:%d, GAME TIME : %dSec"), startTime.wHour,startTime.wMinute,startTime.wSecond,curTime.wHour, curTime.wMinute, curTime.wSecond,playTime);
			}
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		case WM_PAINT:
		{
			hDC = BeginPaint(hWnd, &ps);

			mDC = CreateCompatibleDC(hDC);
			hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
			SelectObject(mDC, (HBITMAP)hBitmap);

			Rectangle(mDC, 0, 0, rect.right, rect.bottom);

			for (int i = 0;i < 3;i++)
			{
				for (int j = 0;j < 10;j++)
				{
					if (row[i].block[j].hit == true)
						hBrush = CreateSolidBrush(row[i].block[j].RGB);
					else
						hBrush = CreateSolidBrush(row[i].RGB);
					oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
					Rectangle(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 40);
					SelectObject(mDC, oldBrush);
					DeleteObject(hBrush);
				}
			}

			hBrush = CreateSolidBrush(RGB(231, 233, 182));
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
			Rectangle(mDC, player.left, player.top, player.right, player.bottom);
			SelectObject(mDC, oldBrush);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(0, 100, 300));
			oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
			Ellipse(mDC, ballX, ballY, ballX + 40, ballY + 40);
			SelectObject(mDC, oldBrush);
			DeleteObject(hBrush);

			if (isT == true)
				TextOut(mDC, rect.right / 2, rect.bottom / 2, str, lstrlen(str));
			if (isP == true)
			{
				wsprintf(str2, TEXT("hit block : %d"),countHit);
				TextOut(mDC, rect.right / 2, rect.bottom / 2 + 10, str2, lstrlen(str2));
			}
			BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

			DeleteDC(mDC);
			DeleteObject(hBitmap);

			EndPaint(hWnd, &ps);
			break;
		}

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CHAR:
			if (wParam == '+')
			{
				if (ballspeed <= 3)
					ballspeed++;
			}
			else if (wParam == '-')
			{
				if (ballspeed > 1)
					ballspeed--;
			}
			break;

		case WM_KEYDOWN:
		{
			if (wParam == 'Q')
				PostQuitMessage(0);
			else if (wParam == 'S')
			{
				KillTimer(hWnd, 2);
				SetTimer(hWnd, 2, 10, NULL);
			}
			else if (wParam == 'T')
			{
				if (isT == false)
				{
					isT = true;
				}
				else
				{
					isT = false;
				}
			}
			else if (wParam == 'P')
			{
				if (isP == false)
				{
					KillTimer(hWnd, 1);
					KillTimer(hWnd, 2);
					isP = true;
				}
				else
				{
					SetTimer(hWnd, 1, 10, NULL);
					SetTimer(hWnd, 2, 10, NULL);
					isP = false;
				}
			}
			else if (wParam == 'R')
			{
				for (int i = 0;i < 3;i++)
				{
					if (i % 2 == 0)
					{
						row[i].way = -1;
					}
					else
						row[i].way = 1;
					row[i].starty = i * 40;
					row[0].RGB = RGB(200, 100, 100);
					row[1].RGB = RGB(100, 200, 100);
					row[2].RGB = RGB(100, 100, 200);
					for (int j = 0;j < 10;j++)
					{
						row[i].block[j].y = row[i].starty;
						row[i].block[j].x = rect.left + j * 80 + 100;
						row[i].block[j].RGB = row[i].RGB;
						row[i].block[j].hit = false;
					}
				}
				ballX = rect.right / 2 - 40;
				ballY = 700;
				ballSpeedy = 5;
				ballSpeedx = 0;
				ballspeed = 1;
				playTime = 0;
				countHit = 0;
				isClick = false, isT = false, isP = false;
				x = rect.right / 2 - 100;
				player = { x,900,x + 200,950 };
				SetTimer(hWnd, 1, 10, NULL);
				SetTimer(hWnd, 3, 1000, NULL);
				GetLocalTime(&startTime);
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);
			if (mx >= player.left && mx <= player.right && my <= player.bottom && my >= player.top)
			{
				isClick = true;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (isClick == true)
			{
				int mx = LOWORD(lParam);
				player.left = mx - 100;
				player.right = mx + 100;
			}
			break;
		}
		case WM_LBUTTONUP:
			isClick = false;
			break;
		}

		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}