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

	// 인스턴스 생성
	static ObjectManager& Get_Instance();



	//

	// 오브젝트를 추가 (단일 매쉬)
	Object& Add_Object(std::wstring object_name, std::wstring mesh_name);
	// 오브젝트를 추가 (다중 매쉬)
	Object& Add_Object(std::wstring object_name, std::vector<std::wstring> mesh_names);
};

