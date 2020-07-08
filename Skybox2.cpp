#include "Skybox2.h"

Skybox2::Skybox2()
{
	//Initialise variables
	device = nullptr;
	deviceContext = nullptr;
	srvSkyBox = nullptr;
	skyBox = nullptr;
	skyBoxTex = nullptr;
	rasterizerState = nullptr;
	CubesTexSamplerState = nullptr;

	dsView = nullptr;
	dsState = nullptr;
	NumSphereVertices = 0;
	NumSphereFaces = 0;

	rotationX = XMMatrixIdentity();
	rotationY = XMMatrixIdentity();
	rotationZ = XMMatrixIdentity();
	sphere = XMMatrixIdentity();
}

//Initialise the skybox
void Skybox2::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//Set the device and device context
	this->device = device;
	this->deviceContext = deviceContext;

	//Create the sphere
	createSphere(100, 100);

	//Create a texture
	ID3D11Texture2D* SMTexture;
	//Load the file from DDS, store in the texture as a resource, and the shader resource view
	DirectX::CreateDDSTextureFromFile(device, L"res/Urban.dds", (ID3D11Resource**)&SMTexture, &srvSkyBox);

	//Create texture Desc
	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	//Create resource desc
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	//Create the resource view
	device->CreateShaderResourceView(SMTexture, &SMViewDesc, &srvSkyBox);

	//Create rasterizer desc
	D3D11_RASTERIZER_DESC cmdesc;
	cmdesc.CullMode = D3D11_CULL_NONE;
	cmdesc.FillMode = D3D11_FILL_SOLID;
	//Create rasterizer
	device->CreateRasterizerState(&cmdesc, &rasterizerState);

	//Create Depth Stencil desc
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	//Create depth stencil
	device->CreateDepthStencilState(&dssDesc, &dsState);

	//Create sampler desc
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	//Create sampler
	device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);

	//Initialise the constant buffer
	vs_constantBuffer.Initialise(device, deviceContext);
}

void Skybox2::createSphere(int latLines, int longLines)
{
	//Creating the sphere
	/* 
	This algorith was taken from Braynzar Soft
	https://www.braynzarsoft.net/viewtutorial/q16390-20-cube-mapping-skybox
	*/

	NumSphereVertices = ((latLines - 2) * longLines) + 2;
	NumSphereFaces = ((latLines - 3) * (longLines) * 2) + (longLines * 2);

	auto sphereYaw = 0.0f;
	auto spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);

	DirectX::XMVECTOR currVertPos = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (float i = 0; i < (__int64)latLines - 2; i++) {
		spherePitch = (i + 1) * (3.14f / (latLines - 1));
		rotationX = DirectX::XMMatrixRotationX(spherePitch);
		for (float j = 0; j < (__int64)longLines; j++) {

			sphereYaw = j * (6.28f / (longLines));
			rotationY = DirectX::XMMatrixRotationZ(sphereYaw);
			currVertPos = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (rotationX * rotationY));
			currVertPos = DirectX::XMVector3Normalize(currVertPos);
			const __int64 y = (__int64)i * longLines + (__int64)j + 1;
			vertices[y].pos.x = DirectX::XMVectorGetX(currVertPos);
			vertices[y].pos.y = DirectX::XMVectorGetY(currVertPos);
			vertices[y].pos.z = DirectX::XMVectorGetZ(currVertPos);

		}
	}

	const __int64 y = (__int64)NumSphereVertices - (__int64)1.0f;
	vertices[y].pos.x = 0.0f;
	vertices[y].pos.y = 0.0f;
	vertices[y].pos.z = -1.0f;

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
	device->CreateBuffer(&vbDesc, &vertexBufferData, &skybox_vb);

	std::vector<DWORD> indices((__int64)NumSphereFaces * 3);
	auto k = 0;
	for (DWORD l = 0; l < (__int64)longLines - 1; l++)
	{
		indices[k] = 0;
		indices[(__int64)k + 1] = l + 1;
		indices[(__int64)k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[(__int64)k + 1] = longLines;
	indices[(__int64)k + 2] = 1;
	k += 3;


	for (unsigned int i = 0; i < (__int64)latLines - 3; i++)
	{
		for (unsigned int j = 0; j < (__int64)longLines - 1; j++)
		{
			indices[k] = i * longLines + j + 1;
			indices[(__int64)k + 1] = i * longLines + j + 2;
			indices[(__int64)k + 2] = (i + 1) * longLines + j + 1;

			indices[(__int64)k + 3] = (i + 1) * longLines + j + 1;
			indices[(__int64)k + 4] = i * longLines + j + 2;
			indices[(__int64)k + 5] = (i + 1) * longLines + j + 2;

			k += 6;
		}

		indices[k] = (i * longLines) + longLines;
		indices[(__int64)k + 1] = (i * longLines) + 1;
		indices[(__int64)k + 2] = ((i + 1) * longLines) + longLines;

		indices[(__int64)k + 3] = ((i + 1) * longLines) + longLines;
		indices[(__int64)k + 4] = (i * longLines) + 1;
		indices[(__int64)k + 5] = ((i + 1) * longLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < ((__int64)longLines - 1); l++)
	{
		indices[k] = NumSphereVertices - 1;
		indices[(__int64)k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[(__int64)k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = (__int64)NumSphereVertices - 1;
	indices[(__int64)k + 1] = ((__int64)NumSphereVertices - 1) - longLines;
	indices[(__int64)k + 2] = (__int64)NumSphereVertices - 2;

	

	//stackCount = latlines
	//sliceCount = longlines


	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;

	initData.pSysMem = &indices[0];
	device->CreateBuffer(&indexBufferDesc, &initData, &skybox_ib);
}

void Skybox2::Draw(Camera camera)
{
	//Set the sphere to an identity matrix
	sphere = XMMatrixIdentity();
	//Scale the sphere
	XMMATRIX Scale = XMMatrixScaling(1000.0f, 1000.0f, 1000.0f);
	//Translate the sphere
	XMMATRIX Translation = XMMatrixTranslation(XMVectorGetX(camera.GetPositionVector()), XMVectorGetY(camera.GetPositionVector()), XMVectorGetZ(camera.GetPositionVector()));
	//Create the transform matrix
	sphere = Scale * Translation;

	//Set constant buffer values
	vs_constantBuffer.data.wvpMat = sphere * camera.GetViewMatrix() * camera.GetProjectionMatrix();
	vs_constantBuffer.data.worldMat = sphere;
	//Apply the changes
	vs_constantBuffer.ApplyChanges();

	//Set the constant buffer
	deviceContext->VSSetConstantBuffers(0, 1, vs_constantBuffer.GetAddress());

	//Set offset and stride
	UINT offset = 0;
	UINT stride = sizeof(Vertex);
	//Set all states
	deviceContext->PSSetShaderResources(0, 1, &srvSkyBox);
	deviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);
	deviceContext->IASetVertexBuffers(0, 1, &skybox_vb, &stride, &offset);
	deviceContext->IASetIndexBuffer(skybox_ib, DXGI_FORMAT_R32_UINT, 0);
	//Draw the skybox
	deviceContext->DrawIndexed(NumSphereFaces * 3, 0, 0);
}


