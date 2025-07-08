#pragma once
#include "common.h"

class Object;

class ObjectManager {
private:
	static std::unique_ptr<ObjectManager> object_manager;

	std::unordered_map<std::wstring, Object> m_object_map;

public:
	ObjectManager() {}
	~ObjectManager() {}

	// �ν��Ͻ� ����
	static ObjectManager& Get_Instance();



	//

	// ������Ʈ�� �߰� (���� �Ž�)
	Object& Add_Object(std::wstring object_name, std::wstring mesh_name);
	// ������Ʈ�� �߰� (���� �Ž�)
	Object& Add_Object(std::wstring object_name, std::vector<std::wstring> mesh_names);
};

