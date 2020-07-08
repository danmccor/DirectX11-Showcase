#pragma once
#include <DirectXMath.h>

//These define the constant buffers for both the vertex shaders and the pixel shaders.
///There are only two here as all my vertex shaders and pixel shaders only use these.
struct vs_cBuffer {
	DirectX::XMMATRIX wvpMat; //16 Bytes
	DirectX::XMMATRIX worldMat; //16 Bytes
};

struct ps_cBuffer {
	DirectX::XMFLOAT3 direction; //12 bytes
	float PackingBytes; //4 bytes
	DirectX::XMFLOAT3 lightColor; //12 bytes
	float lightStrength; //4 bytes

	DirectX::XMFLOAT3 dynamicLightColor; //12 bytes
	float dynamicLightStrength; //4 bytes

	DirectX::XMFLOAT3 dynamicLightPosition; //12 bytes

	float lightAttenuation_a; //4 bytes
	float lightAttenuation_b; //4 bytes
	float lightAttenuation_c; //4 bytes

};
