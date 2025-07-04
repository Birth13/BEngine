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

	// �ν��Ͻ�

	// �ν��Ͻ� ��ü ��ȯ
	static MaterialManager& Get_Instance();

	// ���͸���

	// ���͸��� �̸��� ��ġ�ϴ� ���͸��� ���� ������ ��ȯ
	Material_Info* Get_Material_Info(std::wstring material_name);
	// ���͸��� �ε��� ��ȯ
	UINT Get_Material_Index(std::wstring material_name);

	// ���͸����� ����
	Material_Info* Create_Material(std::wstring material_name = L"default_material",
		DirectX::XMFLOAT4 albedo = { 1.0f, 1.0f, 1.0f, 1.0f });
};

