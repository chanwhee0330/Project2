#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"WindowClass";
LPCTSTR lpszWindowName = L"XOR Drag Line";

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;
    HBRUSH hBrush, oldBrush;
    static COLORREF RGB = RGB(rand() % 255, rand() % 255, rand() % 255);
    static int mx, my,shape=3,speed=10;
    RECT rect;
    static bool isH=false, isV = false, isS = false;
    static bool turn = false;
    static int move=1;
    static int dx = 1, dy = -1;
    GetClientRect(hWnd, &rect);
    switch (iMsg)
    {
    case WM_CREATE:
       
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        if (shape == 1)
        {
            hBrush = CreateSolidBrush(RGB);
            oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

            Rectangle(hDC, mx - 40, my - 40, mx + 40, my + 40);
            SelectObject(hDC, oldBrush);
            DeleteObject(hBrush);
        }
        else if (shape == 2)
        {
            hBrush = CreateSolidBrush(RGB);
            oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

            Ellipse(hDC, mx - 40, my - 40, mx + 40, my + 40);
            SelectObject(hDC, oldBrush);
            DeleteObject(hBrush);
        }
        else if (shape == 3)
        {
            hBrush = CreateSolidBrush(RGB);
            oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
            POINT point[6] = { {mx, my - 40}, {mx - (int)(40 * sqrt(3) / 2), my + 20},{mx + (int)(40 * sqrt(3) / 2), my + 20} };
            Polygon(hDC, point, 3);
            SelectObject(hDC, oldBrush);
            DeleteObject(hBrush);
        }

        EndPaint(hWnd, &ps);
        break;
    case WM_TIMER:
        if (isH == true)
        {
            if (turn == false)
            {
                mx = mx + speed;
                if (mx >= rect.right - 40)
                {
                    mx = rect.right - 40;
                    my += 40;
                    turn = true;
                    
                    if (my >= rect.bottom - 40)
                    {
                        mx = 40;
                        my = 40;
                        turn = false;
                    }
                }
            }
            else
            {
                mx = mx - speed;
                if (mx <= 40)
                {
                    mx = 40;
                    my += 40;
                    turn = false;

                    if (my >= rect.bottom - 40)
                    {
                        mx = rect.right - 40;
                        my = 40;
                        turn = true;
                    }
                }
            }
           
        }
        else if (isV == true)
        {
            if (turn == false)
            {
                my = my + speed;
                if (my >=rect.bottom-40)
                {
                    mx += 40;
                    my = rect.bottom-40;
                    turn = true;

                    if (mx >= rect.right - 40)
                    {
                        mx = 40;
                        my = 40;
                        turn = false;
                    }
                }
            }
            else
            {
                my = my - speed;
                if (my <= 40)
                {
                    mx += 40;
                    my = 40;
                    turn = false;

                    if (my >= rect.right - 40)
                    {
                        mx = 40;
                        my = 40;
                        turn = true;
                    }
                }
            }
        }
        else if (isS == true)
        {
            mx += dx*speed;
            my += dy*speed;

            // 오른쪽 벽
            if (mx >= rect.right - 40)
            {
                mx = rect.right - 40;
                dx = -dx;
            }

            // 왼쪽 벽
            if (mx <= 40)
            {
                mx = 40;
                dx = -dx;
            }

            // 아래 벽
            if (my >= rect.bottom - 40)
            {
                my = rect.bottom - 40;
                dy = -dy;
            }

            // 위 벽
            if (my <= 40)
            {
                my = 40;
                dy = -dy;
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_LBUTTONDOWN:
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        RGB = RGB(rand() % 255, rand() % 255, rand() % 255);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_LBUTTONUP:
        break;
    case WM_MOUSEMOVE:
       
        break;
    case WM_CHAR:
        if (wParam == '+')
        {
            speed=speed+10;
        }
        else if (wParam == '-')
        {
            speed=speed-10;
        }
        break;
    case WM_KEYDOWN:
        if (wParam == 'R')
        {
            shape = 1;
        }
        else if (wParam == 'E')
        {
            shape = 2;
        }
        else if (wParam == 'T')
        {
            shape=3;
        }
        else if (wParam == 'P')
        {
            speed = 0;
        }
        else if (wParam == 'Q')
        {
            PostQuitMessage(0);
        }
        else if (wParam == 'H')
        {
            isH = true;
            isV = false;
            isS = false;
            speed = 10;
            SetTimer(hWnd, 1, 20, NULL);
        }
        else if (wParam == 'V')
        {
            isV = true;
            isH = false;
            isS = false;
            speed = 10;
            SetTimer(hWnd, 1, 20, NULL);
        }
        else if (wParam == 'S')
        {
            isV = false;
            isH = false;
            isS = true;
            move = 1;
            speed = 10;
            SetTimer(hWnd, 1, 20, NULL);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}