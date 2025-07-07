#include "MeshManager.h"
#include "MaterialManager.h"

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

Mesh_Info* MeshManager::Get_Mesh_Info(std::wstring mesh_name) {
	return &m_mesh_info_map[mesh_name];
}

Mesh_Info* MeshManager::Create_Mesh(std::wstring mesh_name,
	std::vector<Vertex_Info>& vertices_in, std::vector<std::uint32_t>& indices_32_in,
	std::vector<UINT>& used_material_indices_in
) {
	// �Ž� ���� ����
	m_mesh_info_map[mesh_name] = Mesh_Info(vertices_in, indices_32_in, used_material_indices_in);

	return &m_mesh_info_map[mesh_name];
}

Mesh_Info* MeshManager::Create_Box_Mesh(float width_x, float width_y, float width_z,
	std::wstring mesh_name, std::wstring material_name
) {
	// ���͸��� �ε��� ������
	UINT material_index = MaterialManager::Get_Instance().Get_Material_Index(material_name);

	// ���� ���͸����̸� �⺻ ���͸���(�ε��� 0) ���
	if (material_index == ~0U) {	// ~0U == UINT_MAX
		material_index = 0U;
	}

	// ���� ���͸��� �ε���
	std::vector<UINT> used_material_indices;
	used_material_indices.emplace_back(material_index);

	// �ʺ��� ���� ���
	float half_x = width_x / 2.0f;
	float half_y = width_y / 2.0f;
	float half_z = width_z / 2.0f;

	// ���� �迭
	std::vector<Vertex_Info> vertices = {
		// �ո� (-z)
		{ { -half_x, -half_y, -half_z }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { -half_x, +half_y, -half_z }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { +half_x, +half_y, -half_z }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { +half_x, -half_y, -half_z }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f }, material_index },

		// �޸� (+z)
		{ { +half_x, -half_y, +half_z }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { +half_x, +half_y, +half_z }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { -half_x, +half_y, +half_z }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { -half_x, -half_y, +half_z }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f }, material_index },

		// ���� (+y)
		{ { -half_x, +half_y, -half_z }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { -half_x, +half_y, +half_z }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { +half_x, +half_y, +half_z }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { +half_x, +half_y, -half_z }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f }, material_index },

		// �Ʒ��� (-y)
		{ { -half_x, -half_y, +half_z }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { -half_x, -half_y, -half_z }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { +half_x, -half_y, -half_z }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { +half_x, -half_y, +half_z }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f }, material_index },

		// ������ (+x)
		{ { +half_x, -half_y, -half_z }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { +half_x, +half_y, -half_z }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { +half_x, +half_y, +half_z }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { +half_x, -half_y, +half_z }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 1.0f, 1.0f }, material_index },

		// �޸� (+x)
		{ { -half_x, -half_y, +half_z }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { -half_x, +half_y, +half_z }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { -half_x, +half_y, -half_z }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { -half_x, -half_y, -half_z }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 1.0f, 1.0f }, material_index }
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
	return Create_Mesh(mesh_name, vertices, indices_32, used_material_indices);
}