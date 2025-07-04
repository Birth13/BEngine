#pragma once
#include "common.h"

struct Material_Info {
	DirectX::XMFLOAT4 albedo;

	//
	Material_Info() {}
	Material_Info(DirectX::XMFLOAT4 albedo_in) {
		albedo = albedo_in;
	}
};

class MaterialManager {
private:
	static std::unique_ptr<MaterialManager> material_manager;

	std::unordered_map<std::wstring, Material_Info> m_material_info_map;

public:
	MaterialManager();
	~MaterialManager() {}

	// 인스턴스

	static MaterialManager& Get_Instance();

	// 머터리얼

	Material_Info& Get_Material_Info(std::wstring material_name);

	Material_Info& Create_Material(std::wstring material_name = L"default_material",
		DirectX::XMFLOAT4 albedo = { 1.0f, 1.0f, 1.0f, 1.0f });
};

