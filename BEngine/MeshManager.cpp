#include "MeshManager.h"

std::unique_ptr<MeshManager> MeshManager::mesh_manager = nullptr;

MeshManager::MeshManager() {
	// �⺻ �Ž� ����
	Create_Box_Mesh();
}

MeshManager& MeshManager::Get_Instance() {
	if (mesh_manager == nullptr) {
		mesh_manager = std::make_unique<MeshManager>();
	}

	return *mesh_manager;
}

Mesh_Info& MeshManager::Create_Mesh(std::wstring mesh_name,
	std::vector<Vertex_Info> vertices_in, std::vector<std::uint32_t> indices_32_in
) {
	// �Ž� ���� ����
	m_mesh_info_map[mesh_name] = Mesh_Info(vertices_in, indices_32_in);

	return m_mesh_info_map[mesh_name];
}

Mesh_Info& MeshManager::Create_Box_Mesh(std::wstring mesh_name, float width) {
	// �ʺ��� ���� ���
	float half = width / 2.0f;

	// ���� �迭
	std::vector<Vertex_Info> vertices = {
		// �ո� (-z)
		{ { -half, -half, -half }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f } },
		{ { -half, +half, -half }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f } },
		{ { +half, +half, -half }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f } },
		{ { +half, -half, -half }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f } },

		// �޸� (+z)
		{ { +half, -half, +half }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f } },
		{ { +half, +half, +half }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f } },
		{ { -half, +half, +half }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f } },
		{ { -half, -half, +half }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f } },

		// ���� (+y)
		{ { -half, +half, -half }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f } },
		{ { -half, +half, +half }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f } },
		{ { +half, +half, +half }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f } },
		{ { +half, +half, -half }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f } },

		// �Ʒ��� (-y)
		{ { -half, -half, +half }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f } },
		{ { -half, -half, -half }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f } },
		{ { +half, -half, -half }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f } },
		{ { +half, -half, +half }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f } },

		// ������ (+x)
		{ { +half, -half, -half }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 0.0f, 1.0f } },
		{ { +half, +half, -half }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 0.0f, 0.0f } },
		{ { +half, +half, +half }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 1.0f, 0.0f } },
		{ { +half, -half, +half }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 1.0f, 1.0f } },

		// �޸� (+x)
		{ { -half, -half, +half }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 0.0f, 1.0f } },
		{ { -half, +half, +half }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 0.0f, 0.0f } },
		{ { -half, +half, -half }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 1.0f, 0.0f } },
		{ { -half, -half, -half }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 1.0f, 1.0f } }
	};

	// �ε��� �迭
	std::vector<std::uint32_t> indices_32 = {
		// �ո�
		0, 1, 2,
		0, 2, 3,

		// �޸�
		4, 5, 6,
		4, 6, 7,

		// ����
		8, 9, 10,
		8, 10, 11,

		// �Ʒ���
		12, 13, 14,
		12, 14, 15,

		// ������
		16, 17, 18,
		16, 18, 19,

		// �޸�
		20, 21, 22,
		20, 22, 23
	};

	// �Ž� ����
	return Create_Mesh(mesh_name, vertices, indices_32);
}

Mesh_Info& MeshManager::Get_Mesh_Info(std::wstring mesh_name) {
	return m_mesh_info_map[mesh_name];
}