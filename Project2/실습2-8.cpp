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

    static int isInsert = 0;

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
            if (row < 9)
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
        else if (wParam == VK_TAB)
        {
            if (isInsert == 0)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (col + i > 30&&row<9)
                    {
                        col = 0;
                        row++;
                    }
                    else if (row == 9 && col + i>30)
                    {
                        row = 0;
                        col = 0;
                    }
                    str[row][col++] = ' ';
                }
            }
            else if (isInsert == 1)
            {
                for (int k = 0; k < 4; k++)
                {
                    TCHAR carry = ' ';
                    for (int i = row; i < 10; i++)
                    {
                        TCHAR last = str[i][29];
                        if (i == row)
                        {
                            for (int j = 29; j > col; j--)
                                str[i][j] = str[i][j - 1];
                            str[i][col] = carry;
                        }
                        else
                        {
                            for (int j = 29; j > 0; j--)
                                str[i][j] = str[i][j - 1];
                            str[i][0] = carry;
                        }
                        carry = last;
                        if (carry == '\0')
                            break;
                    }
                    if (col < 29)
                        col++;
                    else
                    {
                        col = 0;
                        if (row < 9) row++;
                    }
                }
               
            }
        }
        else
        {
            if (col < 30&&isInsert==0)
            {
                str[row][col++] = (TCHAR)wParam;
            }
            else if (col == 30 && row < 9)
            {
                col = 0;
                row++;
            }
            else if (row == 9 && col == 30)
            {
                col = 0;
                row = 0;
            }
            else if (isInsert == 1)
            {
                TCHAR carry = (TCHAR)wParam;
                for (int i = row; i < 10; i++)
                {
                    TCHAR last = str[i][29];
                    if (i == row)
                    {
                        for (int j = 29; j > col; j--)
                            str[i][j] = str[i][j - 1];
                        str[i][col] = carry;
                    }
                    else
                    {
                        for (int j = 29; j > 0; j--)
                            str[i][j] = str[i][j - 1];
                        str[i][0] = carry;
                    }
                    carry = last;
                    if (carry == '\0')
                        break;
                }
                if (col < 29)
                    col++;
                else
                {
                    col = 0;
                    if (row < 9) row++;
                }
            }
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
            if (col > 0)
                col--;
            else if (col == 0)
            {
                col = 0;
            }
            break;

        case VK_RIGHT:
            if (col < lstrlen(str[row]))
                col++;
            else if (col==lstrlen(str[row]))
            {
                col=lstrlen(str[row]);
            }
            break;

        case VK_UP:
            if (row > 0)
            {
                row--;
                if (col > lstrlen(str[row]))
                    col = lstrlen(str[row]);
            }
            break;

        case VK_DOWN:
			if (row < 9)
			{
                if (lstrlen(str[row]) == 0)
                    break;
                row++;
                if (col > lstrlen(str[row]))
                    col = lstrlen(str[row]);
			}
            break;
        case VK_HOME:
            col = 0;
            break;
        case VK_END:
            if (lstrlen(str[row]) == 30)
            {
                col = 0;
                row++;
            }
            else
            {
                col = lstrlen(str[row]);
            }
            break;
        case VK_INSERT:
            if (isInsert == 1)
            {
                isInsert = 0;
            }
            else
                isInsert = 1;
            break;
        case VK_PRIOR:
            row = (row + 7) % 10;
            if (lstrlen(str[row]) == 0)
                col = 0;
            else if (col > lstrlen(str[row]))
                col = lstrlen(str[row]);
            break;
		case VK_NEXT:
            row = (row + 3) % 10;
            if (lstrlen(str[row]) == 0)
                col = 0;
            else if (col > lstrlen(str[row]))
                col = lstrlen(str[row]);

			break;
        case VK_DELETE:
            int i = row;
            int j = col;

            int endRow = i;
            int endCol = j;
            while (endRow < 10)
            {
                if (str[endRow][endCol] == ' ' || str[endRow][endCol] == '\0')
                    break;
                endCol++;
                if (endCol >= 30)
                {
                    endCol = 0;
                    endRow++;
                }
            }
            while (endRow < 10)
            {
                if (endCol < 29)
                {
                    str[i][j] = str[endRow][endCol + 1];
                    endCol++;
                }
                else
                {
                    if (endRow < 9)
                    {
                        str[i][j] = str[endRow + 1][0];
                        endRow++;
                        endCol = 0;
                    }
                    else
                    {
                        str[i][j] = '\0';
                        break;
                    }
                }
                j++;
                if (j >= 30)
                {
                    j = 0;
                    i++;
                }
            }
            break;
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
        GetTextExtentPoint32(hDC, str[row], col, &size);
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