#include "WindowManager.h"

std::unique_ptr<WindowManager> WindowManager::window_manager = nullptr;

WindowManager::WindowManager(HINSTANCE hinstance) {
	m_hinstance = hinstance;
}

WindowManager::~WindowManager() {
    // 벡터에 옮겨 담기
    std::vector<std::wstring> window_info_vector;

    for (const auto& v : m_window_info_map) {
        window_info_vector.emplace_back(v.first);
    }

    // 역순으로 윈도우 파괴
    for (auto i = window_info_vector.rbegin(); i != window_info_vector.rend(); ++i) {
        Destroy_Window(*i);
    }
}

WindowManager& WindowManager::Create_Instance(HINSTANCE hinstance) {
	if (window_manager == nullptr) {
		window_manager = std::make_unique<WindowManager>(hinstance);
	}

	return *window_manager;
}

WindowManager& WindowManager::Get_Instance() {
	return *window_manager;
}

HWND WindowManager::Create_Window(UINT style, WNDPROC window_procedure, LPCWSTR window_name,
    int window_x, int window_y, int client_width, int client_height,
    DWORD dwstyle, DWORD dwstyle_ex, int show_flag,
	LPCWSTR menu_name,
    HWND parent_hwnd, HMENU hmenu, LPVOID parameter,
	int icon_id, LPWSTR cursor_id, int small_icon_id,
	int background_r, int background_g, int background_b,
	int class_extra, int window_extra
) {
    // 윈도우 정보 구조체 생성
    Window_Info window_info;

    // 윈도우 클래스 설정
    WNDCLASSEXW window_class;

    window_class.cbSize = sizeof(WNDCLASSEXW);

    window_class.style = style;
    window_class.lpfnWndProc = window_procedure;
    window_class.cbClsExtra = class_extra;
    window_class.cbWndExtra = window_extra;
    window_class.hInstance = m_hinstance;
    window_class.hIcon = LoadIcon(m_hinstance, MAKEINTRESOURCE(icon_id));
    window_class.hCursor = LoadCursor(nullptr, cursor_id);
    window_class.lpszClassName = window_name;   // 윈도우 클래스 이름은 윈도우 이름과 동일
    window_class.hIconSm = LoadIcon(m_hinstance, MAKEINTRESOURCE(small_icon_id));

    // 배경색 지정시 브러쉬 생성
    if (background_r == -1 && background_g == -1 && background_b == -1) {
        window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

        window_info.background_brush = nullptr;
    }
    else {
        window_class.hbrBackground = CreateSolidBrush(RGB(background_r, background_g, background_b));

        window_info.background_r = background_r;
        window_info.background_g = background_g;
        window_info.background_b = background_b;
        window_info.background_brush = window_class.hbrBackground;
    }

    // 메뉴 사용시 메뉴 설정
    if (menu_name == nullptr) {
        window_class.lpszMenuName = 0;
    }
    else {
        window_class.lpszMenuName = MAKEINTRESOURCEW(IDC_BENGINE);
    }

    // 윈도우 클래스 등록
    if (!RegisterClassExW(&window_class)) {
        MessageBox(0, L"RegisterClassExW() Failed!!", 0, 0);

        return nullptr;
    }

    // 클라이언트 사이즈에 따른 윈도우 사이즈 계산
    window_info.client_width = client_width;
    window_info.client_height = client_height;

    RECT client_rect = { 0, 0, client_width, client_height };
    bool menu = menu_name == nullptr ? false : true;
    AdjustWindowRectEx(&client_rect, dwstyle, menu, dwstyle_ex);
    window_info.window_width = client_rect.right - client_rect.left;
    window_info.window_height = client_rect.bottom - client_rect.top;

    // 윈도우 생성
    HWND hwnd = CreateWindowExW(dwstyle_ex, window_name, window_name, dwstyle,
        window_x, window_y, window_info.window_width, window_info.window_height,
        parent_hwnd, hmenu, m_hinstance, parameter);

    if (!hwnd) {
        MessageBox(0, L"CreateWindowExW() Failed!!", 0, 0);

        return nullptr;
    }

    window_info.hwnd = hwnd;

    // 맵에 저장
    m_window_info_map[std::wstring(window_name)] = window_info;

    // 윈도우 가시성 업데이트
    ShowWindow(hwnd, show_flag);
    UpdateWindow(hwnd);

    return hwnd;
}

void WindowManager::Destroy_Window(std::wstring window_name) {
    Window_Info window_info = m_window_info_map[window_name];

    // 브러쉬 해제
    if (window_info.background_brush != nullptr) {
        DeleteObject(window_info.background_brush);
    }

    // 윈도우 파괴
    DestroyWindow(window_info.hwnd);

    // 맵에서 삭제
    m_window_info_map.erase(window_name);
}

HWND WindowManager::Center_To_Screen(std::wstring window_name) {
    // 윈도우 인포 가져오기
    Window_Info window_info = m_window_info_map[window_name];

    // 화면 정보 가져오기
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    // 화면 중앙으로 윈도우 옮기기
    SetWindowPos(window_info.hwnd, HWND_TOP,
        (screen_width - window_info.window_width) / 2, (screen_height - window_info.window_height) / 2,
        window_info.window_width, window_info.window_height, 0);

    return window_info.hwnd;
}

HWND WindowManager::Get_HWND(std::wstring window_name) {
    return m_window_info_map[window_name].hwnd;
}

Window_Info& WindowManager::Get_Window_Info(std::wstring window_name) {
    return m_window_info_map[window_name];
}