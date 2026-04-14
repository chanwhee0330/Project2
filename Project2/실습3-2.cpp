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
    int x, y,shape,speed,angle;
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
    static int ASIZE = 100, BSIZE = 200, CSIZE = 12, DSIZE=8,select = 0;
    static bool isRclick=false;
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
            part[i].speed = 60;
            SetTimer(hWnd, i, part[i].speed, NULL);
        }
        break;
    case WM_TIMER:
        switch (wParam) {
        case 1:
            part[1].angle++;
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        }
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        /*hBrush = CreateSolidBrush(RGB(255, 255, 255));
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        for (int i = 0; i < 4; i++)
        {
            Rectangle(hDC, part[i].x - BSIZE, part[i].y - BSIZE, part[i].x + BSIZE, part[i].y + BSIZE);
        }
        SelectObject(hDC, oldBrush);
        DeleteObject(hBrush);*/

        hBrush = CreateSolidBrush(RGB(255, 0, 0));
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        for (int i = 0; i < 4; i++)
        {
            Ellipse(hDC, part[i].x - CSIZE, part[i].y - CSIZE, part[i].x + CSIZE, part[i].y + CSIZE);
        }
        SelectObject(hDC, oldBrush);
        DeleteObject(hBrush);

        hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
        oldPen = (HPEN)SelectObject(hDC, hPen);
        oldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
        for (int i = 0; i < 4; i++)
        {
            if(part[i].shape==0)
                Ellipse(hDC, part[i].x - ASIZE, part[i].y - ASIZE, part[i].x + ASIZE, part[i].y + ASIZE);
            else if(part[i].shape==1)
                Rectangle(hDC, part[i].x - ASIZE, part[i].y - ASIZE, part[i].x + ASIZE, part[i].y + ASIZE);
            else if (part[i].shape == 2)
            {
                POINT point[6] = { {part[i].x,part[i].y-ASIZE},{part[i].x-(int)(ASIZE*sqrt(3)/2),part[i].y+ASIZE/2},{part[i].x + (int)(ASIZE * sqrt(3) / 2),part[i].y + ASIZE / 2}};
                Polygon(hDC, point, 3);
            }
        }
        SelectObject(hDC, oldBrush);
        SelectObject(hDC, oldPen);
        DeleteObject(hPen);

        hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        oldPen = (HPEN)SelectObject(hDC, hPen);
        oldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
        Rectangle(hDC, part[select].x - BSIZE, part[select].y - BSIZE, part[select].x + BSIZE, part[select].y + BSIZE);
        SelectObject(hDC, oldPen);
        SelectObject(hDC, oldBrush);
        DeleteObject(hPen);


        hBrush = CreateSolidBrush(RGB(0, 255, 0));
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        for (int i = 0; i < 4; i++)
        {
            if (part[i].shape == 0)
            {
                int x = part[i].x + cos(part[i].angle);
                int y = part[i].y + sin(part[i].angle);
                Ellipse(hDC, x-DSIZE, y - DSIZE, x + DSIZE, y + DSIZE);
            }
            else if (part[i].shape == 1)
                Rectangle(hDC, part[i].x - ASIZE, part[i].y - ASIZE, part[i].x + ASIZE, part[i].y + DSIZE);
            else if (part[i].shape == 2)
            {
                POINT point[6] = { {part[i].x,part[i].y - ASIZE},{part[i].x - (int)(ASIZE * sqrt(3) / 2),part[i].y + ASIZE / 2},{part[i].x + (int)(ASIZE * sqrt(3) / 2),part[i].y + ASIZE / 2} };
                Polygon(hDC, point, 3);
            }
        }
        SelectObject(hDC, oldBrush);
        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
        break;
    case WM_LBUTTONDOWN:
       
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
    case WM_LBUTTONUP:
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
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}