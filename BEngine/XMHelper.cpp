#include "XMHelper.h"

int XMHelper::Greatest_Common_Divisor(int number_a, int number_b) {
	int number_r;	// remainder

	while (number_b != 0) {
		number_r = number_a % number_b;
		number_a = number_b;
		number_b = number_r;
	}

	return number_a;
}

DirectX::XMFLOAT4X4 XMHelper::Identity_XMF4x4() {
	static DirectX::XMFLOAT4X4 identity_xmf4x4{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
	};

	return identity_xmf4x4;
}

DirectX::XMFLOAT2 XMHelper::Linear_Interpolation(const DirectX::XMFLOAT2& xmfloat3_a,
	const DirectX::XMFLOAT2& xmfloat3_b, const float& t
) {
	DirectX::XMFLOAT2 result;

	DirectX::XMStoreFloat2(&result, DirectX::XMVectorLerp(
		DirectX::XMLoadFloat2(&xmfloat3_a), DirectX::XMLoadFloat2(&xmfloat3_b), t));

	return result;
}

DirectX::XMFLOAT3 XMHelper::Linear_Interpolation(const DirectX::XMFLOAT3& xmfloat3_a,
	const DirectX::XMFLOAT3& xmfloat3_b, const float& t
) {
	DirectX::XMFLOAT3 result;

	DirectX::XMStoreFloat3(&result, DirectX::XMVectorLerp(
		DirectX::XMLoadFloat3(&xmfloat3_a), DirectX::XMLoadFloat3(&xmfloat3_b), t));

	return result;
}

DirectX::XMFLOAT4 XMHelper::Linear_Interpolation(const DirectX::XMFLOAT4& xmfloat3_a,
	const DirectX::XMFLOAT4& xmfloat3_b, const float& t
) {
	DirectX::XMFLOAT4 result;

	DirectX::XMStoreFloat4(&result, DirectX::XMVectorLerp(
		DirectX::XMLoadFloat4(&xmfloat3_a), DirectX::XMLoadFloat4(&xmfloat3_b), t));

	return result;
}

DirectX::XMFLOAT4X4 XMHelper::Calculate_View_Matrix(const DirectX::XMFLOAT4& position_in,
	DirectX::XMFLOAT4& look_in, DirectX::XMFLOAT4& up_in, DirectX::XMFLOAT4& right_in
) {
	DirectX::XMFLOAT4X4 view_matrix;

	DirectX::XMVECTOR position = DirectX::XMLoadFloat4(&position_in);
	DirectX::XMVECTOR look = DirectX::XMVector3Normalize(DirectX::XMLoadFloat4(&look_in));
	DirectX::XMVECTOR up = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(look, DirectX::XMLoadFloat4(&right_in)));
	DirectX::XMVECTOR right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(up, look));

	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, right));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, up));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, look));

	DirectX::XMStoreFloat4(&look_in, look);
	DirectX::XMStoreFloat4(&up_in, up);
	DirectX::XMStoreFloat4(&right_in, right);

	view_matrix(0, 0) = right_in.x;
	view_matrix(1, 0) = right_in.y;
	view_matrix(2, 0) = right_in.z;
	view_matrix(3, 0) = x;

	view_matrix(0, 1) = up_in.x;
	view_matrix(1, 1) = up_in.y;
	view_matrix(2, 1) = up_in.z;
	view_matrix(3, 1) = y;

	view_matrix(0, 2) = look_in.x;
	view_matrix(1, 2) = look_in.y;
	view_matrix(2, 2) = look_in.z;
	view_matrix(3, 2) = z;

	view_matrix(0, 3) = 0.0f;
	view_matrix(1, 3) = 0.0f;
	view_matrix(2, 3) = 0.0f;
	view_matrix(3, 3) = 1.0f;

	return view_matrix;
}

DirectX::XMFLOAT4X4 XMHelper::Calculate_View_Matrix(const DirectX::XMFLOAT4& position_in,
	const DirectX::XMFLOAT4& target_position_in,
	DirectX::XMFLOAT4& look_in, DirectX::XMFLOAT4& up_in, DirectX::XMFLOAT4& right_in
) {
	DirectX::XMFLOAT4X4 view_matrix;

	DirectX::XMVECTOR position = DirectX::XMLoadFloat4(&position_in);
	DirectX::XMVECTOR target_position = DirectX::XMLoadFloat4(&target_position_in);
	DirectX::XMVECTOR look = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(target_position, position));
	DirectX::XMVECTOR up = DirectX::XMVector3Normalize(DirectX::XMLoadFloat4(&up_in));
	DirectX::XMVECTOR right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(up, look));

	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, right));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, up));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(position, look));

	DirectX::XMStoreFloat4(&look_in, look);
	DirectX::XMStoreFloat4(&up_in, up);
	DirectX::XMStoreFloat4(&right_in, right);

	view_matrix(0, 0) = right_in.x;
	view_matrix(1, 0) = right_in.y;
	view_matrix(2, 0) = right_in.z;
	view_matrix(3, 0) = x;

	view_matrix(0, 1) = up_in.x;
	view_matrix(1, 1) = up_in.y;
	view_matrix(2, 1) = up_in.z;
	view_matrix(3, 1) = y;

	view_matrix(0, 2) = look_in.x;
	view_matrix(1, 2) = look_in.y;
	view_matrix(2, 2) = look_in.z;
	view_matrix(3, 2) = z;

	view_matrix(0, 3) = 0.0f;
	view_matrix(1, 3) = 0.0f;
	view_matrix(2, 3) = 0.0f;
	view_matrix(3, 3) = 1.0f;

	return view_matrix;
}

float XMHelper::Dot(const DirectX::XMFLOAT3& xmfloat3_a, const DirectX::XMFLOAT3& xmfloat3_b) {
	return DirectX::XMVectorGetX(DirectX::XMVector3Dot(
		DirectX::XMLoadFloat3(&xmfloat3_a), DirectX::XMLoadFloat3(&xmfloat3_b)));
}
