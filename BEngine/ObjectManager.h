#pragma once
#include "common.h"

constexpr UINT DIRECTIONAL_LIGHT = 0U;

struct Light_Info {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 strength;
};

class Object;
class Camera;

class ObjectManager {
private:
	static std::unique_ptr<ObjectManager> object_manager;

	std::unordered_map<std::wstring, Object> m_object_map;
	std::unordered_map<std::wstring, Camera> m_camera_map;
	std::vector<Light_Info> m_light_vector;	// 0 : directional light

public:
	ObjectManager();
	~ObjectManager() {}

	// 인스턴스 생성
	static ObjectManager& Get_Instance();



	// Get 함수들

	// 오브젝트 맵을 반환
	std::unordered_map<std::wstring, Object>& Get_Object_Map();
	// 카메라 맵을 반환
	std::unordered_map<std::wstring, Camera>& Get_Camera_Map();
	// 라이트 벡터를 반환
	std::vector<Light_Info>& Get_Light_Map();

	// 이름과 일치하는 오브젝트를 반환
	Object& Get_Object(std::wstring object_name);
	// 이름과 일치하는 카메라를 반환
	Camera& Get_Camera(std::wstring camera_name);



	// 업데이트
	
	// 오브젝트들을 업데이트
	void Update(float elapsed_time);



	//

	// 오브젝트를 추가 (단일 매쉬)
	Object& Add_Object(std::wstring object_name, std::wstring mesh_name);
	// 오브젝트를 추가 (다중 매쉬)
	Object& Add_Object(std::wstring object_name, std::vector<std::wstring> mesh_names);

	// 카메라를 추가
	Camera& Add_Camera(std::wstring camera_name, float field_of_view_y, float aspect_ratio, float near_z, float far_z);

	// 라이트를 추가
	void Add_Light(UINT light_type, float position_x, float position_y, float position_z,
		float direction_x, float direction_y, float direction_z,
		float strength_r, float strength_g, float strength_b);
};

