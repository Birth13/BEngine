#pragma once
#include "common.h"

constexpr UINT DIRECTIONAL_LIGHT = 0U;

struct Light_Info {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 strength;
};

class Object;
class Camera;

class ObjectManager {
private:
	static std::unique_ptr<ObjectManager> object_manager;

	std::unordered_map<std::wstring, Object> m_object_map;
	std::unordered_map<std::wstring, Camera> m_camera_map;
	std::vector<Light_Info> m_light_vector;	// 0 : directional light

public:
	ObjectManager();
	~ObjectManager() {}

	// �ν��Ͻ� ����
	static ObjectManager& Get_Instance();



	// Get �Լ���

	// ������Ʈ ���� ��ȯ
	std::unordered_map<std::wstring, Object>& Get_Object_Map();
	// ī�޶� ���� ��ȯ
	std::unordered_map<std::wstring, Camera>& Get_Camera_Map();
	// ����Ʈ ���͸� ��ȯ
	std::vector<Light_Info>& Get_Light_Map();

	// �̸��� ��ġ�ϴ� ������Ʈ�� ��ȯ
	Object& Get_Object(std::wstring object_name);
	// �̸��� ��ġ�ϴ� ī�޶� ��ȯ
	Camera& Get_Camera(std::wstring camera_name);



	// ������Ʈ
	
	// ������Ʈ���� ������Ʈ
	void Update(float elapsed_time);



	//

	// ������Ʈ�� �߰� (���� �Ž�)
	Object& Add_Object(std::wstring object_name, std::wstring mesh_name);
	// ������Ʈ�� �߰� (���� �Ž�)
	Object& Add_Object(std::wstring object_name, std::vector<std::wstring> mesh_names);

	// ī�޶� �߰�
	Camera& Add_Camera(std::wstring camera_name, float field_of_view_y, float aspect_ratio, float near_z, float far_z);

	// ����Ʈ�� �߰�
	void Add_Light(UINT light_type, float position_x, float position_y, float position_z,
		float direction_x, float direction_y, float direction_z,
		float strength_r, float strength_g, float strength_b);
};

