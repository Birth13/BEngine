#include "ObjectManager.h"

#include "Object.h"
#include "Camera.h"

std::unique_ptr<ObjectManager> ObjectManager::object_manager = nullptr;

ObjectManager& ObjectManager::Get_Instance() {
	if (object_manager == nullptr) {
		object_manager = std::make_unique<ObjectManager>();
	}

	return *object_manager;
}

std::unordered_map<std::wstring, Object>& ObjectManager::Get_Object_Map() {
	return m_object_map;
}

std::unordered_map<std::wstring, Camera>& ObjectManager::Get_Camera_Map() {
	return m_camera_map;
}

Object& ObjectManager::Get_Object(std::wstring object_name) {
	return m_object_map[object_name];
}

Camera& ObjectManager::Get_Camera(std::wstring camera_name) {
	return m_camera_map[camera_name];
}

void ObjectManager::Update(float elapsed_time) {
	for (auto& o : m_object_map) {
		o.second.Update(elapsed_time);
	}

	for (auto& c : m_camera_map) {
		c.second.Update(elapsed_time);
	}
}

Object& ObjectManager::Add_Object(std::wstring object_name, std::wstring mesh_name) {
	m_object_map[object_name] = Object(mesh_name);

	return m_object_map[object_name];
}

Object& ObjectManager::Add_Object(std::wstring object_name, std::vector<std::wstring> mesh_names) {
	m_object_map[object_name] = Object(mesh_names);

	return m_object_map[object_name];
}

Camera& ObjectManager::Add_Camera(std::wstring camera_name,
	float field_of_view_y, float aspect_ratio, float near_z, float far_z
) {
	m_camera_map[camera_name] = Camera(field_of_view_y, aspect_ratio, near_z, far_z);

	return m_camera_map[camera_name];
}