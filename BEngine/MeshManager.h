#pragma once
#include "common.h"

struct Vertex_Info {
	DirectX::XMFLOAT3 position;	// ���� ��ġ (x, y, z)
	DirectX::XMFLOAT3 normal;	// �븻 ���� (x, y, z)
	DirectX::XMFLOAT4 tangent;	// ź��Ʈ ���� (x, y, z) // w : handedness
	DirectX::XMFLOAT2 uv;	// uv �� (x, y)

	UINT material_index = -1;	// ����ϴ� ���͸��� �ε���
};

struct Mesh_Info {
	std::vector<Vertex_Info> vertices;
	std::vector<std::uint16_t> indices_16;
	std::vector<std::uint32_t> indices_32;

	std::vector<UINT> used_material_indices;	// ���� ���͸��� �ε���

	//
	Mesh_Info() {};
	Mesh_Info(std::vector<Vertex_Info>& vertices_in, std::vector<std::uint32_t>& indices_32_in,
		std::vector<UINT>& used_material_indices_in) {
		// ���� ���� ä���
		vertices.assign(vertices_in.begin(), vertices_in.end());

		// �ε����� ������ UINT16_MAX ���� ������ 16����
		if (vertices.size() <= UINT16_MAX) {
			// ��������
			indices_16.resize(indices_32_in.size());

			// �ε���(16) ���� ä���
			for (size_t i = 0; i < indices_32_in.size(); ++i) {
				indices_16[i] = (std::uint16_t)indices_32_in[i];
			}
		}
		else {
			// �ε���(32) ���� ä���
			indices_32.assign(indices_32_in.begin(), indices_32_in.end());
		}

		// ���� ���͸��� �ε��� ä���
		used_material_indices.assign(used_material_indices_in.begin(), used_material_indices_in.end());
	}
};

class MeshManager {
private:
	static std::unique_ptr<MeshManager> mesh_manager;

	std::unordered_map<std::wstring, Mesh_Info> m_mesh_info_map;

public:
	MeshManager();
	~MeshManager() {}

	MeshManager(const MeshManager&) = delete;	// ���� ������ ����
	MeshManager& operator=(const MeshManager&) = delete;	// ���� ������ ����

	// �ν��Ͻ� ����

	// �ν��Ͻ� ��ü ��ȯ
	static MeshManager& Get_Instance();

	// �Ž� ����

	// �Ž� �̸��� ��ġ�ϴ� �Ž� ���� ������ ��ȯ
	Mesh_Info* Get_Mesh_Info(std::wstring mesh_name);

	// �Ž��� ����
	// ���� ������ ���͸��� �ε��� ������ ä���� �ִ� ���� ����
	Mesh_Info* Create_Mesh(std::wstring mesh_name,
		std::vector<Vertex_Info>& vertices_in, std::vector<std::uint32_t>& indices_32_in,
		std::vector<UINT>& used_material_indices_in);


	// �ڽ� �Ž��� ����
	// ������ ���� ���͸��� ����
	Mesh_Info* Create_Box_Mesh(float width = 1.0f,
		std::wstring mesh_name = L"default_box_mesh", std::wstring material_name = L"default_material");
};

