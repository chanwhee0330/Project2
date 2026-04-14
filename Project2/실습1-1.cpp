#include <windows.h>
#include <Windowsx.h>
#include <tchar.h>
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name"; 
LPCTSTR lpszWindowName = L"window program 1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass); // 본 구조체의 크기
	WndClass.style = CS_HREDRAW | CS_VREDRAW; // 윈도우 클래스의 스타일을 나타냄 (출력 스타일)
											  // CS_HEADRAW / CS_VREDRAW -> 작업영역의 폭 / 높이가 변경되면 윈도우를 다시 그린다. 
											  // CS_DBCLKS -> 마우스 더블 클릭 메세지를 보낸다
											  // CS_CLASSDC -> 이 클래스로부터 만들어진 모든 윈도우가 하나의 DC를 공유한다
											  // CS_OWNDC -> 각 윈도우가 하난의 DC를 독점적으로 사용한다
											  // CS_PARENTDC - 자식 윈도우가 부모 윈도우의 DC를  
	WndClass.lpfnWndProc = (WNDPROC)WndProc; // 윈도우 메시지를 처리하는 윈도우 프로시저 함수 이름
	WndClass.cbClsExtra = 0; // 클래스 여분 메모리 -> 사용 안함
	WndClass.cbWndExtra = 0; // 윈도우 여분 메모리 -> 사용 안함
	WndClass.hInstance = hInstance; // WinMain 함수의 인수로 전달된 hInstance값을 사용
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 실행 파일에 쓰일 아이콘 지정 IDI_APPLICATION / IDI_ASTERISK / IDI_EXCLAMATION / IDI_HAND / IDI_QUESTION / IDI_ERROR / IDI_WARNING / IDI_INFORMATION
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // 윈도우에 쓰일 커서 지정 IDC_APPSTARTING / IDC_ARROW / IDC_CROSS / IDC_HAND / IDC_HELP / IDC_IBEAM / IDC_SIZEALL / IDC_SIZESW / IDC_SIZENS / IDC_SIZENWSE / IDC_SIZEWE / IDC_UPARROW / IDC_WAIT
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // 윈도우의 배경색, 기본색 또는 임의의 색 지정 가능. GetStockObject 함수의 인자값에 BLACK_BRUSH / WHITE_BRUSH / DKGRAY_BRUSH / LTGRAY_BRUSH / GRAY_BRUSH
	WndClass.lpszMenuName = NULL; // 메뉴의 이름, 컨트롤의 id로도 사용된다
	WndClass.lpszClassName = lpszClass; // 이 윈도우 클래스의 이름
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // 작은 아이콘에 쓰일 아이콘을 지정
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPED, 0, 0, 1280, 960, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	TCHAR temp[] = TEXT("");
	int x = 0, y = 0;
	switch (iMessage) {
	case WM_PAINT: // 윈도우가 다시 그려져야 할 때 
		hDC = BeginPaint(hWnd, &ps);
		TextOut(hDC, x, y, temp, lstrlen(temp));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: // 윈도우가 없어질 때 
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}