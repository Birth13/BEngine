#pragma once
#include "common.h"
#include "Object.h"

class Camera : public Object {
private:
	float m_field_of_view_y;
	float m_aspect_ratio;

	float m_near_z;
	float m_far_z;

	float m_near_plane_height;
	float m_far_plane_height;

	DirectX::XMFLOAT4X4 m_view_matrix;
	DirectX::XMFLOAT4X4 m_projection_matrix;

public:
	Camera();
	Camera(float field_of_view_y, float aspect_ratio, float near_z, float far_z);
	virtual ~Camera() {}



	//
	virtual void Update(float elapsed_time);
	void Update_View_Matrix();

	void Set_Frustum(float field_of_view_y, float aspect_ratio, float near_z, float far_z);
};

