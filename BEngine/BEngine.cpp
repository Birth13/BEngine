#include "common.h"
#include "BEngine.h"
#include "WindowManager.h"


// GDI+
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름

ULONG_PTR gdiplusToken;

// 이 코드 모듈에 포함된 함수의 선언을 전달
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Splash Window
LRESULT CALLBACK SplashWndProc(HWND, UINT, WPARAM, LPARAM);
void ShowSplashWindow(HINSTANCE);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    //UNREFERENCED_PARAMETER(hPrevInstance);
    //UNREFERENCED_PARAMETER(lpCmdLine);

    // splash window
    ShowSplashWindow(hInstance);

    //// 전역 문자열을 초기화
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_BENGINE, szWindowClass, MAX_LOADSTRING);
    //MyRegisterClass(hInstance);

    //// 애플리케이션 초기화를 수행
    //if (!InitInstance (hInstance, nCmdShow))
    //{
    //    return FALSE;
    //}

    //
    WindowManager::Create_Instance(hInstance).Create_Window(
        CS_HREDRAW | CS_VREDRAW, WndProc, L"main_window",
        10, 10, 800, 600, WS_OVERLAPPEDWINDOW, 0);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BENGINE));

    MSG msg;

    // 기본 메시지 루프
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEXW);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BENGINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BENGINE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 생성
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장
//        주 프로그램 창을 만든 다음 표시
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리
//  WM_PAINT    - 주 창을 그립
//  WM_DESTROY  - 종료 메시지를 게시하고 반환
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // 배경 클리어
            RECT rect;
            GetClientRect(hWnd, &rect);
            FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

            // 출력
            SetBkMode(hdc, TRANSPARENT);
            DrawTextW(hdc, L"Hello, World!!", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYUP:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Splash Window
LRESULT CALLBACK SplashWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static Gdiplus::Image* splashImage = nullptr;

    switch (msg) {
    case WM_CREATE:
        // 이미지를 로드
        splashImage = Gdiplus::Image::FromFile(L"image\\icon.png");
        return 0;
    case WM_PAINT:
        // 이미지를 출력
        if (splashImage) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            Gdiplus::Graphics graphic(hdc);
            graphic.DrawImage(splashImage, 0, 0, 400, 400);

            EndPaint(hWnd, &ps);
        }
        return 0;
    case WM_DESTROY:
        // 이미지 해제
        delete[] splashImage;
        splashImage = nullptr;
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ShowSplashWindow(HINSTANCE hInstance) {
    // GDI+ 초기화
    Gdiplus::GdiplusStartupInput gdiStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiStartupInput, nullptr);

    // 윈도우 생성
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = SplashWndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SplashWindowClass";
    RegisterClass(&wc);

    HWND hSplash = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
        L"SplashWindowClass", L"Splash",
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        nullptr, nullptr, hInstance, nullptr
    );

    // 중앙 배치
    RECT rc;
    GetClientRect(hSplash, &rc);
    int w = 400, h = 400;
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hSplash, HWND_TOP,
        (screenW - w) / 2, (screenH - h) / 2, w, h, 0);

    ShowWindow(hSplash, SW_SHOWNORMAL);
    UpdateWindow(hSplash);

    // 메세지 루프 (지금은 임시로 3초. 실제로는 로딩 시간 동안)
    DWORD start = GetTickCount();
    MSG msg;
    while (GetTickCount() - start < 3000) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Sleep(10);
    }

    // Shutdown
    DestroyWindow(hSplash);
    Gdiplus::GdiplusShutdown(gdiplusToken);
}