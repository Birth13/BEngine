#pragma once
#include "common.h"

class Object;
class Camera;

class ObjectManager {
private:
	static std::unique_ptr<ObjectManager> object_manager;

	std::unordered_map<std::wstring, Object> m_object_map;
	std::unordered_map<std::wstring, Camera> m_camera_map;

public:
	ObjectManager() {}
	~ObjectManager() {}

	// �ν��Ͻ� ����
	static ObjectManager& Get_Instance();



	// Get �Լ���

	// ������Ʈ ���� ��ȯ
	std::unordered_map<std::wstring, Object>& Get_Object_Map();
	// ī�޶� ���� ��ȯ
	std::unordered_map<std::wstring, Camera>& Get_Camera_Map();

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
};

