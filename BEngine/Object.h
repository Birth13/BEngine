#pragma once
#include "common.h"

struct Mesh_Info;
struct Material_Info;

class Object {
private:
	std::vector<Mesh_Info*> m_meshes;
	std::vector<Material_Info*> m_materials;

	std::unordered_map<UINT, UINT> m_material_index_map;

	DirectX::XMFLOAT4 m_translation = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 m_rotation_quaternion = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 m_scaling = { 1.0f, 1.0f, 1.0f, 1.0f };

	DirectX::XMFLOAT4X4 m_world_matrix = XMHelper::Identity_XMF4x4();

	DirectX::XMFLOAT4 m_look = { 0.0f, 0.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT4 m_up = { 0.0f, 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 m_right = { 1.0f, 0.0f, 0.0f, 0.0f };

public:
	Object() {}
	Object(std::wstring mesh_name);
	Object(std::vector<std::wstring>& mesh_names);
	~Object() {}



	// 매쉬 추가 함수들

	// 매쉬를 추가
	void Add_Mesh(std::wstring& mesh_name);
	// 매쉬들을 추가
	void Add_Meshs(std::vector<std::wstring>& mesh_names);



	// Get 함수들

	// 이동값을 XMVECTOR로 반환
	DirectX::XMVECTOR Get_Translation_V();
	// 이동값을 XMFLOAT4로 반환
	DirectX::XMFLOAT4 Get_Translation_XMF4();

	// 회전값을 XMVECTOR로 반환
	DirectX::XMVECTOR Get_Rotation_Quaternion_V();
	// 회전값을 XMFLOAT4로 반환
	DirectX::XMFLOAT4 Get_Rotation_Quaternion_XMF4();

	// 확대값을 XMVECTOR로 반환
	DirectX::XMVECTOR Get_Scaling_V();
	// 확대값을 XMFLOAT4로 반환
	DirectX::XMFLOAT4 Get_Scaling_XMF4();

	// 월드 변환 행렬을 XMMATRIX로 반환
	DirectX::XMMATRIX Get_World_Matrix_M();
	// 월드 변환 행렬을 XMFLOAT4X4로 반환
	DirectX::XMFLOAT4X4 Get_World_Matrix_XMF4x4();

	// 룩 벡터를 XMFLOAT4로 반환
	DirectX::XMFLOAT4 Get_Look_XMF4();
	// 업 벡터를 XMFLOAT4로 반환
	DirectX::XMFLOAT4 Get_Up_XMF4();
	// 롸이트 벡터를 XMFLOAT4로 반환
	DirectX::XMFLOAT4 Get_Right_XMF4();



	// 업데이트 함수들

	// 오브젝트를 업데이트
	void Update(float elapsed_time);
	// 월드 변환 행렬을 업데이트
	void Update_World_Matrix();
	// 룩, 업, 롸이트 벡터를 업데이트
	void Update_Look_Up_Right();



	// 이동, 회전, 스케일 관련 함수들

	// 이동값을 설정
	void Set_Translation(float translation_x, float translation_y, float translation_z);
	// 회전값을 설정 (쿼터니언)
	void Set_Rotation(float rotation_x, float rotation_y, float rotation_z, float rotation_w);
	// 회전값을 설정 (각도)
	void Set_Rotation(float degree_x, float degree_y, float degree_z);
	// 확대값을 설정
	void Set_Scaling(float scaling_x, float scaling_y, float scaling_z);

	// 각도 만큼 오브젝트를 회전
	void Rotate(float degree_x, float degree_y, float degree_z);
};
