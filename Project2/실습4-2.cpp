#include <windows.h> 
#include <tchar.h> 
#include <time.h> 
#include <stdbool.h> 
#include "resource.h" 

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"WindowClass";
LPCTSTR lpszWindowName = L"Random Text Program";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int winx = 1500, winy = 1100;

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
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU2);
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

struct Car {
    int left, top, right, bottom, speed;
    COLORREF RGB;
};

RECT rect, center;

bool IsInCenter(struct Car c)
{
    return !(c.right < center.left ||
        c.left > center.right ||
        c.bottom < center.top ||
        c.top > center.bottom);
}

struct Car car[8];

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hDC, mDC;
    HBITMAP hBitmap;
    PAINTSTRUCT ps;
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;

    static int distance = 100;
    static bool V_Red = false, V_Green = true, H_Red = true, H_Green = false;
    static bool V_Yellow = false, H_Yellow = false, signal = false;

    static int width = 150;
    static int Vtimer = 0, Htimer = 0, speedControl = 0;

    static int pX, pY, pSpeed;
    static int p1x, p2x, p3x, p4x, p1y, p2y, p3y, p4y;

    static bool pright = false, pleft = false,pdown=false,pup=false;
    static bool pcross1 = false, pcross2 = false, pcross3 = false, pcross4 = false;
    static bool personHorizontal = false;
    static bool personVertical = false,ismoving=false;

    GetClientRect(hWnd, &rect);

    switch (iMessage)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_MENU_H:
            V_Red = true;
            H_Red = false;
            break;

        case ID_MENU_H40002:
            V_Red = false;
            H_Red = true;
            break;

        case ID_MENU_AUTO:
            if (signal == false)
            {
                signal = true;
                KillTimer(hWnd, 2);
                SetTimer(hWnd, 2, 1000, NULL);
            }
            else
            {
                signal = false;
                KillTimer(hWnd, 2);
            }
            break;

        case ID_MENU_STOP:
            signal = false;
            KillTimer(hWnd, 2);
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
        GetClientRect(hWnd, &rect);

        center = {
            rect.right / 2 - 150,
            rect.bottom / 2 - 150,
            rect.right / 2 + 150,
            rect.bottom / 2 + 150
        };

        car[0] = { 0, rect.bottom / 2 + 125, width, rect.bottom / 2 + 25, 4 };
        car[0].RGB = RGB(50, 0, 0);

        car[1] = { 170, rect.bottom / 2 + 125, 170 + width, rect.bottom / 2 + 25, 7 };
        car[1].RGB = RGB(0, 50, 0);

        car[2] = { rect.right - width, rect.bottom / 2 - 125, rect.right, rect.bottom / 2 - 25, 5 };
        car[2].RGB = RGB(0, 0, 50);

        car[3] = { rect.right - width - 170, rect.bottom / 2 - 125, rect.right - 170, rect.bottom / 2 - 25, 8 };
        car[3].RGB = RGB(50, 0, 50);

        car[4] = { rect.right / 2 - 125, 0, rect.right / 2 - 25, width, 8 };
        car[4].RGB = RGB(50, 100, 50);

        car[5] = { rect.right / 2 - 125, -170, rect.right / 2 - 25, -170 + width, 5 };
        car[5].RGB = RGB(0, 150, 150);

        car[6] = { rect.right / 2 + 25, rect.bottom - width, rect.right / 2 + 125, rect.bottom, 6 };
        car[6].RGB = RGB(100, 100, 0);

        car[7] = { rect.right / 2 + 25, rect.bottom + 170 - width, rect.right / 2 + 125, rect.bottom + 170, 5 };
        car[7].RGB = RGB(150, 150, 0);

        p1x = rect.right / 2 - 180;
        p1y = rect.bottom / 2 - 180;

        p2x = rect.right / 2 + 150;
        p2y = rect.bottom / 2 - 180;

        p3x = rect.right / 2 + 150;
        p3y = rect.bottom / 2 + 150;

        p4x = rect.right / 2 - 180;
        p4y = rect.bottom / 2 + 150;

        pX = p1x;
        pY = p1y;
        pSpeed = 3;

        SetTimer(hWnd, 1, 10, NULL);
        break;
    case WM_TIMER:
    {
        if (wParam == 1)
        {
            bool horizontalInCenter = false; // → ←
            bool verticalInCenter = false;   // ↑ ↓

            for (int i = 0; i < 8; i++)
            {
                if (IsInCenter(car[i]))
                {
                    if (i < 4) horizontalInCenter = true;
                    else verticalInCenter = true;
                }
            }

            for (int i = 0; i < 2; i++)
            {
                bool stop = false;

                for (int j = 0; j < 2; j++)
                {
                    if (i == j) continue;

                    int dist = car[j].left - car[i].right;
                    if (dist < 0) dist += rect.right;

                    if (dist < 30)
                    {
                        stop = true;
                        break;
                    }
                }

                bool beforeCenter = (car[i].right < center.left);
                bool enteringNow = (car[i].right + car[i].speed >= center.left);

                if (V_Red == true &&
                    car[i].right >= center.left - 100 &&
                    car[i].right <= center.left + 10)
                {
                    // 빨간불 정지
                }
                else if ((verticalInCenter && beforeCenter && enteringNow) || (personVertical && beforeCenter && enteringNow))
                {
                    // 대기
                }
                else if (!stop)
                {
                    car[i].left += car[i].speed;
                    car[i].right += car[i].speed;
                }

                if (car[i].left >= rect.right)
                {
                    car[i].left -= rect.right;
                    car[i].right -= rect.right;
                }
            }

            for (int i = 2; i < 4; i++)
            {
                bool stop = false;

                for (int j = 2; j < 4; j++)
                {
                    if (i == j) continue;

                    int dist = car[i].left - car[j].right;
                    if (dist < 0) dist += rect.right;

                    if (dist < 30)
                    {
                        stop = true;
                        break;
                    }
                }

                bool beforeCenter = (car[i].left > center.right);
                bool enteringNow = (car[i].left - car[i].speed <= center.right);

                if (V_Red == true &&
                    car[i].left <= center.right + 100 &&
                    car[i].left >= center.right - 10)
                {
                }
                else if ((verticalInCenter && beforeCenter && enteringNow)||(personVertical&&beforeCenter&&enteringNow))
                {
                }
                else if (!stop)
                {
                    car[i].left -= car[i].speed;
                    car[i].right -= car[i].speed;
                }

                if (car[i].right <= rect.left)
                {
                    car[i].left += rect.right;
                    car[i].right += rect.right;
                }
            }

            for (int i = 4; i < 6; i++)
            {
                bool stop = false;

                for (int j = 4; j < 6; j++)
                {
                    if (i == j) continue;

                    int dist = car[j].top - car[i].bottom;
                    if (dist < 0) dist += rect.bottom;

                    if (dist < 30)
                    {
                        stop = true;
                        break;
                    }
                }

                bool beforeCenter = (car[i].bottom < center.top);
                bool enteringNow = (car[i].bottom + car[i].speed >= center.top);

                if (H_Red == true &&
                    car[i].bottom >= center.top - 100 &&
                    car[i].bottom <= center.top + 10)
                {
                }
                else if ((horizontalInCenter && beforeCenter && enteringNow) ||
                    (personHorizontal && beforeCenter && enteringNow))
                {
                }
                else if (!stop)
                {
                    car[i].top += car[i].speed;
                    car[i].bottom += car[i].speed;
                }

                if (car[i].top >= rect.bottom)
                {
                    car[i].top -= rect.bottom;
                    car[i].bottom -= rect.bottom;
                }
            }

            for (int i = 6; i < 8; i++)
            {
                bool stop = false;

                for (int j = 6; j < 8; j++)
                {
                    if (i == j) continue;

                    int dist = car[i].top - car[j].bottom;
                    if (dist < 0) dist += rect.bottom;

                    if (dist < 30)
                    {
                        stop = true;
                        break;
                    }
                }

                bool beforeCenter = (car[i].top > center.bottom);
                bool enteringNow = (car[i].top - car[i].speed <= center.bottom);

                if (H_Red == true &&
                    car[i].top <= center.bottom + 100 &&
                    car[i].top >= center.bottom - 10)
                {
                }
                else if ((horizontalInCenter && beforeCenter && enteringNow) ||
                    (personHorizontal && beforeCenter && enteringNow))
                {
                }
                else if (!stop)
                {
                    car[i].top -= car[i].speed;
                    car[i].bottom -= car[i].speed;
                }

                if (car[i].bottom <= 0)
                {
                    car[i].top += rect.bottom;
                    car[i].bottom += rect.bottom;
                }
            }

           // 사람이 움직여야 되는데 1,2,3,4로 나누어서 움직이고 1->2로 2->3으로 3->4로 4->1로만 움직이게 하면 좀 쉽게 할거 같은데.
           
            if (ismoving == false)
            {
                if (pX == p1x && pY == p1y && H_Red == true && V_Red == false)
                {
                    pright = true;
                }
                else if (pX == p2x && pY == p2y && V_Red == true && H_Red == false)
                {
                    pdown = true;
                }
                else if (pX == p3x && pY == p3y && H_Red == true && V_Red == false)
                {
                    pleft = true;
                }
                else if (pX == p4x && pY == p4y && V_Red == true && H_Red == false)
                {
                    pup = true;
                }
                else if (pX == p1x && pY == p1y && H_Red == true && V_Red == true)
                {
                    pcross1 = true;
                }
                else  if (pX == p2x && pY == p2y && H_Red == true && V_Red == true)
                {
                    pcross2 = true;
                }
                else  if (pX == p3x && pY == p3y && H_Red == true && V_Red == true)
                {
                    pcross3 = true;
                }
                else  if (pX == p4x && pY == p4y && H_Red == true && V_Red == true)
                {
                    pcross4 = true;
                }
            }
           
            if (pright == true)
            {
                personHorizontal = true;
                pX = pX + pSpeed;
                if (pX >= p2x)
                {
                    pX = p2x;
                    pright = false;
                    personHorizontal = false;
                    ismoving = false;
                }
            }
            if (pleft == true)
            {
                personHorizontal = true;
                pX = pX - pSpeed;
                if (pX <= p4x)
                {
                    pX = p4x;
                    pleft = false;
                    personHorizontal = false;
                    ismoving = false;
                }
            }
            if (pdown == true)
            {
                personVertical = true;
                pY = pY + pSpeed;
                if (pY >= p3y)
                {
                    pY = p3y;
                    pdown = false;
                    personVertical = false;
                    ismoving = false;
                }
            }
            if (pup == true)
            {
                personVertical = true;
                pY = pY - pSpeed;
                if (pY <= p1y)
                {
                    pY = p1y;
                    pup = false;
                    personVertical = false;
                    ismoving = false;
                }
            }
            if (pcross1 == true)
            {
                personHorizontal = true;
                personVertical = true;
                pX += pSpeed;
                pY += pSpeed;
                if (pX >= p3x && pY >= p3y)
                {
                    pX = p3x;
                    pY = p3y;
                    pcross1 = false;
                    personHorizontal = false;
                    personVertical = false;
                    ismoving = false;
                }
            }
            if (pcross2 == true)
            {
                personHorizontal = true;
                personVertical = true;
                pX -= pSpeed;
                pY += pSpeed;
                if (pX <= p4x && pY >= p4y)
                {
                    pX = p4x;
                    pY = p4y;
                    pcross2 = false;
                    personHorizontal = false;
                    personVertical = false;
                    ismoving = false;
                }
            }
            if (pcross3 == true)
            {
                personHorizontal = true;
                personVertical = true;
                pX -= pSpeed;
                pY -= pSpeed;
                if (pX <= p1x && pY <= p1y)
                {
                    pX = p1x;
                    pY = p1y;
                    pcross3 = false;
                    personHorizontal = false;
                    personVertical = false;
                    ismoving = false;
                }
            }
            if (pcross4 == true)
            {
                personHorizontal = true;
                personVertical = true;
                pX += pSpeed;
                pY -= pSpeed;
                if (pX >= p2x && pY <= p2y)
                {
                    pX = p2x;
                    pY = p2y;
                    pcross4 = false;
                    personHorizontal = false;
                    personVertical = false;
                    ismoving = false;
                }
            }
        }

        if (wParam == 2)
        {
            Vtimer++;

            if (V_Red == true && Vtimer == 6)
            {
                V_Red = false;
                V_Yellow = false;
                V_Green = true;
                Vtimer = 0;
            }
            else if (V_Green == true && Vtimer == 5)
            {
                V_Red = false;
                V_Yellow = true;
                V_Green = false;
                Vtimer = 0;
            }
            else if (V_Yellow == true && Vtimer == 1)
            {
                V_Red = true;
                V_Yellow = false;
                V_Green = false;
                Vtimer = 0;
            }

            Htimer++;

            if (H_Red == true && Htimer == 6)
            {
                H_Red = false;
                H_Yellow = false;
                H_Green = true;
                Htimer = 0;
            }
            else if (H_Green == true && Htimer == 5)
            {
                H_Red = false;
                H_Yellow = true;
                H_Green = false;
                Htimer = 0;
            }
            else if (H_Yellow == true && Htimer == 1)
            {
                H_Red = true;
                H_Yellow = false;
                H_Green = false;
                Htimer = 0;
            }
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

            SelectObject(mDC, GetStockObject(NULL_PEN));

            // 차도
            hBrush = CreateSolidBrush(RGB(50, 50, 50));
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

            Rectangle(mDC,
                rect.right / 2 - 150, 0,
                rect.right / 2 + 150, rect.bottom);

            Rectangle(mDC,
                0, rect.bottom / 2 - 150,
                rect.right, rect.bottom / 2 + 150);

            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);

            // 중앙선
            hPen = CreatePen(PS_DASH, 1, RGB(255, 204, 0));
            oldPen = (HPEN)SelectObject(mDC, hPen);

            MoveToEx(mDC, rect.right / 2, 0, NULL);
            LineTo(mDC, rect.right / 2, rect.bottom);

            MoveToEx(mDC, 0, rect.bottom / 2, NULL);
            LineTo(mDC, rect.right, rect.bottom / 2);

            SelectObject(mDC, oldPen);
            DeleteObject(hPen);

            // 가운데 네모
            hPen = CreatePen(PS_SOLID, 1, RGB(255, 204, 0));
            oldPen = (HPEN)SelectObject(mDC, hPen);
            oldBrush = (HBRUSH)SelectObject(mDC, GetStockObject(NULL_BRUSH));

            Rectangle(mDC, center.left, center.top, center.right, center.bottom);

            SelectObject(mDC, oldBrush);
            SelectObject(mDC, oldPen);
            DeleteObject(hPen);

            // 자동차 (오른쪽)
            for (int i = 0; i < 2; i++)
            {
                hBrush = CreateSolidBrush(car[i].RGB);
                oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

                int left = car[i].left;
                int right = car[i].right;

                int drawL = max(0, left);
                int drawR = min(rect.right, right);

                if (drawL < drawR)
                    Rectangle(mDC, drawL, car[i].top, drawR, car[i].bottom);

                if (right > rect.right)
                {
                    int overflow = right - rect.right;
                    Rectangle(mDC, 0, car[i].top, overflow, car[i].bottom);
                }

                SelectObject(mDC, oldBrush);
                DeleteObject(hBrush);
            }

            // 자동차 (왼쪽)
            for (int i = 2; i < 4; i++)
            {
                hBrush = CreateSolidBrush(car[i].RGB);
                oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

                int left = car[i].left;
                int right = car[i].right;

                int drawL = max(0, left);
                int drawR = min(rect.right, right);

                if (drawL < drawR)
                    Rectangle(mDC, drawL, car[i].top, drawR, car[i].bottom);

                if (left < 0)
                {
                    int overflow = -left;
                    Rectangle(mDC,
                        rect.right - overflow,
                        car[i].top,
                        rect.right,
                        car[i].bottom);
                }

                SelectObject(mDC, oldBrush);
                DeleteObject(hBrush);
            }

            // 자동차 (아래)
            for (int i = 4; i < 6; i++)
            {
                hBrush = CreateSolidBrush(car[i].RGB);
                oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

                int top = car[i].top;
                int bottom = car[i].bottom;

                int drawT = max(0, top);
                int drawB = min(rect.bottom, bottom);

                if (drawT < drawB)
                    Rectangle(mDC, car[i].left, drawT, car[i].right, drawB);

                if (bottom > rect.bottom)
                {
                    int overflow = bottom - rect.bottom;
                    Rectangle(mDC, car[i].left, 0, car[i].right, overflow);
                }

                SelectObject(mDC, oldBrush);
                DeleteObject(hBrush);
            }

            // 자동차 (위)
            for (int i = 6; i < 8; i++)
            {
                hBrush = CreateSolidBrush(car[i].RGB);
                oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

                int top = car[i].top;
                int bottom = car[i].bottom;

                int drawT = max(0, top);
                int drawB = min(rect.bottom, bottom);

                if (drawT < drawB)
                    Rectangle(mDC, car[i].left, drawT, car[i].right, drawB);

                if (top < 0)
                {
                    int overflow = -top;
                    Rectangle(mDC,
                        car[i].left,
                        rect.bottom - overflow,
                        car[i].right,
                        rect.bottom);
                }

                SelectObject(mDC, oldBrush);
                DeleteObject(hBrush);
            }

            // 사람
            hBrush = CreateSolidBrush(RGB(30, 30, 30));
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

            Ellipse(mDC, pX, pY, pX + 30, pY + 30);

            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);

            // 신호등 박스
            hBrush = CreateSolidBrush(RGB(30, 30, 30));
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

            Rectangle(mDC,
                rect.right / 2 + 20,
                rect.bottom / 2 - 165,
                rect.right / 2 + 140,
                rect.bottom / 2 - 205);

            Rectangle(mDC,
                rect.right / 2 + 155,
                rect.bottom / 2 + 20,
                rect.right / 2 + 195,
                rect.bottom / 2 + 140);

            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);

            // H 신호등
            hBrush = CreateSolidBrush(H_Red ? RGB(255, 40, 40) : RGB(80, 0, 0));
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
            Ellipse(mDC, rect.right / 2 + 20, rect.bottom / 2 - 165, rect.right / 2 + 60, rect.bottom / 2 - 205);
            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);

            hBrush = CreateSolidBrush(H_Yellow ? RGB(255, 220, 0) : RGB(80, 80, 0));
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
            Ellipse(mDC, rect.right / 2 + 60, rect.bottom / 2 - 165, rect.right / 2 + 100, rect.bottom / 2 - 205);
            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);

            hBrush = CreateSolidBrush(H_Green ? RGB(0, 255, 80) : RGB(0, 80, 20));
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
            Ellipse(mDC, rect.right / 2 + 100, rect.bottom / 2 - 165, rect.right / 2 + 140, rect.bottom / 2 - 205);
            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);

            // V 신호등
            hBrush = CreateSolidBrush(V_Red ? RGB(255, 40, 40) : RGB(80, 0, 0));
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
            Ellipse(mDC, rect.right / 2 + 155, rect.bottom / 2 + 20, rect.right / 2 + 195, rect.bottom / 2 + 60);
            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);

            hBrush = CreateSolidBrush(V_Yellow ? RGB(255, 220, 0) : RGB(80, 80, 0));
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
            Ellipse(mDC, rect.right / 2 + 155, rect.bottom / 2 + 60, rect.right / 2 + 195, rect.bottom / 2 + 100);
            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);

            hBrush = CreateSolidBrush(V_Green ? RGB(0, 255, 80) : RGB(0, 80, 20));
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
            Ellipse(mDC, rect.right / 2 + 155, rect.bottom / 2 + 100, rect.right / 2 + 195, rect.bottom / 2 + 140);
            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);

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
                if (speedControl < 3)
                {
                    speedControl++;
                    for (int i = 0; i < 8; i++)
                        car[i].speed++;
                }
            }
            else if (wParam == '-')
            {
                if (speedControl > -3)
                {
                    speedControl--;
                    for (int i = 0; i < 8; i++)
                        car[i].speed--;
                }
            }
            break;

        case WM_KEYDOWN:
            if (wParam == 'Q')
                PostQuitMessage(0);
            else if (wParam == 'A')
            {
                if (signal == false)
                {
                    signal = true;
                    KillTimer(hWnd, 2);
                    SetTimer(hWnd, 2, 1000, NULL);
                }
                else
                {
                    signal = false;
                    KillTimer(hWnd, 2);
                }
            }
            break;

        case WM_LBUTTONDOWN:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            if (x <= rect.right / 2 + 60 && x >= rect.right / 2 + 20 &&
                y >= rect.bottom / 2 - 205 && y <= rect.bottom / 2 - 165)
            {
                H_Red = true; H_Yellow = false; H_Green = false;
                V_Red = false; V_Yellow = false; V_Green = true;
                Vtimer = Htimer = 0;
            }
            else if (x <= rect.right / 2 + 100 && x >= rect.right / 2 + 60 &&
                y >= rect.bottom / 2 - 205 && y <= rect.bottom / 2 - 165)
            {
                H_Red = false; H_Yellow = true; H_Green = false;
                V_Red = true; V_Yellow = false; V_Green = false;
                Htimer = 0; Vtimer = 5;
            }
            else if (x <= rect.right / 2 + 140 && x >= rect.right / 2 + 100 &&
                y >= rect.bottom / 2 - 205 && y <= rect.bottom / 2 - 165)
            {
                H_Red = false; H_Yellow = false; H_Green = true;
                V_Red = true; V_Yellow = false; V_Green = false;
                Vtimer = Htimer = 0;
            }
            else if (x >= rect.right / 2 + 155 && x <= rect.right / 2 + 195 &&
                y >= rect.bottom / 2 + 20 && y <= rect.bottom / 2 + 60)
            {
                H_Red = false; H_Yellow = false; H_Green = true;
                V_Red = true; V_Yellow = false; V_Green = false;
                Vtimer = Htimer = 0;
            }
            else if (x >= rect.right / 2 + 155 && x <= rect.right / 2 + 195 &&
                y >= rect.bottom / 2 + 60 && y <= rect.bottom / 2 + 100)
            {
                H_Red = true; H_Yellow = false; H_Green = false;
                V_Red = false; V_Yellow = true; V_Green = false;
                Htimer = 5; Vtimer = 0;
            }
            else if (x >= rect.right / 2 + 155 && x <= rect.right / 2 + 195 &&
                y >= rect.bottom / 2 + 100 && y <= rect.bottom / 2 + 140)
            {
                H_Red = true; H_Yellow = false; H_Green = false;
                V_Red = false; V_Yellow = false; V_Green = true;
                Vtimer = Htimer = 0;
            }
            else
            {
                H_Red = true; H_Yellow = false; H_Green = false;
                V_Red = true; V_Yellow = false; V_Green = false;
                Vtimer = Htimer = 0;
            }
            break;
        }

        case WM_RBUTTONDOWN:
            H_Red = true; H_Yellow = false; H_Green = false;
            V_Red = false; V_Yellow = false; V_Green = true;
            Vtimer = Htimer = 0;
            break;
    }

    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}