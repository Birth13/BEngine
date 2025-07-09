#pragma once

#ifndef RC_INVOKED
#include <DirectXMath.h>
#endif

class XMHelper {
public:
	// ���� ���� ��ȯ
	template<typename T>
	static T Min(const T& number_a, const T& number_b) {
		return number_a < number_b ? number_a : number_b;
	}

	// ū ���� ��ȯ
	template<typename T>
	static T Max(const T& number_a, const T& number_b) {
		return number_a > number_b ? number_a : number_b;
	}

	// �ִ������� ��ȯ
	static int Greatest_Common_Divisor(int number_a, int number_b);

	// ���� ����� ��ȯ
	static DirectX::XMFLOAT4X4 Identity_XMF4x4();

	// ���������� ���� (XMFLOAT2)
	static DirectX::XMFLOAT2 Linear_Interpolation(const DirectX::XMFLOAT2& xmfloat3_a,
		const DirectX::XMFLOAT2& xmfloat3_b, const float& t);
	// ���������� ���� (XMFLOAT3)
	static DirectX::XMFLOAT3 Linear_Interpolation(const DirectX::XMFLOAT3& xmfloat3_a,
		const DirectX::XMFLOAT3& xmfloat3_b, const float& t);
	// ���������� ���� (XMFLOAT4)
	static DirectX::XMFLOAT4 Linear_Interpolation(const DirectX::XMFLOAT4& xmfloat3_a,
		const DirectX::XMFLOAT4& xmfloat3_b, const float& t);

	// �� ��ȯ ����� ��� (Ÿ�� X)
	static DirectX::XMFLOAT4X4 Calculate_View_Matrix(const DirectX::XMFLOAT4& position_in,
		DirectX::XMFLOAT4& look_in, DirectX::XMFLOAT4& up_in, DirectX::XMFLOAT4& right_in);
	// �� ��ȯ ����� ��� (Ÿ�� O)
	static DirectX::XMFLOAT4X4 Calculate_View_Matrix(const DirectX::XMFLOAT4& position_in,
		const DirectX::XMFLOAT4& target_position_in,
		DirectX::XMFLOAT4& look_in, DirectX::XMFLOAT4& up_in, DirectX::XMFLOAT4& right_in);
};