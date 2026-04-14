#include <windows.h>
#include <tchar.h>
#include <time.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"WindowClass";
LPCTSTR lpszWindowName = L"Random Text Program";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int winx = 800, winy = 600;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;

	g_hInst = hInstance;

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
		100, 100,
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static TCHAR str[10][20][20] = { 0 };
	static TCHAR temp[100];
	static COLORREF textColor[10];
	static COLORREF bgColor[10];
	static int xPos[10], yPos[10];
	static int n[10], count[10], cnt = 0;

	PAINTSTRUCT ps;
	HDC hDC;

	static bool showAll = false;

	switch (iMessage)
	{
	case WM_CREATE:
		srand((unsigned int)time(NULL));
		textColor[cnt] = RGB(rand() % 256, rand() % 256, rand() % 256);
		bgColor[cnt] = RGB(rand() % 256, rand() % 256, rand() % 256);

		xPos[cnt] = rand() % 600;
		yPos[cnt] = rand() % 400;
		n[cnt] = rand() % 10;
		count[cnt] = rand() % 5 + 5;
		for (int i = 0; i < count[cnt]; i++)
		{
			for (int j = 0; j < count[cnt]; j++)
			{
				str[cnt][i][j] = '0' + n[cnt];
			}
			str[cnt][i][count[cnt]] = '\0';
		}
		break;

	case WM_CHAR:
	{
		if (wParam == 'a')
		{
			showAll = true;
		}
		else if (wParam == VK_RETURN)
		{
			if (cnt < 9)
			{
				cnt++;
				textColor[cnt] = RGB(rand() % 256, rand() % 256, rand() % 256);
				bgColor[cnt] = RGB(rand() % 256, rand() % 256, rand() % 256);
				xPos[cnt] = rand() % 600;
				yPos[cnt] = rand() % 400;
				n[cnt] = rand() % 10;
				count[cnt] = rand() % 5 + 5;
				for (int i = 0; i < count[cnt]; i++)
				{
					for (int j = 0; j < count[cnt]; j++)
					{
						str[cnt][i][j] = '0' + (n[cnt]);
					}
					str[cnt][i][count[cnt]] = '\0';
				}
				showAll = false;
			}
		}
		else if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		else if(wParam=='r')
		{
			cnt = 0;
			textColor[cnt] = RGB(rand() % 256, rand() % 256, rand() % 256);
			bgColor[cnt] = RGB(rand() % 256, rand() % 256, rand() % 256);
			showAll = false;
			xPos[cnt] = rand() % 600;
			yPos[cnt] = rand() % 400;
			n[cnt] = rand() % 10;
			count[cnt] = rand() % 5 + 5;
			for (int i = 0; i < count[cnt]; i++)
			{
				for (int j = 0; j < count[cnt]; j++)
				{
					str[cnt][i][j] = '0' + (n[cnt]);
				}
				str[cnt][i][count[cnt]] = '\0';
			}

		}

		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;

	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);
		SetBkMode(hDC, OPAQUE);
		
		if (showAll)
		{
			for (int k = 0; k <= cnt; k++)
			{
				SetTextColor(hDC, textColor[k]);
				SetBkColor(hDC, bgColor[k]);
				for (int i = 0; i < count[k]; i++)
				{
					TextOut(hDC, xPos[k], yPos[k] + i * 20, str[k][i], lstrlen(str[k][i]));
				}
			}
		}
		else
		{
			SetTextColor(hDC, textColor[cnt]);
			SetBkColor(hDC, bgColor[cnt]);
			for (int i = 0; i < count[cnt]; i++)
			{
				TextOut(hDC, xPos[cnt], yPos[cnt] + i * 20, str[cnt][i], lstrlen(str[cnt][i]));
			}
		}
		wsprintf(temp, L"xpos : %d ypos : %d n : %d count : %d cnt : %d", xPos[cnt], yPos[cnt], n[cnt], count[cnt], cnt);
		TextOut(hDC, 300, 300, temp, lstrlen(temp));

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}