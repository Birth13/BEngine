#include "Camera.h"

Camera::Camera() {
	Set_Frustum(0.25f * DirectX::XM_PI, 16.0f / 9.0f, 1.0f, 1000.0f);
}

Camera::Camera(float field_of_view_y, float aspect_ratio, float near_z, float far_z) {
	Set_Frustum(field_of_view_y, aspect_ratio, near_z, far_z);
}

void Camera::Update(float elapsed_time) {
	Update_World_Matrix();
	Update_View_Matrix();
}

void Camera::Update_View_Matrix() {
	m_view_matrix = XMHelper::Calculate_View_Matrix(m_translation, m_look, m_up, m_right);
}

void Camera::Set_Frustum(float field_of_view_y, float aspect_ratio, float near_z, float far_z) {
	m_field_of_view_y = field_of_view_y;
	m_aspect_ratio = aspect_ratio;
	m_near_z = near_z;
	m_far_z = far_z;

	m_near_plane_height = 2.0f * m_near_z * tanf(0.5f * m_field_of_view_y);
	m_far_plane_height = 2.0f * m_far_z * tanf(0.5f * m_field_of_view_y);

	DirectX::XMStoreFloat4x4(&m_projection_matrix,
		DirectX::XMMatrixPerspectiveFovLH(m_field_of_view_y, m_aspect_ratio, m_near_z, m_far_z));
}
