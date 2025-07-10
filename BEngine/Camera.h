#pragma once
#include "common.h"
#include "Object.h"

class Camera : public Object {
private:
	float m_field_of_view_y;	// 수직 시야각
	float m_aspect_ratio;	// 화면 비 (width / height)

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



	// 업데이트 함수들

	// 업데이트
	virtual void Update(float elapsed_time);
	// 뷰 매트릭스 업데이트
	void Update_View_Matrix();



	// Get 함수들

	float Get_Near_Z();
	float Get_Far_Z();

	// 뷰 변환 행렬을 XMMATRIX로 반환
	DirectX::XMMATRIX Get_View_Matrix_M();
	// 뷰 변환 행렬을 XMFLOAT4X4로 반환
	DirectX::XMFLOAT4X4 Get_View_Matrix_XMF4x4();

	// 투영 변환 행렬을 XMMATRIX로 반환
	DirectX::XMMATRIX Get_Projection_Matrix_M();
	// 투영 변환 행렬을 XMFLOAT4X4로 반환
	DirectX::XMFLOAT4X4 Get_Projection_Matrix_XMF4x4();


	// Set 함수들

	// 절두체 설정
	void Set_Frustum(float field_of_view_y, float aspect_ratio, float near_z, float far_z);
};

