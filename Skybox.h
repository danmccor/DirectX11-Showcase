#pragma once
#include "Shaders.h"
#include <DDSTextureLoader.h>
#include <DirectXMath.h>
#include <vector>
#include "Vertex.h"
class Skybox
{
public: 
	bool Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Draw();

private:
	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertexBuffer;

	ID3D11VertexShader* Skymap_vs;
	ID3D11PixelShader* Skymap_ps;
	ID3D10Blob* Skymap_vs_buffer;
	ID3D10Blob* Skymap_ps_buffer;

	ID3D11ShaderResourceView* srvSkyBox;
	ID3D11Texture2D* skyBox;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	int NumSphereVertices;
	int NumSphereFaces;

	DirectX::XMMATRIX sphereWorld;

	void createSphere(int latLines, int longLines);
	
	DirectX::XMMATRIX rotationX;
	DirectX::XMMATRIX rotationY;
	DirectX::XMMATRIX rotationZ;
	
};
