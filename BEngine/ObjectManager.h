#pragma once
#include "common.h"

class Object;
class Camera;

class ObjectManager {
private:
	static std::unique_ptr<ObjectManager> object_manager;

	std::unordered_map<std::wstring, Object> m_object_map;
	std::unordered_map<std::wstring, Camera> m_camera_map;

public:
	ObjectManager() {}
	~ObjectManager() {}

	// 인스턴스 생성
	static ObjectManager& Get_Instance();



	// Get 함수들

	// 오브젝트 맵을 반환
	std::unordered_map<std::wstring, Object>& Get_Object_Map();
	// 카메라 맵을 반환
	std::unordered_map<std::wstring, Camera>& Get_Camera_Map();

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
};

