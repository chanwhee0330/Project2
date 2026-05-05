#include <windows.h> 
#include <tchar.h> 
#include <time.h> 
#include <stdbool.h> 
#include "resource3.h" 

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"WindowClass";
LPCTSTR lpszWindowName = L"Random Text Program";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int winx = 817, winy = 860;

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
	int x, y, state, set;
	bool isClick,hint;
	COLORREF RGB;
};

struct Row {
	struct Block block[10];
	COLORREF RGB;
};

struct Row row[10];

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hDC, mDC;
	HBITMAP hBitmap;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	RECT rect;
	TCHAR text[50];
	static int startX, startY, endX, endY;
	static bool isDrag = false, hasMove = false, isItem, isBomb, isScore;
	static int bombCount = 0, itemCount = 0, lefttop = 0, righttop, leftbottom, rightbottom;
	static int hintTimer = 0, set, set1, set2, set3, set4, set5;
	GetClientRect(hWnd, &rect);

	switch (iMessage)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_MENU_GAMESTART:
			bombCount = 0, itemCount = 0, lefttop = 0, righttop = 0, leftbottom = 0, rightbottom = 0, set = 0;
			isItem = false, isBomb = false, set1 = 0, set2 = 0, set3 = 0, set4 = 0, set5 = 0;
			for (int i = 0;i < 10;i++)
			{
				for (int j = 0;j < 10;j++)
				{
					row[i].block[j].x = j * 80;
					row[i].block[j].y = i * 80;
					row[i].block[j].state = 0; //0 아무것도 아님, 1 폭탄 2 아이템 
					row[i].block[j].isClick = false;
					row[i].block[j].hint = false;
				}
			}

			while (bombCount < 20)
			{
				int j = rand() % 10;
				int k = rand() % 10;

				if (row[j].block[k].state == 0)
				{
					row[j].block[k].state = 1;
					bombCount++;
				}
			}

			while (itemCount < 10)
			{
				int j = rand() % 10;
				int k = rand() % 10;

				if (row[j].block[k].state == 0)
				{
					row[j].block[k].state = 2;
					itemCount++;
				}
			}
			while (lefttop < 5)
			{
				int j = rand() % 10;
				int k = rand() % 10;

				if (row[j].block[k].state == 0)
				{
					row[j].block[k].state = 3;
					row[j].block[k].set = lefttop;
					lefttop++;
				}
			}
			while (righttop < 5)
			{
				int j = rand() % 10;
				int k = rand() % 10;

				if (row[j].block[k].state == 0)
				{
					row[j].block[k].state = 4;
					row[j].block[k].set = righttop;
					righttop++;
				}
			}
			while (leftbottom < 5)
			{
				int j = rand() % 10;
				int k = rand() % 10;

				if (row[j].block[k].state == 0)
				{
					row[j].block[k].state = 5;
					row[j].block[k].set = leftbottom;
					leftbottom++;
				}
			}
			while (rightbottom < 5)
			{
				int j = rand() % 10;
				int k = rand() % 10;

				if (row[j].block[k].state == 0)
				{
					row[j].block[k].state = 6;
					row[j].block[k].set = rightbottom;
					rightbottom++;
				}
			}
			break;
		case ID_MENU_GAMEEND:
		{
			int answer = MessageBox(
				hWnd,
				L"프로그램을 종료하시겠습니까?",
				L"끝내기 선택",
				MB_YESNO
			);

			if (answer == IDYES)
				PostQuitMessage(0);
			break;
		}
		case ID_MENU_HINT:
		{
			hintTimer = 0;
			for (int i = 0;i < 10;i++)
			{
				for (int j = 0;j < 10;j++)
				{
					row[i].block[j].hint = true;
				}
			}
			KillTimer(hWnd, 1);
			SetTimer(hWnd, 1, 1000, NULL);
			break;
		}
		case ID_MENU_SCORE:
		{
			if (isScore)
				isScore = false;
			else
				isScore = true;
			break;
		}
		}
		break;
	case WM_TIMER:
	{
		if (wParam == 1)
		{
			hintTimer++;
			if (hintTimer >= 2)
			{
				for (int i = 0;i < 10;i++)
				{
					for (int j = 0;j < 10;j++)
					{
						row[i].block[j].hint = false;
					}
				}
				KillTimer(hWnd, 1);
				hintTimer = 0;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	case WM_CREATE:
		bombCount = 0, itemCount = 0, lefttop = 0, righttop = 0, leftbottom = 0, rightbottom = 0, set = 0;
		isItem = false, isBomb = false, set1 = 0, set2 = 0, set3 = 0, set4 = 0, set5 = 0;
		for (int i = 0;i < 10;i++)
		{
			for (int j = 0;j < 10;j++)
			{
				row[i].block[j].x = j * 80;
				row[i].block[j].y = i * 80;
				row[i].block[j].state = 0; //0 아무것도 아님, 1 폭탄 2 아이템 
				row[i].block[j].isClick = false;
				row[i].block[j].hint = false;
			}
		}

		while (bombCount < 20)
		{
			int j = rand() % 10;
			int k = rand() % 10;

			if (row[j].block[k].state == 0)
			{
				row[j].block[k].state = 1;
				bombCount++;
			}
		}

		while (itemCount < 10)
		{
			int j = rand() % 10;
			int k = rand() % 10;

			if (row[j].block[k].state == 0)
			{
				row[j].block[k].state = 2;
				itemCount++;
			}
		}
		while (lefttop < 5)
		{
			int j = rand() % 10;
			int k = rand() % 10;

			if (row[j].block[k].state == 0)
			{
				row[j].block[k].state = 3;
				row[j].block[k].set = lefttop;
				lefttop++;
			}
		}
		while (righttop < 5)
		{
			int j = rand() % 10;
			int k = rand() % 10;

			if (row[j].block[k].state == 0)
			{
				row[j].block[k].state = 4;
				row[j].block[k].set = righttop;
				righttop++;
			}
		}
		while (leftbottom < 5)
		{
			int j = rand() % 10;
			int k = rand() % 10;

			if (row[j].block[k].state == 0)
			{
				row[j].block[k].state = 5;
				row[j].block[k].set = leftbottom;
				leftbottom++;
			}
		}
		while (rightbottom < 5)
		{
			int j = rand() % 10;
			int k = rand() % 10;

			if (row[j].block[k].state == 0)
			{
				row[j].block[k].state = 6;
				row[j].block[k].set = rightbottom;
				rightbottom++;
			}
		}

		break;
	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);

		mDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		SelectObject(mDC, (HBITMAP)hBitmap);

		Rectangle(mDC, 0, 0, rect.right, rect.bottom);

		hPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 20));
		oldPen = (HPEN)SelectObject(mDC, hPen);
		for (int i = 0;i < 10;i++)
		{
			for (int j = 0;j < 10;j++)
			{
				if (row[i].block[j].isClick == false && row[i].block[j].hint == false)
				{
					hBrush = CreateSolidBrush(RGB(0, 0, 0));
					oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
					Rectangle(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80);
					SelectObject(mDC, oldBrush);
					DeleteObject(hBrush);
				}
				else if (row[i].block[j].isClick == true || row[i].block[j].hint == true)
				{
					hBrush = CreateSolidBrush(RGB(255, 255, 255));
					oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
					Rectangle(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80);
					if (row[i].block[j].state == 0)
					{
					}
					else if (row[i].block[j].state == 1)
					{
						SetTextColor(mDC, RGB(255, 0, 0));
						TextOut(mDC, row[i].block[j].x + 30, row[i].block[j].y + 25, L"B", 1);
					}
					else if (row[i].block[j].state == 2)
					{
						HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0)); // 노란색
						HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, yellowBrush);
						POINT point[4] = { {row[i].block[j].x + 40,row[i].block[j].y},{row[i].block[j].x + 80,row[i].block[j].y + 40},{row[i].block[j].x + 40,row[i].block[j].y + 80},{row[i].block[j].x,row[i].block[j].y + 40} };
						Polygon(mDC, point, 4);
						SelectObject(mDC, oldBrush2);
						DeleteObject(yellowBrush);
					}
					else if (row[i].block[j].state == 3)
					{
						if (row[i].block[j].set == 0)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(0, 120, 255));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y, row[i].block[j].x, row[i].block[j].y + 40);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 1)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(0, 200, 100));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y, row[i].block[j].x, row[i].block[j].y + 40);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 2)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(100, 0, 255));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y, row[i].block[j].x, row[i].block[j].y + 40);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 3)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 140, 0));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y, row[i].block[j].x, row[i].block[j].y + 40);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 4)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 215, 0));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y, row[i].block[j].x, row[i].block[j].y + 40);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
					}
					else if (row[i].block[j].state == 4)
					{
						if (row[i].block[j].set == 0)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(0, 120, 255));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y + 40, row[i].block[j].x + 40, row[i].block[j].y);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 1)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(0, 200, 100));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y + 40, row[i].block[j].x + 40, row[i].block[j].y);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 2)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(100, 0, 255));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y + 40, row[i].block[j].x + 40, row[i].block[j].y);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 3)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 140, 0));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y + 40, row[i].block[j].x + 40, row[i].block[j].y);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 4)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 215, 0));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y + 40, row[i].block[j].x + 40, row[i].block[j].y);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
					}
					else if (row[i].block[j].state == 5)
					{
						if (row[i].block[j].set == 0)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(0, 120, 255));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x, row[i].block[j].y + 40, row[i].block[j].x + 40, row[i].block[j].y + 80);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 1)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(0, 200, 100));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x, row[i].block[j].y + 40, row[i].block[j].x + 40, row[i].block[j].y + 80);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 2)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(100, 0, 255));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x, row[i].block[j].y + 40, row[i].block[j].x + 40, row[i].block[j].y + 80);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 3)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 140, 0));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x, row[i].block[j].y + 40, row[i].block[j].x + 40, row[i].block[j].y + 80);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 4)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 215, 0));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x, row[i].block[j].y + 40, row[i].block[j].x + 40, row[i].block[j].y + 80);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}

					}
					else if (row[i].block[j].state == 6)
					{
						if (row[i].block[j].set == 0)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(0, 120, 255));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y + 80, row[i].block[j].x + 80, row[i].block[j].y + 40);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 1)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(0, 200, 100));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y + 80, row[i].block[j].x + 80, row[i].block[j].y + 40);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 2)
						{

							HBRUSH hBrush2 = CreateSolidBrush(RGB(100, 0, 255));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y + 80, row[i].block[j].x + 80, row[i].block[j].y + 40);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 3)
						{
							HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 140, 0));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y + 80, row[i].block[j].x + 80, row[i].block[j].y + 40);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
						else if (row[i].block[j].set == 4)
						{
							HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 215, 0));
							HBRUSH oldBrush2 = (HBRUSH)SelectObject(mDC, hBrush2);
							Pie(mDC, row[i].block[j].x, row[i].block[j].y, row[i].block[j].x + 80, row[i].block[j].y + 80, row[i].block[j].x + 40, row[i].block[j].y + 80, row[i].block[j].x + 80, row[i].block[j].y + 40);
							SelectObject(mDC, oldBrush2);
							DeleteObject(hBrush2);
						}
					}
					if (row[i].block[j].isClick == true && row[i].block[j].state >= 3)
					{
						if (row[i].block[j].set == 0) set1++;
						else if (row[i].block[j].set == 1) set2++;
						else if (row[i].block[j].set == 2) set3++;
						else if (row[i].block[j].set == 3) set4++;
						else if (row[i].block[j].set == 4) set5++;
					}
					SelectObject(mDC, oldBrush);
					DeleteObject(hBrush);
				}
			}
			if (isBomb == true)
			{
				DrawText(mDC, L"GAME OVER!", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}
		SelectObject(mDC, oldPen);
		DeleteObject(hPen);

		if (isDrag)
		{
			HPEN pen = CreatePen(PS_DOT, 1, RGB(0, 50, 0)); // 점선
			HPEN oldPen = (HPEN)SelectObject(mDC, pen);

			HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, GetStockObject(NULL_BRUSH)); // 내부 비우기

			Rectangle(mDC, startX, startY, endX, endY);

			SelectObject(mDC, oldBrush);
			SelectObject(mDC, oldPen);

			DeleteObject(pen);
		}

		if (isScore == true)
		{
			set1 = set2 = set3 = set4 = set5 = 0;
			set = 0;

			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (row[i].block[j].isClick && row[i].block[j].state >= 3)
					{
						if (row[i].block[j].set == 0) set1++;
						else if (row[i].block[j].set == 1) set2++;
						else if (row[i].block[j].set == 2) set3++;
						else if (row[i].block[j].set == 3) set4++;
						else if (row[i].block[j].set == 4) set5++;
					}
				}
			}

			if (set1 == 4) set++;
			if (set2 == 4) set++;
			if (set3 == 4) set++;
			if (set4 == 4) set++;
			if (set5 == 4) set++;

			SetBkMode(mDC, OPAQUE);
			SetBkColor(mDC, RGB(255, 255, 255));


			wsprintf(text, L"Clear Set : %d", set);
			TextOut(mDC, 10, 10, text, lstrlen(text));
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
	case WM_KEYDOWN:
	{
		if (wParam == 'Q')
			PostQuitMessage(0);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		isDrag = true;
		hasMove = false;
		startX = LOWORD(lParam);
		startY = HIWORD(lParam);
		endX = startX;
		endY = startY;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (isDrag == true)
		{
			int x = endX = LOWORD(lParam);
			int y = endY = HIWORD(lParam);
			if (x != startX || y != startY)
				hasMove = true;

			endX = x;
			endY = y;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	}
	case WM_LBUTTONUP:
		isDrag = false;
		if (!hasMove)
		{
			int j = startX / 80;
			int i = startY / 80;

			if (i >= 0 && i < 10 && j >= 0 && j < 10)
			{
				row[i].block[j].isClick = true;
				if (row[i].block[j].state == 2)
					isItem = true;
				else if (row[i].block[j].state == 1)
					isBomb = true;
			}
		}
		else
		{
			int sx = startX / 80;
			int sy = startY / 80;
			int ex = endX / 80;
			int ey = endY / 80;

			if (sx > ex) { int t = sx; sx = ex; ex = t; }
			if (sy > ey) { int t = sy; sy = ey; ey = t; }

			for (int i = sy; i <= ey; i++)
			{
				for (int j = sx; j <= ex; j++)
				{
					if (i >= 0 && i < 10 && j >= 0 && j < 10)
					{
						row[i].block[j].isClick = true;
						if (row[i].block[j].state == 2)
							isItem = true;
						else if (row[i].block[j].state == 1)
							isBomb = true;
					}
				}
			}
		}

		if (isItem == true)
		{
			for (int i = 0;i < 10;i++)
			{
				for (int j = 0;j < 10;j++)
				{
					if (row[i].block[j].state >= 3 && row[i].block[j].isClick == true)
					{
						for (int k = 0;k < 10;k++)
						{
							for (int l = 0;l < 10;l++)
							{
								if (row[k].block[l].set == row[i].block[j].set)
									row[k].block[l].isClick = true;
							}
						}
					}
				}
			}
			isItem = false;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}