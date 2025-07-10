#include "WindowManager.h"

std::unique_ptr<WindowManager> WindowManager::window_manager = nullptr;

WindowManager::WindowManager(HINSTANCE hinstance) {
	m_hinstance = hinstance;
}

WindowManager::~WindowManager() {
    // ���Ϳ� �Ű� ���
    std::vector<std::wstring> window_info_vector;

    for (const auto& v : m_window_info_map) {
        window_info_vector.emplace_back(v.first);
    }

    // �������� ������ �ı�
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
    // ������ ���� ����ü ����
    Window_Info window_info;

    // ������ Ŭ���� ����
    WNDCLASSEXW window_class;

    window_class.cbSize = sizeof(WNDCLASSEXW);

    window_class.style = style;
    window_class.lpfnWndProc = window_procedure;
    window_class.cbClsExtra = class_extra;
    window_class.cbWndExtra = window_extra;
    window_class.hInstance = m_hinstance;
    window_class.hIcon = LoadIcon(m_hinstance, MAKEINTRESOURCE(icon_id));
    window_class.hCursor = LoadCursor(nullptr, cursor_id);
    window_class.lpszClassName = window_name;   // ������ Ŭ���� �̸��� ������ �̸��� ����
    window_class.hIconSm = LoadIcon(m_hinstance, MAKEINTRESOURCE(small_icon_id));

    // ���� ������ �귯�� ����
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

    // �޴� ���� �޴� ����
    if (menu_name == nullptr) {
        window_class.lpszMenuName = 0;
    }
    else {
        window_class.lpszMenuName = MAKEINTRESOURCEW(IDC_BENGINE);
    }

    // ������ Ŭ���� ���
    if (!RegisterClassExW(&window_class)) {
        MessageBox(0, L"RegisterClassExW() Failed!!", 0, 0);

        return nullptr;
    }

    // Ŭ���̾�Ʈ ����� ���� ������ ������ ���
    window_info.client_width = client_width;
    window_info.client_height = client_height;

    RECT client_rect = { 0, 0, client_width, client_height };
    bool menu = menu_name == nullptr ? false : true;
    AdjustWindowRectEx(&client_rect, dwstyle, menu, dwstyle_ex);
    window_info.window_width = client_rect.right - client_rect.left;
    window_info.window_height = client_rect.bottom - client_rect.top;

    // ������ ����
    HWND hwnd = CreateWindowExW(dwstyle_ex, window_name, window_name, dwstyle,
        window_x, window_y, window_info.window_width, window_info.window_height,
        parent_hwnd, hmenu, m_hinstance, parameter);

    if (!hwnd) {
        MessageBox(0, L"CreateWindowExW() Failed!!", 0, 0);

        return nullptr;
    }

    window_info.hwnd = hwnd;

    // �ʿ� ����
    m_window_info_map[std::wstring(window_name)] = window_info;

    // ������ ���ü� ������Ʈ
    ShowWindow(hwnd, show_flag);
    UpdateWindow(hwnd);

    return hwnd;
}

void WindowManager::Destroy_Window(std::wstring window_name) {
    Window_Info window_info = m_window_info_map[window_name];

    // �귯�� ����
    if (window_info.background_brush != nullptr) {
        DeleteObject(window_info.background_brush);
    }

    // ������ �ı�
    DestroyWindow(window_info.hwnd);

    // �ʿ��� ����
    m_window_info_map.erase(window_name);
}

HWND WindowManager::Center_To_Screen(std::wstring window_name) {
    // ������ ���� ��������
    Window_Info window_info = m_window_info_map[window_name];

    // ȭ�� ���� ��������
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    // ȭ�� �߾����� ������ �ű��
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