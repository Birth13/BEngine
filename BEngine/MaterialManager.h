#pragma once
#include "common.h"

struct Material_Info {
	DirectX::XMFLOAT4 albedo;

	UINT material_index = -1;

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

	// 인스턴스 객체 반환
	static MaterialManager& Get_Instance();

	// 머터리얼

	// 머터리얼 이름과 일치하는 머터리얼 인포 포인터 반환
	Material_Info* Get_Material_Info(std::wstring material_name);
	// 머터리얼 인덱스 반환
	UINT Get_Material_Index(std::wstring material_name);

	// 머터리얼을 생성
	Material_Info* Create_Material(std::wstring material_name = L"default_material",
		DirectX::XMFLOAT4 albedo = { 1.0f, 1.0f, 1.0f, 1.0f });
};

