#pragma once
#include "common.h"

struct Vertex_Info {
	DirectX::XMFLOAT3 position;	// 정점 위치 (x, y, z)
	DirectX::XMFLOAT3 normal;	// 노말 벡터 (x, y, z)
	DirectX::XMFLOAT4 tangent;	// 탄젠트 벡터 (x, y, z) // w : handedness
	DirectX::XMFLOAT2 uv;	// uv 값 (x, y)

	UINT material_index = -1;	// 사용하는 머터리얼 인덱스
};

struct Mesh_Info {
	std::vector<Vertex_Info> vertices;
	std::vector<std::uint16_t> indices_16;
	std::vector<std::uint32_t> indices_32;

	std::vector<UINT> used_material_indices;	// 사용된 머터리얼 인덱스

	//
	Mesh_Info() {};
	Mesh_Info(std::vector<Vertex_Info>& vertices_in, std::vector<std::uint32_t>& indices_32_in,
		std::vector<UINT>& used_material_indices_in) {
		// 정점 정보 채우기
		vertices.assign(vertices_in.begin(), vertices_in.end());

		// 인덱스의 개수가 UINT16_MAX 보다 작으면 16으로
		if (vertices.size() <= UINT16_MAX) {
			// 리사이즈
			indices_16.resize(indices_32_in.size());

			// 인덱스(16) 정보 채우기
			for (size_t i = 0; i < indices_32_in.size(); ++i) {
				indices_16[i] = (std::uint16_t)indices_32_in[i];
			}
		}
		else {
			// 인덱스(32) 정보 채우기
			indices_32.assign(indices_32_in.begin(), indices_32_in.end());
		}

		// 사용된 머터리얼 인덱스 채우기
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

	MeshManager(const MeshManager&) = delete;	// 복사 생성자 삭제
	MeshManager& operator=(const MeshManager&) = delete;	// 대입 연산자 삭제

	// 인스턴스 관련

	// 인스턴스 객체 반환
	static MeshManager& Get_Instance();

	// 매쉬 관련

	// 매쉬 이름과 일치하는 매쉬 인포 포인터 반환
	Mesh_Info* Get_Mesh_Info(std::wstring mesh_name);

	// 매쉬를 생성
	// 정점 정보에 머터리얼 인덱스 정보가 채워져 있는 것을 전제
	Mesh_Info* Create_Mesh(std::wstring mesh_name,
		std::vector<Vertex_Info>& vertices_in, std::vector<std::uint32_t>& indices_32_in,
		std::vector<UINT>& used_material_indices_in);


	// 박스 매쉬를 생성
	// 무조건 단일 머터리얼 생성
	Mesh_Info* Create_Box_Mesh(float width = 1.0f,
		std::wstring mesh_name = L"default_box_mesh", std::wstring material_name = L"default_material");
};

