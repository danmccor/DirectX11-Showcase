#include "Skybox.h"
bool Skybox::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->device = device;
	this->deviceContext = deviceContext;

	createSphere(10, 10);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = ARRAYSIZE(layout);
	//Skymap_vs.Initialise(device, "");
	
	
	DirectX::CreateDDSTextureFromFile(device, L"res/Texture3.dds", (ID3D11Resource**)&skyBox, &srvSkyBox);

	D3D11_TEXTURE2D_DESC skyboxTexDesc;
	skyBox->GetDesc(&skyboxTexDesc);

	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = skyboxTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = skyboxTexDesc.MipLevels;
	srvDesc.TextureCube.MostDetailedMip = 0;

	device->CreateShaderResourceView(skyBox, &srvDesc, &srvSkyBox);
	
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	cmdesc.FrontCounterClockwise = false;
	 device->CreateRasterizerState(&cmdesc, &RSCullNone);

	 D3D11_DEPTH_STENCIL_DESC dssDesc;
	 ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	 dssDesc.DepthEnable = true;
	 dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	 dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	 device->CreateDepthStencilState(&dssDesc, &DSLessEqual);


	return true;
}

void Skybox::Draw()
{
	

}

void Skybox::createSphere(int latLines, int longLines)
{
	NumSphereVertices = ((latLines - 2) * longLines) + 2;
	NumSphereFaces = ((latLines - 3) * (longLines * 2)) + (longLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);
	DirectX::XMVECTOR currVertPos = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (DWORD i = 0; i < latLines - 2; i++) {
		spherePitch = (i + 1) * (3.14 / (latLines - 1));
		rotationX - DirectX::XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < longLines; j++) {
			sphereYaw = j + (6.28 / longLines);
			rotationY = DirectX::XMMatrixRotationZ(sphereYaw);
			currVertPos = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (rotationX * rotationY));
			currVertPos = DirectX::XMVector3Normalize(currVertPos);
			vertices[i * longLines + j + 1].pos.x = DirectX::XMVectorGetX(currVertPos);
			vertices[i * longLines + j + 1].pos.y = DirectX::XMVectorGetY(currVertPos);
			vertices[i * longLines + j + 1].pos.z = DirectX::XMVectorGetZ(currVertPos);

		}
	}

	vertices[NumSphereVertices - 1].pos.x = 0.0f;
	vertices[NumSphereVertices - 1].pos.y = 0.0f;
	vertices[NumSphereVertices - 1].pos.z = -1.0f;

	D3D11_BUFFER_DESC vbDesc; 
	ZeroMemory(&vbDesc, sizeof(vbDesc));
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	device->CreateBuffer(&vbDesc, &vertexBufferData, &sphereVertexBuffer);

	std::vector<DWORD> indices(NumSphereFaces * 3);
	int k = 0;
	for (DWORD l = 0; l < longLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = longLines;
	indices[k + 2] = 1;
	k += 3;


	for (DWORD i = 0; i < latLines - 3; ++i)
	{
		for (DWORD j = 0; j < longLines - 1; ++j)
		{
			indices[k] = i * longLines + j + 1;
			indices[k + 1] = i * longLines + j + 2;
			indices[k + 2] = (i + 1) * longLines + j + 1;

			indices[k + 3] = (i + 1) * longLines + j + 1;
			indices[k + 4] = i * longLines + j + 2;
			indices[k + 5] = (i + 1) * longLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i * longLines) + longLines;
		indices[k + 1] = (i * longLines) + 1;
		indices[k + 2] = ((i + 1) * longLines) + longLines;

		indices[k + 3] = ((i + 1) * longLines) + longLines;
		indices[k + 4] = (i * longLines) + 1;
		indices[k + 5] = ((i + 1) * longLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < longLines - 1; ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - longLines;
	indices[k + 2] = NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;

	initData.pSysMem = &indices[0];
	device->CreateBuffer(&indexBufferDesc, &initData, &sphereIndexBuffer);
	
}
