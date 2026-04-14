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
    static TCHAR str[10][31] = { 0 };
    static int row = 0, col = 0;

    static int xPos, yPos;

    PAINTSTRUCT ps;
    HDC hDC;

    switch (iMessage)
    {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 2, 15);
        ShowCaret(hWnd);
        break;

    case WM_CHAR:
    {
        if (wParam == VK_BACK)
        {
            if (col > 0)
            {
                col--;

                for (int i = row; i < 10; i++)
                {
                    int start;
                    if (i == row)
                        start = col;
                    else
                        start = 0;
                    for (int j = start; j < 30; j++)
                    {
                        if (j < 29)
                        {
                            str[i][j] = str[i][j + 1];
                        }
                        else
                        {
                            if (i < 9)
                                str[i][j] = str[i + 1][0];
                            else
                                str[i][j] = '\0';
                        }
                    }
                }
                
            }
            else if (row > 0)
            {
                row--;
                col = lstrlen(str[row]);
            }
        }
        else if (wParam == VK_RETURN)
        {
            if (row<9)
            {
                if (lstrlen(str[row]) == 0)
                {
                    for (int i = 0; i < 30; i++)
                        str[row][i] = ' ';
                }
                row++;
                col = 0;
            }
            else if (row == 9)
            {
                row = 0;
                col = 0;
            }
        }
        else if (wParam == VK_ESCAPE)
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 30; j++)
                {
                    str[i][j] = '\0';
                }
            }
            row = 0;
            col = 0;
        }
        else
        {
            if (col < 30)
            {
                str[row][col++] = (TCHAR)wParam;
            }
            else if (col == 30&&row<9)
            {
                col = 0;
                row++;
            }
            else if (row == 9 && col == 30)
            {
                col = 0;
                row = 0;
            }
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

    case WM_PAINT:
    {
        hDC = BeginPaint(hWnd, &ps);

        for (int i = 0; i < 10; i++)
        {
            TextOut(hDC, xPos, yPos + i * 20, str[i], lstrlen(str[i]));
        }
        SIZE size;
        GetTextExtentPoint32(hDC, str[row],col, &size);
        SetCaretPos(xPos + size.cx, yPos + row * 20);

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

//2-7 31일 까지 2-8,9 4월 7일까지 