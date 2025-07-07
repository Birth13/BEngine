#include "XMHelper.h"

DirectX::XMFLOAT4X4 XMHelper::Identity_XMF4x4() {
	static DirectX::XMFLOAT4X4 identity_xmf4x4{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
	};

	return identity_xmf4x4;
}
