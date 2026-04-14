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
    static TCHAR str[5][21] = { 0 }; 
    static int row = 0, col = 0;

    static int xPos, yPos;
    static COLORREF textColor;

    PAINTSTRUCT ps;
    HDC hDC;

    switch (iMessage)
    {
    case WM_CREATE:
        srand((unsigned int)time(NULL));
        textColor = RGB(rand() % 256, rand() % 256, rand() % 256);

        xPos = rand() % 600;
        yPos = rand() % 500;

        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        break;

    case WM_CHAR:
    {
        if (wParam == VK_BACK)
        {
            if (col > 0)
            {
                str[row][--col] = '\0';
            }
            else if (row > 0)
            {
                row--;
                col = lstrlen(str[row]);
            }
        }
        else if (wParam == VK_RETURN)
        {
            yPos += 20;
        }
        else if (wParam==VK_ESCAPE)
        {
            PostQuitMessage(0);
        }
        else
        {
            if (col < 20)
            {
                str[row][col++] = (TCHAR)wParam;
                str[row][col] = '\0';
            }
            else
            {
                if (row < 4)
                {
                    row++;
                    col = 0;
                    str[row][col++] = (TCHAR)wParam;
                    str[row][col] = '\0';
                }
            }
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

    case WM_PAINT:
    {
        hDC = BeginPaint(hWnd, &ps);

        SetTextColor(hDC, textColor);

        for (int i = 0; i < 5; i++)
        {
            TextOut(hDC, xPos, yPos + i * 20, str[i], lstrlen(str[i]));
        }

        SIZE size;
        GetTextExtentPoint32(hDC, str[row], lstrlen(str[row]), &size);
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