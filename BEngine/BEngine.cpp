#include "common.h"
#include "BEngine.h"
#include "WindowManager.h"

// GDI+
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

// 메인 윈도우 프로시저
LRESULT CALLBACK Main_Window_Procedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

// 스플래시 윈도우 프로시저
LRESULT CALLBACK Splash_Window_Procedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

// 윈도우 메인 함수
int APIENTRY wWinMain(HINSTANCE hinstance, HINSTANCE previous_hinstance, LPWSTR command_line, int command_show) {
	// 메모리 누수 디버그
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// 윈도우 매니저 인스턴스 생성
	WindowManager::Create_Instance(hinstance);

	// 스플래시 윈도우
	{
		// GDI+ 초기화
		ULONG_PTR g_gdiplus_token;
		Gdiplus::GdiplusStartupInput gdiplus_startup_input;
		Gdiplus::GdiplusStartup(&g_gdiplus_token, &gdiplus_startup_input, nullptr);

		// 윈도우 생성
		HWND hwnd = WindowManager::Get_Instance().Create_Window(
			0, Splash_Window_Procedure, L"splash_window",
			CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, SW_HIDE);

		// 윈도우 중앙 설정
		ShowWindow(WindowManager::Get_Instance().Center_To_Screen(L"splash_window"), SW_SHOW);

		// 메세지 루프 (지금은 임시로 3초. 실제로는 로딩 시간 동안)
		DWORD start = GetTickCount();
		MSG message;
		while (GetTickCount() - start < 3000) {
			while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			Sleep(10);
		}

		// 스플래시 윈도우 파괴
		WindowManager::Get_Instance().Destroy_Window(L"splash_window");
		Gdiplus::GdiplusShutdown(g_gdiplus_token);
	}

	// 메인 윈도우
	WindowManager::Get_Instance().Create_Window(
		CS_HREDRAW | CS_VREDRAW, Main_Window_Procedure, L"main_window",
		10, 10, 800, 600, WS_OVERLAPPEDWINDOW, 0);

	MSG message = { 0 };

	// 메세지 루프
	while (message.message != WM_QUIT) {
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else {
			// 게임 로직
		}
	}

	return 0;
}

LRESULT CALLBACK Main_Window_Procedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
	case WM_COMMAND:
	{
		int wmId = LOWORD(wparam);
		// 메뉴 선택을 구문 분석
		switch (wmId)
		{
		case IDM_ABOUT:
			break;
		case IDM_EXIT:
			DestroyWindow(hwnd);
			break;
		default:
			return DefWindowProc(hwnd, message, wparam, lparam);
		}
	}

	return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT paint_struct;
		HDC hdc = BeginPaint(hwnd, &paint_struct);

		// 배경 클리어
		RECT rect;
		GetClientRect(hwnd, &rect);
		FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

		// 출력
		SetBkMode(hdc, TRANSPARENT);
		DrawTextW(hdc, L"Hello, World!!", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		EndPaint(hwnd, &paint_struct);
	}

	return 0;
	case WM_KEYUP:
		if (wparam == VK_ESCAPE) {
			PostQuitMessage(0);
		}

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
}

LRESULT CALLBACK Splash_Window_Procedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	static Gdiplus::Image* splash_image = nullptr;

	switch (message) {
	case WM_CREATE:
		// 이미지를 로드
		splash_image = Gdiplus::Image::FromFile(L"image\\icon.png");

		return 0;
	case WM_PAINT:
		// 이미지를 출력
		if (splash_image) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			Gdiplus::Graphics graphic(hdc);
			graphic.DrawImage(splash_image, 0, 0, 400, 400);

			EndPaint(hwnd, &ps);
		}

		return 0;
	case WM_DESTROY:
		// 이미지 해제
		delete[] splash_image;
		splash_image = nullptr;

		return 0;
	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
}