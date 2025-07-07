#include "MaterialManager.h"

std::unique_ptr<MaterialManager> MaterialManager::material_manager = nullptr;

MaterialManager::MaterialManager() {
	// �⺻ ���͸��� ����	// ���͸��� �ε��� 0
	Create_Material();
}

MaterialManager& MaterialManager::Get_Instance() {
	if (material_manager == nullptr) {
		material_manager = std::make_unique<MaterialManager>();
	}

	return *material_manager;
}

Material_Info* MaterialManager::Get_Material_Info(std::wstring material_name) {
	return &m_material_info_map[material_name];
}

Material_Info* MaterialManager::Get_Material_Info(UINT material_index) {
	return m_material_info_pointer_map[material_index];
}

UINT MaterialManager::Get_Material_Index(std::wstring material_name) {
	// �ʿ� ������ �Ǿ������� �ε��� ����
	if (m_material_info_map.find(material_name) != m_material_info_map.end()) {
		return m_material_info_map[material_name].material_index;
	}

	// ������ UINT �ִ� ����
	return -1;
}

Material_Info* MaterialManager::Create_Material(std::wstring material_name, DirectX::XMFLOAT4 albedo) {
	// ���͸��� ���� ����
	m_material_info_map[material_name] = Material_Info(albedo);
	m_material_info_map[material_name].material_index = (UINT)m_material_info_map.size() - 1U;

	m_material_info_pointer_map.emplace_back(Get_Material_Info(material_name));

	return &m_material_info_map[material_name];
}
