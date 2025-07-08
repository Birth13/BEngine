#include "ObjectManager.h"

#include "Object.h"

std::unique_ptr<ObjectManager> ObjectManager::object_manager = nullptr;

ObjectManager& ObjectManager::Get_Instance() {
	if (object_manager == nullptr) {
		object_manager = std::make_unique<ObjectManager>();
	}

	return *object_manager;
}

Object& ObjectManager::Add_Object(std::wstring object_name, std::wstring mesh_name) {
	m_object_map[object_name] = Object(mesh_name);

	return m_object_map[object_name];
}

Object& ObjectManager::Add_Object(std::wstring object_name, std::vector<std::wstring> mesh_names) {
	m_object_map[object_name] = Object(mesh_names);

	return m_object_map[object_name];
}
