#include "RectRenderer.h"

#include "WindowManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "Object.h"
#include "Camera.h"
#include "ObjectManager.h"

struct Polygon_Info {
	std::vector<Vertex_Info> vertices;	// 3개
};

RectRenderer::RectRenderer(std::wstring window_name, int resolution_level) : Renderer(window_name) {
	// 해상도 레벨을 설정
	m_resolution_level = resolution_level;

	// 윈도우 인포 가져오기
	Window_Info window_info = WindowManager::Get_Instance().Get_Window_Info(window_name);

	// 클라이언트 크기 설정
	m_client_width = window_info.client_width;
	m_client_height = window_info.client_height;

	// 최대공약수 구하기
	int greatest_common_divisor = XMHelper::Greatest_Common_Divisor(m_client_width, m_client_height);

	// 화면 비 구하기
	m_ratio_width = m_client_width / greatest_common_divisor;
	m_ratio_height = m_client_height / greatest_common_divisor;

	// 최대 해상도 레벨 설정 및 해상도 레벨 제한
	m_max_resolution_level = m_client_width / m_ratio_width;
	m_resolution_level = XMHelper::Min(m_resolution_level, m_max_resolution_level);

	// 해상도 레벨은 최대 해상도 레벨의 약수로 제한 (연산 낭비 예상)
	while (m_max_resolution_level % m_resolution_level != 0) {
		--m_resolution_level;
	}

	// 버퍼의 크기 설정 (사각형의 개수)
	m_buffer_width = m_ratio_width * m_resolution_level;
	m_buffer_height = m_ratio_height * m_resolution_level;

	// 사각형 하나의 크기 설정
	m_rect_width = m_client_width / m_buffer_width;
	m_rect_height = m_client_height / m_buffer_height;

	// 출력 윈도우의 디바이스 콘텍스트 가져오기
	HDC hdc = GetDC(window_info.hwnd);

	// 호환되는 메모리 DC 생성
	m_memory_dc_handle = CreateCompatibleDC(hdc);
	
	// 비트맵 정보 설정
	BITMAPINFO bitmapinfo = {};
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biWidth = m_client_width;
	bitmapinfo.bmiHeader.biHeight = -m_client_height;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;

	// 비트맵 생성 (백 버퍼)
	m_bitmap_handle = CreateDIBSection(m_memory_dc_handle, &bitmapinfo, DIB_RGB_COLORS,
		(void**)&m_back_buffer, nullptr, 0);
	m_old_bitmap_handle = (HBITMAP)SelectObject(m_memory_dc_handle, m_bitmap_handle);

	ReleaseDC(window_info.hwnd, hdc);

	// z 버퍼 리사이즈
	m_z_buffer.resize(m_client_width * m_client_height);
}

RectRenderer::~RectRenderer() {
	SelectObject(m_memory_dc_handle, m_old_bitmap_handle);
	DeleteObject(m_bitmap_handle);
	DeleteDC(m_memory_dc_handle);
}

void RectRenderer::Prepare_Render() {
}

void RectRenderer::Render(HDC hdc) {
	// 백 버퍼와 Z 버퍼 초기화
	Clear_Back_Buffer();
	Clear_Z_Buffer();

	// 오브젝트 그리기
	for (auto& o : ObjectManager::Get_Instance().Get_Object_Map()) {
		Render_Object(o.second);
	}

	// 백 버퍼에서 윈도우 화면으로 복사
	BitBlt(hdc, 0, 0, m_client_width, m_client_height, m_memory_dc_handle, 0, 0, SRCCOPY);
}

void RectRenderer::Render_Object(Object& object) {
	// 각 매쉬 마다
	for (auto& m : object.Get_Meshes()) {
		// 변환 행렬을 계산해서
		DirectX::XMMATRIX transformation_matrix =
			object.Get_World_Matrix_M() *
			ObjectManager::Get_Instance().Get_Camera(L"main_camera").Get_View_Matrix_M() *
			ObjectManager::Get_Instance().Get_Camera(L"main_camera").Get_Projection_Matrix_M();

		DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationQuaternion(object.Get_Rotation_Quaternion_V());

		// 좌표계 변환이 완료된 정점들을 담을 벡터
		std::vector<Vertex_Info> vertices;
		vertices.resize(m->vertices.size());

		// 변환 행렬을 반영한 정점들을 생성
		for (size_t i = 0; i < m->vertices.size(); ++i) {
			Vertex_Info& origin_vertex_info = m->vertices[i];

			Vertex_Info vertex_info;

			// 위치
			DirectX::XMFLOAT4 position;
			DirectX::XMStoreFloat4(&position,
				DirectX::XMVector3Transform(
					DirectX::XMVectorSetW(
						DirectX::XMLoadFloat3(&origin_vertex_info.position), 1.0f), transformation_matrix));

			// 클립 공간 좌표계에서 NDC(Normalized Device Coordinates)로 변환
			vertex_info.position = { position.x / position.w, position.y / position.w , position.z / position.w };

			// 백 버퍼에 맞게 xy좌표 변환
			vertex_info.position.x = (vertex_info.position.x + 1.0f) * 0.5f * m_buffer_width;
			vertex_info.position.y = (1.0f - vertex_info.position.y) * 0.5f * m_buffer_height;

			// 노말
			DirectX::XMStoreFloat3(&vertex_info.normal,
				DirectX::XMVector3Transform(
					DirectX::XMVectorSetW(
						DirectX::XMLoadFloat3(&origin_vertex_info.normal), 0.0f), rotation_matrix));

			// 탄젠트
			DirectX::XMStoreFloat4(&vertex_info.tangent,
				DirectX::XMVector4Transform(
					DirectX::XMVectorSetW(
						DirectX::XMLoadFloat4(&origin_vertex_info.tangent), 0.0f), rotation_matrix));
			vertex_info.tangent.w = origin_vertex_info.tangent.w;

			// uv
			vertex_info.uv = origin_vertex_info.uv;

			// 머터리얼 인덱스
			vertex_info.material_index = origin_vertex_info.material_index;

			// 벡터에 추가
			vertices[i] = vertex_info;
		}

		// 인덱스 배열에 맞게 폴리곤을 생성하여 출력
		if (m->indices_32.size()) {
			for (size_t i = 0; i < m->indices_32.size(); ++++++i) {
				Polygon_Info polygon_info;
				polygon_info.vertices.emplace_back(vertices[m->indices_32[i]]);
				polygon_info.vertices.emplace_back(vertices[m->indices_32[i + 1]]);
				polygon_info.vertices.emplace_back(vertices[m->indices_32[i + 2]]);

				Render_Polygon(polygon_info);
			}
		}
		else {
			for (size_t i = 0; i < m->indices_16.size(); ++++++i) {
				Polygon_Info polygon_info;
				polygon_info.vertices.emplace_back(vertices[m->indices_16[i]]);
				polygon_info.vertices.emplace_back(vertices[m->indices_16[i + 1]]);
				polygon_info.vertices.emplace_back(vertices[m->indices_16[i + 2]]);

				Render_Polygon(polygon_info);
			}
		}
	}
}

void RectRenderer::Render_Polygon(Polygon_Info& polygon_info) {
	// y의 최대와 최소를 계산
	float minimum_y_float = XMHelper::Min(
		XMHelper::Min(polygon_info.vertices[0].position.y, polygon_info.vertices[1].position.y),
		polygon_info.vertices[2].position.y);
	float maximum_y_float = XMHelper::Max(
		XMHelper::Max(polygon_info.vertices[0].position.y, polygon_info.vertices[1].position.y),
		polygon_info.vertices[2].position.y);

	// 백 버퍼 영역을 벗어나지 않도록 제한
	int minimum_y = XMHelper::Max(0, (int)std::ceil(minimum_y_float));
	int maximum_y = XMHelper::Min(m_buffer_height - 1, (int)std::floor(maximum_y_float));

	// 정점 정보를 잠시 저장할 공용 변수
	Vertex_Info vertex_info;

	// 삼각형이 그려지는 y에 대해서
	for (int y = minimum_y; y <= maximum_y; ++y) {
		std::vector<Vertex_Info> vertex_vector;

		// 삼각형의 세 변을 순회하며
		for (int i = 0; i < 3; ++i) {
			int j = (i + 1) % 3;

			// 해당 y에 그려지면
			if ((polygon_info.vertices[i].position.y <= (float)y &&
				polygon_info.vertices[j].position.y > (float)y) ||
				(polygon_info.vertices[j].position.y <= (float)y &&
				polygon_info.vertices[i].position.y > (float)y)
			) {
				// 그 점을 계산하여 벡터에 추가 (제로 디비전 처리)
				float t = (polygon_info.vertices[i].position.y == polygon_info.vertices[j].position.y) ? 0.5f :
					((float)y - polygon_info.vertices[i].position.y) /
					(polygon_info.vertices[j].position.y - polygon_info.vertices[i].position.y);

				// 정점 보간 (선형)
				vertex_info = Get_Interpolated_Vertex(polygon_info.vertices[i], polygon_info.vertices[j], t);
				vertex_info.position.y = (float)y;

				// 벡터에 추가
				vertex_vector.emplace_back(vertex_info);
			}
		}

		// 만약 점이 2개 미만이면 패스
		if (vertex_vector.size() < 2) {
			continue;
		}

		// 만약 첫 번째 점의 x가 더 크다면 스왑
		if (vertex_vector[0].position.x > vertex_vector[1].position.x) {
			std::swap(vertex_vector[0], vertex_vector[1]);
		}

		// 그려질 x의 최대와 최소를 계산
		int minimum_x = XMHelper::Max(0, (int)std::ceil(vertex_vector[0].position.x));
		int maximum_x = XMHelper::Min(m_buffer_width - 1, (int)std::floor(vertex_vector[1].position.x));

		// 그려야하는 모든 x에 대해서
		for (int x = minimum_x; x <= maximum_x; ++x) {
			float t = (vertex_vector[0].position.x == vertex_vector[1].position.x) ? 0.5f :
				((float)x - vertex_vector[0].position.x) /
				(vertex_vector[1].position.x - vertex_vector[0].position.x);

			// 정점 보간 (선형)
			vertex_info = Get_Interpolated_Vertex(vertex_vector[0], vertex_vector[1], t);
			vertex_info.position.x = (float)x;

			// 사각형 그리기
			Render_Rect(vertex_info);
		}
	}
}

void RectRenderer::Render_Rect(Vertex_Info& vertex_info) {
	int x = (int)vertex_info.position.x;
	int y = (int)vertex_info.position.y;

	// 백 버퍼 범위 체크
	if (x < 0 || x >= m_buffer_width || y < 0 || y >= m_buffer_height) {
		return;
	}

	// 버퍼 내 인덱스 계산
	int buffer_index = y * m_buffer_width + x;

	// 만약 유효한 z값이라면
	if (vertex_info.position.z >= 0.0f &&
		vertex_info.position.z < m_z_buffer[buffer_index]
	) {
		// z buffer 값을 덮어 쓰고
		m_z_buffer[buffer_index] = vertex_info.position.z;

		Light_Info direction_light_info = ObjectManager::Get_Instance().Get_Light_Map()[0];
		float brightness = XMHelper::Max(0.1f, -XMHelper::Dot(vertex_info.normal, direction_light_info.direction));

		Material_Info* material_info = MaterialManager::Get_Instance().Get_Material_Info(vertex_info.material_index);
		int color_r = (int)(material_info->albedo.x * 255.0f * brightness * direction_light_info.strength.x);
		int color_g = (int)(material_info->albedo.y * 255.0f * brightness * direction_light_info.strength.y);
		int color_b = (int)(material_info->albedo.z * 255.0f * brightness * direction_light_info.strength.z);

		// 칠해야 하는 픽셀만큼 칠하기
		for (int i = 0; i < m_rect_height; ++i) {
			for (int j = 0; j < m_rect_width; ++j) {
				int target_x = x * m_rect_width + j;
				int target_y = y * m_rect_height + i;

				m_back_buffer[target_y * m_client_width + target_x] = RGB(color_b, color_g, color_r);
			}
		}
	}
}

void RectRenderer::Clear_Back_Buffer() {
	std::fill_n(m_back_buffer, m_client_width * m_client_height, RGB(0, 0, 0));
}

void RectRenderer::Clear_Z_Buffer() {
	std::fill_n(m_z_buffer.begin(), m_buffer_width * m_buffer_height, 1.0f);
}
