#include "MaterialManager.h"

std::unique_ptr<MaterialManager> MaterialManager::material_manager = nullptr;

MaterialManager::MaterialManager() {
	Create_Material();
}

MaterialManager& MaterialManager::Get_Instance() {
	if (material_manager == nullptr) {
		material_manager = std::make_unique<MaterialManager>();
	}

	return *material_manager;
}

Material_Info& MaterialManager::Get_Material_Info(std::wstring material_name) {
	return m_material_info_map[material_name];
}

Material_Info& MaterialManager::Create_Material(std::wstring material_name, DirectX::XMFLOAT4 albedo) {
	m_material_info_map[material_name] = Material_Info(albedo);

	return m_material_info_map[material_name];
}
