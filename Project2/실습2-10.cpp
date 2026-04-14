#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <stdbool.h>

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    static HDC hDC;
    PAINTSTRUCT ps;
    static RECT rt,arect[20],brect[20];
    static HBRUSH hBrush,abrush,bbrush,cbrush;
    static int anum, bnum, cnum;
    static bool acheck=true,bcheck=true,ccheck=true;
    static POINT point[20][3];
    static bool isChange = false;
    switch (iMessage) {
    case WM_SIZE:
        isChange = true;
        break;
    case WM_PAINT:
    {
        hDC = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rt);
        Rectangle(hDC, rt.left, rt.top, rt.right/3, rt.bottom); //1번영역
        Rectangle(hDC, rt.right/3, rt.top, (rt.right/3)*2, rt.bottom); //2번영역
        Rectangle(hDC, (rt.right/3)*2, rt.top, rt.right, rt.bottom);//3번영역
        if (acheck == true||isChange==true)
        {
            if (abrush) DeleteObject(abrush);
            abrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
            anum = rand() % 20;
            for (int i = 0; i < anum; i++)
            {
                arect[i].left = rand() % (rt.right / 3 - 10);
                arect[i].top = rand() % (rt.bottom - 10);
                arect[i].right = arect[i].left + 10;
                arect[i].bottom = arect[i].top + 10;
            }
            acheck = false;
        }
        for (int i = 0; i < anum; i++)
        {
            FillRect(hDC, &arect[i], abrush);
        }
        if (bcheck == true||isChange == true)
        {
            if (bbrush) DeleteObject(bbrush);
            bbrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
            bnum = rand() % 20;
            for (int i = 0; i < bnum; i++)
            {
                brect[i].left = rand() % (rt.right / 3 - 10) + (rt.right / 3);
                brect[i].top = rand() % (rt.bottom - 10);
                brect[i].right = brect[i].left + 10;
                brect[i].bottom = brect[i].top + 10;
            }
            bcheck = false;
        }
        HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, bbrush);
        for (int i = 0; i < bnum; i++)
        {
            Ellipse(hDC, brect[i].left, brect[i].top, brect[i].right, brect[i].bottom);
        }
        SelectObject(hDC, oldBrush);

        if (ccheck == true || isChange == true)
        {
            if (cbrush) DeleteObject(cbrush);
            cbrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
            cnum = rand() % 20;
            for (int i = 0; i < cnum; i++)
            {
                int x=rand() % (rt.right / 3 - 10) + (rt.right / 3 * 2);
                int y = rand() % (rt.bottom - 10);

                point[i][0].x = x;
                point[i][0].y = y;
                point[i][1].x = x - 5;
                point[i][1].y = y + 5;
                point[i][2].x = x + 5;
                point[i][2].y = y + 5;
            }
            ccheck = false;
            isChange = false;
        }
        oldBrush = (HBRUSH)SelectObject(hDC, cbrush);
        for (int i = 0; i < cnum; i++)
        {
            Polygon(hDC, point[i], 3);
        }
        SelectObject(hDC, oldBrush);
       
        EndPaint(hWnd, &ps);
        return 0;

    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case '1':
            acheck = true;
            InvalidateRect(hWnd, NULL, TRUE);
            break; 
        case '2':
            bcheck = true;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case '3':
            ccheck = true;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RETURN:
            acheck = true;
            bcheck = true;
            ccheck = true;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;
    }
    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

//2-7 31일 까지 2-8,9 4월 7일까지 