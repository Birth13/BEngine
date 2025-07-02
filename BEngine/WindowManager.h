#pragma once
#include "common.h"

struct Window_Info {
	HWND hwnd = nullptr;
	int client_width = -1, client_height = -1;
	int background_r = -1, background_g = -1, background_b = -1;
	HBRUSH background_brush = nullptr;
};

class WindowManager {
private:
	static std::unique_ptr<WindowManager> window_manager;

	HINSTANCE m_hinstance = nullptr;

	std::unordered_map<std::wstring, Window_Info> m_window_info_map;

public:
	WindowManager() = delete;	// 기본 생성자 삭제
	WindowManager(HINSTANCE hinstance);
	~WindowManager();

	WindowManager(const WindowManager&) = delete;	// 복사 생성자 삭제
	WindowManager& operator=(const WindowManager&) = delete;	// 대입 연산자 삭제

	// 인스턴스 관련

	static WindowManager& Create_Instance(HINSTANCE hinstance);
	static WindowManager& Get_Instance();

	// 윈도우 관련
	
	HWND Create_Window(UINT style, WNDPROC window_procedure, LPCWSTR window_name,
		int window_x, int window_y, int client_width, int client_height,
		DWORD dwstyle, DWORD dwstyle_ex,
		LPCWSTR menu_name = nullptr,
		HWND parent_hwnd = nullptr, HMENU hmenu = nullptr, LPVOID parameter = nullptr,
		int icon_ID = IDI_BENGINE, LPWSTR cursor_ID = IDC_ARROW, int small_icon_ID = IDI_SMALL,
		int background_r = -1, int background_g = -1, int background_b = -1,
		int class_extra = 0, int window_extra = 0
		);
	void Destroy_Window(std::wstring window_name);

	HWND Get_HWND(std::wstring window_name);
};

