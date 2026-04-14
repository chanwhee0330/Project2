#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <stdbool.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"WindowClass";
LPCTSTR lpszWindowName = L"Random Text Program";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int winx = 1000, winy = 1000;

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
    HDC hDC;
    PAINTSTRUCT ps;
    HBRUSH qPen,wPen,ePen,rPen;
    static bool isQ=false, isW=false, isE=false, isR=false;
    static int position = 1;
    static int aPos = 1, bPos = 2, cPos = 3,dPos=4;
    static COLORREF acolor = RGB(rand() % 256, rand() % 256, rand() % 256);
    static COLORREF bcolor = RGB(rand() % 256, rand() % 256, rand() % 256);
    static COLORREF ccolor = RGB(rand() % 256, rand() % 256, rand() % 256);
    static COLORREF dcolor = RGB(rand() % 256, rand() % 256, rand() % 256);
    static COLORREF newcolor;
    switch (iMessage) {
    case WM_PAINT:
    {
        hDC = BeginPaint(hWnd, &ps);
        Rectangle(hDC,300,300,600,600);
        if (isQ == true||cPos==1&&isQ==false&&isW==false&&isE==false&&isR==false) //q가 원 w가 모래시게 e가 오각형 r이 파이 1이 모래시계 2가 오각형 3이 원 4가 파이 
        {
            qPen = CreateSolidBrush(newcolor);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, qPen);
            Ellipse(hDC, 300, 375, 600, 525);
            SelectObject(hDC, oldBrush);
            DeleteObject(qPen);
        }
        if (isW == true || aPos == 1 && isQ == false && isW == false && isE == false && isR == false)
        {
            wPen = CreateSolidBrush(newcolor);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, wPen);
            POINT point[10] = { {300,300},{300,600}, {450,450}, {600,300}, {600,600} };
            Polygon(hDC, point, 5);
            SelectObject(hDC, oldBrush);
            DeleteObject(wPen);
        }
        if (isE == true||bPos==1 && isQ == false && isW == false && isE == false && isR == false)
        {
            ePen = CreateSolidBrush(newcolor);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, ePen);
            POINT point[10] = {{450, 570},{336, 486},{378, 354},{522, 354},{564, 486} };
            Polygon(hDC, point, 5);
            SelectObject(hDC, oldBrush);
            DeleteObject(ePen);
        }
        if (isR == true||dPos==1 && isQ == false && isW == false && isE == false && isR == false)
        {
            rPen = CreateSolidBrush(newcolor);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, rPen);
            Pie(hDC, 300, 300, 600, 600, 600, 450, 450, 300);
            SelectObject(hDC, oldBrush);
            DeleteObject(rPen);
        }

        if (aPos == 1)
        {
            if (isW == true)
            {
                wPen = CreateSolidBrush(newcolor);
            }
            else
            {
                wPen = CreateSolidBrush(bcolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, wPen);
            POINT point[10] = { {300,0},{600,0}, {450,150}, {300,300}, {600,300} };
            Polygon(hDC, point, 5);
            SelectObject(hDC, oldBrush);
            DeleteObject(wPen);
        }
        else if (aPos == 2)
        {
            if (isW == true)
            {
                wPen = CreateSolidBrush(newcolor);
            }
            else
            {
                wPen = CreateSolidBrush(bcolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, wPen);
            POINT point[10] = { {600,300},{900,300}, {750,450}, {600,600}, {900,600} };
            Polygon(hDC, point, 5);
            SelectObject(hDC, oldBrush);
            DeleteObject(wPen);
        }
        else if (aPos == 3)
        {
            if (isW == true)
            {
                wPen = CreateSolidBrush(newcolor);
            }
            else
            {
                wPen = CreateSolidBrush(bcolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, wPen);
            POINT point[10] = { {300,600},{600,600}, {450,750}, {300,900}, {600,900} };
            Polygon(hDC, point, 5);
            SelectObject(hDC, oldBrush);
            DeleteObject(wPen);
        }
        else if (aPos == 4)
        {
            if (isW == true)
            {
                wPen = CreateSolidBrush(newcolor);
            }
            else
            {
                wPen = CreateSolidBrush(bcolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, wPen);
            POINT point[10] = { {0,300},{300,300}, {150,450}, {000,600}, {300,600} };
            Polygon(hDC, point, 5);
             SelectObject(hDC, oldBrush);
             DeleteObject(wPen);
        }

        if (bPos == 1)
        {
            if (isE == true)
            {
                ePen = CreateSolidBrush(newcolor);
            }
            else
            {
                ePen = CreateSolidBrush(ccolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, ePen);
            POINT pentagon[10] = { {450,30},{564,113},{520,246},{380,246},{336,113} };
            Polygon(hDC, pentagon, 5);
            SelectObject(hDC, oldBrush);
            DeleteObject(ePen);
        }
        else if (bPos == 2)
        {
            if (isE == true)
            {
                ePen = CreateSolidBrush(newcolor);
            }
            else
            {
                ePen = CreateSolidBrush(ccolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, ePen);
            POINT pentagon[10] = { {750,330},{864,413},{820,546},{680,546},{636,413} };
            Polygon(hDC, pentagon, 5);
            SelectObject(hDC, oldBrush);
            DeleteObject(ePen);
        }
        else if (bPos == 3)
        {
            if (isE == true)
            {
                ePen = CreateSolidBrush(newcolor);
            }
            else
            {
                ePen = CreateSolidBrush(ccolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, ePen);
            POINT pentagon[10] = { {450,630},{564,713},{520,846},{380,846},{336,713} };
            Polygon(hDC, pentagon, 5);
            SelectObject(hDC, oldBrush);
            DeleteObject(ePen);
        }
        else if (bPos == 4)
        {
            if (isE == true)
            {
                ePen = CreateSolidBrush(newcolor);
            }
            else
            {
                ePen = CreateSolidBrush(ccolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, ePen);
            POINT pentagon[10] = { {150,330},{264,413},{220,546},{80,546},{36,413} };
            Polygon(hDC, pentagon, 5);
            SelectObject(hDC, oldBrush);
            DeleteObject(ePen);
        }

		if (cPos == 1)
		{
			if (isQ == true)
			{
				qPen = CreateSolidBrush(newcolor);
			}
			else
			{
				qPen = CreateSolidBrush(acolor);
			}
			HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, qPen);
			Ellipse(hDC, 300, 000, 600, 300);
			SelectObject(hDC, oldBrush);
			DeleteObject(qPen);
		}
        else if (cPos == 2)
        {
            if (isQ == true)
            {
                qPen = CreateSolidBrush(newcolor);
            }
            else
            {
                qPen = CreateSolidBrush(acolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, qPen);
            Ellipse(hDC, 600, 300, 900, 600);
            SelectObject(hDC, oldBrush);
            DeleteObject(qPen);
        }
        else if (cPos == 3)
        {
            if (isQ == true)
            {
                qPen = CreateSolidBrush(newcolor);
            }
            else
            {
                qPen = CreateSolidBrush(acolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, qPen);
            Ellipse(hDC, 300, 600, 600, 900);
            SelectObject(hDC, oldBrush);
            DeleteObject(qPen);
        }
        else if (cPos == 4)
        {
            if (isQ == true)
            {
                qPen = CreateSolidBrush(newcolor);
            }
            else
            {
                qPen = CreateSolidBrush(acolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, qPen);
            Ellipse(hDC, 000, 300, 300, 600);
            SelectObject(hDC, oldBrush);
            DeleteObject(qPen);
        }

        if (dPos == 1)
        {
            if (isR == true)
            {
                rPen = CreateSolidBrush(newcolor);
            }
            else
            {
                rPen = CreateSolidBrush(dcolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, rPen);
            Pie(hDC, 300, 000, 600, 300, 450, 000, 600, 150);
            SelectObject(hDC, oldBrush);
            DeleteObject(rPen);
        }
        else if (dPos == 2)
        {
            if (isR == true)
            {
                rPen = CreateSolidBrush(newcolor);
            }
            else
            {
                rPen = CreateSolidBrush(dcolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, rPen);
            Pie(hDC, 600, 300, 900, 600, 750, 300, 900, 450);
            SelectObject(hDC, oldBrush);
            DeleteObject(rPen);
        }
        else if (dPos == 3)
        {
            if (isR == true)
            {
                rPen = CreateSolidBrush(newcolor);
            }
            else
            {
                rPen = CreateSolidBrush(dcolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, rPen);
            Pie(hDC, 300, 600, 600, 900, 450, 600, 600, 750);
            SelectObject(hDC, oldBrush);
            DeleteObject(rPen);
        }
        else if (dPos == 4)
        {
            if (isR == true)
            {
                rPen = CreateSolidBrush(newcolor);
            }
            else
            {
                rPen = CreateSolidBrush(dcolor);
            }
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, rPen);
            Pie(hDC, 000, 300, 300, 600, 150, 300, 300, 450);
            SelectObject(hDC, oldBrush);
            DeleteObject(rPen);
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_KEYDOWN:
        if (wParam == 'Q')
        {
            if (!(lParam & (1 << 30)))
            {
                if (wParam == 'Q')
                {
                    isQ = true;
                    newcolor = RGB(rand() % 256, rand() % 256, rand() % 256);
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            }
        }
        else if (wParam == 'W')
        {
            if (!(lParam & (1 << 30)))
            {
                if (wParam == 'W')
                {
                    isW = true;
                    newcolor = RGB(rand() % 256, rand() % 256, rand() % 256);
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            }
        }
        else if (wParam == 'E')
        {
            if (!(lParam & (1 << 30)))
            {
                if (wParam == 'E')
                {
                    isE = true;
                    newcolor = RGB(rand() % 256, rand() % 256, rand() % 256);
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            }
        }
        else if (wParam == 'R')
        {
            if (!(lParam & (1 << 30)))
            {
                if (wParam == 'R')
                {
                    isR = true;
                    newcolor = RGB(rand() % 256, rand() % 256, rand() % 256);
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            }
        }
        else if (wParam == VK_RIGHT)
        {
            aPos++;
            bPos++;
            cPos++;
            dPos++;
            if (aPos > 4) aPos = 1;
            if (bPos > 4) bPos = 1;
            if (cPos > 4) cPos = 1;
            if (dPos > 4) dPos = 1;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == VK_LEFT)
        {
            aPos--;
            bPos--;
            cPos--;
            dPos--;
            if (aPos < 1) aPos = 4;
            if (bPos < 1) bPos = 4;
            if (cPos < 1) cPos = 4;
            if (dPos < 1) dPos = 4;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == VK_UP)
        {
            if (aPos == 1) aPos = 3;
            else if (aPos == 3) aPos = 1;

            if (bPos == 1) bPos = 3;
            else if (bPos == 3) bPos = 1;

            if (cPos == 1) cPos = 3;
            else if (cPos == 3) cPos = 1;

            if (dPos == 1) dPos = 3;
            else if (dPos == 3) dPos = 1;

            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == VK_DOWN)
        {
            if (aPos == 2) aPos = 4;
            else if (aPos == 4) aPos = 2;

            if (bPos == 2) bPos = 4;
            else if (bPos == 4) bPos = 2;

            if (cPos == 2) cPos = 4;
            else if (cPos == 4) cPos = 2;

            if (dPos == 2) dPos = 4;
            else if (dPos == 4) dPos = 2;
            
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_KEYUP:
        if (wParam == 'Q')
        {
            isQ = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == 'W')
        {
            isW = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == 'E')
        {
            isE = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (wParam == 'R')
        {
            isR = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

//2-7 31일 까지 2-8,9 4월 7일까지 