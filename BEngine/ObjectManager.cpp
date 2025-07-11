#include "ObjectManager.h"

#include "Object.h"
#include "Camera.h"

std::unique_ptr<ObjectManager> ObjectManager::object_manager = nullptr;

ObjectManager::ObjectManager() {
	m_light_vector.emplace_back(Light_Info());
}

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

std::vector<Light_Info>& ObjectManager::Get_Light_Map() {
	return m_light_vector;
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

void ObjectManager::Add_Light(UINT light_type, float position_x, float position_y, float position_z,
	float direction_x, float direction_y, float direction_z,
	float strength_r, float strength_g, float strength_b
) {
	switch (light_type) {
	case DIRECTIONAL_LIGHT:
	{
		Light_Info light_info;
		light_info.position = { position_x, position_y, position_z };
		DirectX::XMStoreFloat3(&light_info.direction, DirectX::XMVector3Normalize(
			DirectX::XMVectorSet(direction_x, direction_y, direction_z, 0.0f)));
		light_info.strength = { strength_r, strength_g, strength_b };

		m_light_vector[0] = light_info;
	}
		break;
	default:
		break;
	}
}