#include "MeshManager.h"
#include "MaterialManager.h"

std::unique_ptr<MeshManager> MeshManager::mesh_manager = nullptr;

MeshManager::MeshManager() {
	// 기본 매쉬 생성
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
	// 매쉬 인포 저장
	m_mesh_info_map[mesh_name] = Mesh_Info(vertices_in, indices_32_in, used_material_indices_in);

	return &m_mesh_info_map[mesh_name];
}

Mesh_Info* MeshManager::Create_Box_Mesh(float width_x, float width_y, float width_z,
	std::wstring mesh_name, std::wstring material_name
) {
	// 머터리얼 인덱스 얻어오기
	UINT material_index = MaterialManager::Get_Instance().Get_Material_Index(material_name);

	// 없는 머터리얼이면 기본 머터리얼(인덱스 0) 사용
	if (material_index == ~0U) {	// ~0U == UINT_MAX
		material_index = 0U;
	}

	// 사용된 머터리얼 인덱스
	std::vector<UINT> used_material_indices;
	used_material_indices.emplace_back(material_index);

	// 너비의 절반 계산
	float half_x = width_x / 2.0f;
	float half_y = width_y / 2.0f;
	float half_z = width_z / 2.0f;

	// 정점 배열
	std::vector<Vertex_Info> vertices = {
		// 앞면 (-z)
		{ { -half_x, -half_y, -half_z }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { -half_x, +half_y, -half_z }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { +half_x, +half_y, -half_z }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { +half_x, -half_y, -half_z }, { 0.0f, 0.0f, -1.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f }, material_index },

		// 뒷면 (+z)
		{ { +half_x, -half_y, +half_z }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { +half_x, +half_y, +half_z }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { -half_x, +half_y, +half_z }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { -half_x, -half_y, +half_z }, { 0.0f, 0.0f, +1.0f }, { -1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f }, material_index },

		// 윗면 (+y)
		{ { -half_x, +half_y, -half_z }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { -half_x, +half_y, +half_z }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { +half_x, +half_y, +half_z }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { +half_x, +half_y, -half_z }, { 0.0f, +1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f }, material_index },

		// 아랫면 (-y)
		{ { -half_x, -half_y, +half_z }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { -half_x, -half_y, -half_z }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { +half_x, -half_y, -half_z }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { +half_x, -half_y, +half_z }, { 0.0f, -1.0f, 0.0f }, { +1.0f, 0.0f, 0.0f, +1.0f }, { 1.0f, 1.0f }, material_index },

		// 오른면 (+x)
		{ { +half_x, -half_y, -half_z }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { +half_x, +half_y, -half_z }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { +half_x, +half_y, +half_z }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { +half_x, -half_y, +half_z }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, +1.0f }, { 1.0f, 1.0f }, material_index },

		// 왼면 (+x)
		{ { -half_x, -half_y, +half_z }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 0.0f, 1.0f }, material_index },
		{ { -half_x, +half_y, +half_z }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 0.0f, 0.0f }, material_index },
		{ { -half_x, +half_y, -half_z }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 1.0f, 0.0f }, material_index },
		{ { -half_x, -half_y, -half_z }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, +1.0f, +1.0f }, { 1.0f, 1.0f }, material_index }
	};

	// 인덱스 배열
	std::vector<std::uint32_t> indices_32 = {
		// 앞면
		0, 1, 2,
		0, 2, 3,

		// 뒷면
		4, 5, 6,
		4, 6, 7,

		// 윗면
		8, 9, 10,
		8, 10, 11,

		// 아랫면
		12, 13, 14,
		12, 14, 15,

		// 오른면
		16, 17, 18,
		16, 18, 19,

		// 왼면
		20, 21, 22,
		20, 22, 23
	};

	// 매쉬 생성
	return Create_Mesh(mesh_name, vertices, indices_32, used_material_indices);
}