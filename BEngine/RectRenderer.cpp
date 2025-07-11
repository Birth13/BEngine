#include "RectRenderer.h"

#include "WindowManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "Object.h"
#include "Camera.h"
#include "ObjectManager.h"

struct Polygon_Info {
	std::vector<Vertex_Info> vertices;	// 3��
};

RectRenderer::RectRenderer(std::wstring window_name, int resolution_level) : Renderer(window_name) {
	// �ػ� ������ ����
	m_resolution_level = resolution_level;

	// ������ ���� ��������
	Window_Info window_info = WindowManager::Get_Instance().Get_Window_Info(window_name);

	// Ŭ���̾�Ʈ ũ�� ����
	m_client_width = window_info.client_width;
	m_client_height = window_info.client_height;

	// �ִ����� ���ϱ�
	int greatest_common_divisor = XMHelper::Greatest_Common_Divisor(m_client_width, m_client_height);

	// ȭ�� �� ���ϱ�
	m_ratio_width = m_client_width / greatest_common_divisor;
	m_ratio_height = m_client_height / greatest_common_divisor;

	// �ִ� �ػ� ���� ���� �� �ػ� ���� ����
	m_max_resolution_level = m_client_width / m_ratio_width;
	m_resolution_level = XMHelper::Min(m_resolution_level, m_max_resolution_level);

	// �ػ� ������ �ִ� �ػ� ������ ����� ���� (���� ���� ����)
	while (m_max_resolution_level % m_resolution_level != 0) {
		--m_resolution_level;
	}

	// ������ ũ�� ���� (�簢���� ����)
	m_buffer_width = m_ratio_width * m_resolution_level;
	m_buffer_height = m_ratio_height * m_resolution_level;

	// �簢�� �ϳ��� ũ�� ����
	m_rect_width = m_client_width / m_buffer_width;
	m_rect_height = m_client_height / m_buffer_height;

	// ��� �������� ����̽� ���ؽ�Ʈ ��������
	HDC hdc = GetDC(window_info.hwnd);

	// ȣȯ�Ǵ� �޸� DC ����
	m_memory_dc_handle = CreateCompatibleDC(hdc);
	
	// ��Ʈ�� ���� ����
	BITMAPINFO bitmapinfo = {};
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biWidth = m_client_width;
	bitmapinfo.bmiHeader.biHeight = -m_client_height;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;

	// ��Ʈ�� ���� (�� ����)
	m_bitmap_handle = CreateDIBSection(m_memory_dc_handle, &bitmapinfo, DIB_RGB_COLORS,
		(void**)&m_back_buffer, nullptr, 0);
	m_old_bitmap_handle = (HBITMAP)SelectObject(m_memory_dc_handle, m_bitmap_handle);

	ReleaseDC(window_info.hwnd, hdc);

	// z ���� ��������
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
	// �� ���ۿ� Z ���� �ʱ�ȭ
	Clear_Back_Buffer();
	Clear_Z_Buffer();

	// ������Ʈ �׸���
	for (auto& o : ObjectManager::Get_Instance().Get_Object_Map()) {
		Render_Object(o.second);
	}

	// �� ���ۿ��� ������ ȭ������ ����
	BitBlt(hdc, 0, 0, m_client_width, m_client_height, m_memory_dc_handle, 0, 0, SRCCOPY);
}

void RectRenderer::Render_Object(Object& object) {
	// �� �Ž� ����
	for (auto& m : object.Get_Meshes()) {
		// ��ȯ ����� ����ؼ�
		DirectX::XMMATRIX transformation_matrix =
			object.Get_World_Matrix_M() *
			ObjectManager::Get_Instance().Get_Camera(L"main_camera").Get_View_Matrix_M() *
			ObjectManager::Get_Instance().Get_Camera(L"main_camera").Get_Projection_Matrix_M();

		DirectX::XMMATRIX rotation_matrix = DirectX::XMMatrixRotationQuaternion(object.Get_Rotation_Quaternion_V());

		// ��ǥ�� ��ȯ�� �Ϸ�� �������� ���� ����
		std::vector<Vertex_Info> vertices;
		vertices.resize(m->vertices.size());

		// ��ȯ ����� �ݿ��� �������� ����
		for (size_t i = 0; i < m->vertices.size(); ++i) {
			Vertex_Info& origin_vertex_info = m->vertices[i];

			Vertex_Info vertex_info;

			// ��ġ
			DirectX::XMFLOAT4 position;
			DirectX::XMStoreFloat4(&position,
				DirectX::XMVector3Transform(
					DirectX::XMVectorSetW(
						DirectX::XMLoadFloat3(&origin_vertex_info.position), 1.0f), transformation_matrix));

			// Ŭ�� ���� ��ǥ�迡�� NDC(Normalized Device Coordinates)�� ��ȯ
			vertex_info.position = { position.x / position.w, position.y / position.w , position.z / position.w };

			// �� ���ۿ� �°� xy��ǥ ��ȯ
			vertex_info.position.x = (vertex_info.position.x + 1.0f) * 0.5f * m_buffer_width;
			vertex_info.position.y = (1.0f - vertex_info.position.y) * 0.5f * m_buffer_height;

			// �븻
			DirectX::XMStoreFloat3(&vertex_info.normal,
				DirectX::XMVector3Transform(
					DirectX::XMVectorSetW(
						DirectX::XMLoadFloat3(&origin_vertex_info.normal), 0.0f), rotation_matrix));

			// ź��Ʈ
			DirectX::XMStoreFloat4(&vertex_info.tangent,
				DirectX::XMVector4Transform(
					DirectX::XMVectorSetW(
						DirectX::XMLoadFloat4(&origin_vertex_info.tangent), 0.0f), rotation_matrix));
			vertex_info.tangent.w = origin_vertex_info.tangent.w;

			// uv
			vertex_info.uv = origin_vertex_info.uv;

			// ���͸��� �ε���
			vertex_info.material_index = origin_vertex_info.material_index;

			// ���Ϳ� �߰�
			vertices[i] = vertex_info;
		}

		// �ε��� �迭�� �°� �������� �����Ͽ� ���
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
	// y�� �ִ�� �ּҸ� ���
	float minimum_y_float = XMHelper::Min(
		XMHelper::Min(polygon_info.vertices[0].position.y, polygon_info.vertices[1].position.y),
		polygon_info.vertices[2].position.y);
	float maximum_y_float = XMHelper::Max(
		XMHelper::Max(polygon_info.vertices[0].position.y, polygon_info.vertices[1].position.y),
		polygon_info.vertices[2].position.y);

	// �� ���� ������ ����� �ʵ��� ����
	int minimum_y = XMHelper::Max(0, (int)std::ceil(minimum_y_float));
	int maximum_y = XMHelper::Min(m_buffer_height - 1, (int)std::floor(maximum_y_float));

	// ���� ������ ��� ������ ���� ����
	Vertex_Info vertex_info;

	// �ﰢ���� �׷����� y�� ���ؼ�
	for (int y = minimum_y; y <= maximum_y; ++y) {
		std::vector<Vertex_Info> vertex_vector;

		// �ﰢ���� �� ���� ��ȸ�ϸ�
		for (int i = 0; i < 3; ++i) {
			int j = (i + 1) % 3;

			// �ش� y�� �׷�����
			if ((polygon_info.vertices[i].position.y <= (float)y &&
				polygon_info.vertices[j].position.y > (float)y) ||
				(polygon_info.vertices[j].position.y <= (float)y &&
				polygon_info.vertices[i].position.y > (float)y)
			) {
				// �� ���� ����Ͽ� ���Ϳ� �߰� (���� ����� ó��)
				float t = (polygon_info.vertices[i].position.y == polygon_info.vertices[j].position.y) ? 0.5f :
					((float)y - polygon_info.vertices[i].position.y) /
					(polygon_info.vertices[j].position.y - polygon_info.vertices[i].position.y);

				// ���� ���� (����)
				vertex_info = Get_Interpolated_Vertex(polygon_info.vertices[i], polygon_info.vertices[j], t);
				vertex_info.position.y = (float)y;

				// ���Ϳ� �߰�
				vertex_vector.emplace_back(vertex_info);
			}
		}

		// ���� ���� 2�� �̸��̸� �н�
		if (vertex_vector.size() < 2) {
			continue;
		}

		// ���� ù ��° ���� x�� �� ũ�ٸ� ����
		if (vertex_vector[0].position.x > vertex_vector[1].position.x) {
			std::swap(vertex_vector[0], vertex_vector[1]);
		}

		// �׷��� x�� �ִ�� �ּҸ� ���
		int minimum_x = XMHelper::Max(0, (int)std::ceil(vertex_vector[0].position.x));
		int maximum_x = XMHelper::Min(m_buffer_width - 1, (int)std::floor(vertex_vector[1].position.x));

		// �׷����ϴ� ��� x�� ���ؼ�
		for (int x = minimum_x; x <= maximum_x; ++x) {
			float t = (vertex_vector[0].position.x == vertex_vector[1].position.x) ? 0.5f :
				((float)x - vertex_vector[0].position.x) /
				(vertex_vector[1].position.x - vertex_vector[0].position.x);

			// ���� ���� (����)
			vertex_info = Get_Interpolated_Vertex(vertex_vector[0], vertex_vector[1], t);
			vertex_info.position.x = (float)x;

			// �簢�� �׸���
			Render_Rect(vertex_info);
		}
	}
}

void RectRenderer::Render_Rect(Vertex_Info& vertex_info) {
	int x = (int)vertex_info.position.x;
	int y = (int)vertex_info.position.y;

	// �� ���� ���� üũ
	if (x < 0 || x >= m_buffer_width || y < 0 || y >= m_buffer_height) {
		return;
	}

	// ���� �� �ε��� ���
	int buffer_index = y * m_buffer_width + x;

	// ���� ��ȿ�� z���̶��
	if (vertex_info.position.z >= 0.0f &&
		vertex_info.position.z < m_z_buffer[buffer_index]
	) {
		// z buffer ���� ���� ����
		m_z_buffer[buffer_index] = vertex_info.position.z;

		Light_Info direction_light_info = ObjectManager::Get_Instance().Get_Light_Map()[0];
		float brightness = XMHelper::Max(0.1f, -XMHelper::Dot(vertex_info.normal, direction_light_info.direction));

		Material_Info* material_info = MaterialManager::Get_Instance().Get_Material_Info(vertex_info.material_index);
		int color_r = (int)(material_info->albedo.x * 255.0f * brightness * direction_light_info.strength.x);
		int color_g = (int)(material_info->albedo.y * 255.0f * brightness * direction_light_info.strength.y);
		int color_b = (int)(material_info->albedo.z * 255.0f * brightness * direction_light_info.strength.z);

		// ĥ�ؾ� �ϴ� �ȼ���ŭ ĥ�ϱ�
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
