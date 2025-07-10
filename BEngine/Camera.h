#pragma once
#include "common.h"
#include "Object.h"

class Camera : public Object {
private:
	float m_field_of_view_y;	// ���� �þ߰�
	float m_aspect_ratio;	// ȭ�� �� (width / height)

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



	// ������Ʈ �Լ���

	// ������Ʈ
	virtual void Update(float elapsed_time);
	// �� ��Ʈ���� ������Ʈ
	void Update_View_Matrix();



	// Get �Լ���

	float Get_Near_Z();
	float Get_Far_Z();

	// �� ��ȯ ����� XMMATRIX�� ��ȯ
	DirectX::XMMATRIX Get_View_Matrix_M();
	// �� ��ȯ ����� XMFLOAT4X4�� ��ȯ
	DirectX::XMFLOAT4X4 Get_View_Matrix_XMF4x4();

	// ���� ��ȯ ����� XMMATRIX�� ��ȯ
	DirectX::XMMATRIX Get_Projection_Matrix_M();
	// ���� ��ȯ ����� XMFLOAT4X4�� ��ȯ
	DirectX::XMFLOAT4X4 Get_Projection_Matrix_XMF4x4();


	// Set �Լ���

	// ����ü ����
	void Set_Frustum(float field_of_view_y, float aspect_ratio, float near_z, float far_z);
};

