#include "Object.h"

#include "MeshManager.h"
#include "MaterialManager.h"

Object::Object(std::wstring mesh_name) {
	Add_Mesh(mesh_name);
}

Object::Object(std::vector<std::wstring>& mesh_names) {
	Add_Meshs(mesh_names);
}

void Object::Update(float elapsed_time) {
	Update_World_Matrix();
	Update_Look_Up_Right();
}

void Object::Update_World_Matrix() {
	// 이동 행렬, 회전 행렬, 확대 행렬 계산 후 월드 변환 행렬 계산
	DirectX::XMMATRIX translate_matrix = DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
	DirectX::XMMATRIX rotate_matrix = DirectX::XMMatrixRotationQuaternion(Get_Rotation_Quaternion_V());
	DirectX::XMMATRIX scale_matrix = DirectX::XMMatrixScaling(m_scaling.x, m_scaling.y, m_scaling.z);

	DirectX::XMStoreFloat4x4(&m_world_matrix, scale_matrix * rotate_matrix * translate_matrix);
}

void Object::Update_Look_Up_Right() {
	DirectX::XMMATRIX rotate_matrix = DirectX::XMMatrixRotationQuaternion(Get_Rotation_Quaternion_V());

	// w값 관련 문제 있을 가능성 있음
	DirectX::XMVECTOR m_look_vector = DirectX::XMVector3Normalize(DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotate_matrix));
	DirectX::XMVECTOR m_up_vector = DirectX::XMVector3Normalize(DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotate_matrix));

	// 룩, 업, 롸이트 벡터 저장
	DirectX::XMStoreFloat4(&m_look, m_look_vector);
	DirectX::XMStoreFloat4(&m_up, m_up_vector);
	DirectX::XMStoreFloat4(&m_right, DirectX::XMVector3Normalize(
		DirectX::XMVector3Cross(m_up_vector, m_look_vector)));
}

void Object::Add_Mesh(std::wstring& mesh_name) {
	Mesh_Info* mesh_info = MeshManager::Get_Instance().Get_Mesh_Info(mesh_name);

	for (UINT& i : mesh_info->used_material_indices) {
		// 사용하지 않던 머터리얼이면 새로 저장
		if (m_material_index_map.find(i) == m_material_index_map.end()) {
			m_materials.emplace_back(MaterialManager::Get_Instance().Get_Material_Info(i));

			m_material_index_map[i] = (UINT)m_material_index_map.size();
		}
	}

	m_meshes.emplace_back(mesh_info);
}

void Object::Add_Meshs(std::vector<std::wstring>& mesh_names) {
	for (auto& mesh_name : mesh_names) {
		Add_Mesh(mesh_name);
	}
}

std::vector<Mesh_Info*>& Object::Get_Meshes() {
	return m_meshes;
}

DirectX::XMVECTOR Object::Get_Translation_V() {
	return DirectX::XMLoadFloat4(&m_translation);
}

DirectX::XMFLOAT4 Object::Get_Translation_XMF4() {
	return m_translation;
}

DirectX::XMVECTOR Object::Get_Rotation_Quaternion_V() {
	return DirectX::XMLoadFloat4(&m_rotation_quaternion);
}

DirectX::XMFLOAT4 Object::Get_Rotation_Quaternion_XMF4() {
	return m_rotation_quaternion;
}

DirectX::XMVECTOR Object::Get_Scaling_V() {
	return DirectX::XMLoadFloat4(&m_scaling);
}

DirectX::XMFLOAT4 Object::Get_Scaling_XMF4() {
	return m_scaling;
}

DirectX::XMMATRIX Object::Get_World_Matrix_M() {
	return DirectX::XMLoadFloat4x4(&m_world_matrix);
}

DirectX::XMFLOAT4X4 Object::Get_World_Matrix_XMF4x4() {
	return m_world_matrix;
}

DirectX::XMFLOAT4 Object::Get_Look_XMF4() {
	return m_look;
}

DirectX::XMFLOAT4 Object::Get_Up_XMF4() {
	return m_up;
}

DirectX::XMFLOAT4 Object::Get_Right_XMF4() {
	return m_right;
}

void Object::Set_Translation(float translation_x, float translation_y, float translation_z) {
	m_translation = { translation_x, translation_y, translation_z, 0.0f };
}

void Object::Set_Rotation(float rotation_x, float rotation_y, float rotation_z, float rotation_w) {
	m_rotation_quaternion = { rotation_x, rotation_y, rotation_z, rotation_w };
}

void Object::Set_Rotation(float degree_x, float degree_y, float degree_z) {
	// 각도에 따라 쿼터니언 계산 후 저장
	DirectX::XMStoreFloat4(&m_rotation_quaternion, DirectX::XMQuaternionRotationRollPitchYaw(
		DirectX::XMConvertToRadians(degree_x),
		DirectX::XMConvertToRadians(degree_y),
		DirectX::XMConvertToRadians(degree_z)));
}

void Object::Set_Scaling(float scaling_x, float scaling_y, float scaling_z) {
	m_scaling = { scaling_x, scaling_y, scaling_z, 0.0f };
}

void Object::Rotate(float degree_x, float degree_y, float degree_z) {
	// 현재 쿼터니언에 추가 회전 쿼터니언을 곱
	DirectX::XMStoreFloat4(&m_rotation_quaternion, DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(
		Get_Rotation_Quaternion_V(), DirectX::XMQuaternionRotationRollPitchYaw(
			DirectX::XMConvertToRadians(degree_x),
			DirectX::XMConvertToRadians(degree_y),
			DirectX::XMConvertToRadians(degree_z)))));
}