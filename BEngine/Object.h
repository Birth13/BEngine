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



	// �Ž� �߰� �Լ���

	// �Ž��� �߰�
	void Add_Mesh(std::wstring& mesh_name);
	// �Ž����� �߰�
	void Add_Meshs(std::vector<std::wstring>& mesh_names);



	// Get �Լ���

	// �̵����� XMVECTOR�� ��ȯ
	DirectX::XMVECTOR Get_Translation_V();
	// �̵����� XMFLOAT4�� ��ȯ
	DirectX::XMFLOAT4 Get_Translation_XMF4();

	// ȸ������ XMVECTOR�� ��ȯ
	DirectX::XMVECTOR Get_Rotation_Quaternion_V();
	// ȸ������ XMFLOAT4�� ��ȯ
	DirectX::XMFLOAT4 Get_Rotation_Quaternion_XMF4();

	// Ȯ�밪�� XMVECTOR�� ��ȯ
	DirectX::XMVECTOR Get_Scaling_V();
	// Ȯ�밪�� XMFLOAT4�� ��ȯ
	DirectX::XMFLOAT4 Get_Scaling_XMF4();

	// ���� ��ȯ ����� XMMATRIX�� ��ȯ
	DirectX::XMMATRIX Get_World_Matrix_M();
	// ���� ��ȯ ����� XMFLOAT4X4�� ��ȯ
	DirectX::XMFLOAT4X4 Get_World_Matrix_XMF4x4();

	// �� ���͸� XMFLOAT4�� ��ȯ
	DirectX::XMFLOAT4 Get_Look_XMF4();
	// �� ���͸� XMFLOAT4�� ��ȯ
	DirectX::XMFLOAT4 Get_Up_XMF4();
	// ����Ʈ ���͸� XMFLOAT4�� ��ȯ
	DirectX::XMFLOAT4 Get_Right_XMF4();



	// ������Ʈ �Լ���

	// ������Ʈ�� ������Ʈ
	void Update(float elapsed_time);
	// ���� ��ȯ ����� ������Ʈ
	void Update_World_Matrix();
	// ��, ��, ����Ʈ ���͸� ������Ʈ
	void Update_Look_Up_Right();



	// �̵�, ȸ��, ������ ���� �Լ���

	// �̵����� ����
	void Set_Translation(float translation_x, float translation_y, float translation_z);
	// ȸ������ ���� (���ʹϾ�)
	void Set_Rotation(float rotation_x, float rotation_y, float rotation_z, float rotation_w);
	// ȸ������ ���� (����)
	void Set_Rotation(float degree_x, float degree_y, float degree_z);
	// Ȯ�밪�� ����
	void Set_Scaling(float scaling_x, float scaling_y, float scaling_z);

	// ���� ��ŭ ������Ʈ�� ȸ��
	void Rotate(float degree_x, float degree_y, float degree_z);
};
