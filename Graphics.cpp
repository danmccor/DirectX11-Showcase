#include "Graphics.h"

//Initialise the graphics
bool Graphics::Initialise(HWND hWnd)
{
	//Initialise DirectX
	if (!InitialiseD3D(hWnd)) {
		return false;
	}
	//Initialise the Shaders
	if (!InitialiseShaders()) {
		return false;
	}
	//Initialise the Scene
	if (!InitialiseScene()) {
		return false;
	}
	return true;
}

//Render the frame
void Graphics::RenderFrame(std::vector<GameObject*> gameObjects, std::vector<std::string> strings, Camera& camera)
{
	//pixel shader constant buffers to take in the light source
	ps_constantBuffer.data.dynamicLightColor = light.lightColour;
	ps_constantBuffer.data.dynamicLightStrength = light.lightStrength;
	ps_constantBuffer.data.dynamicLightPosition = light.GetPositionFloat3();
	ps_constantBuffer.data.lightAttenuation_a = light.attenuation_a;
	ps_constantBuffer.data.lightAttenuation_b = light.attenuation_b;
	ps_constantBuffer.data.lightAttenuation_c = light.attenuation_c;

	//Apply changes to the constant buffer
	ps_constantBuffer.ApplyChanges();
	//Set the pixel constant buffer
	deviceContext->PSSetConstantBuffers(0, 1, ps_constantBuffer.GetAddress());
	//Set the vertex constant buffer
	deviceContext->VSSetConstantBuffers(0, 1, vs_constantBuffer.GetAddress());

	//Set background colour to black
	float rgba[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//Clear the render target view with the colour
	deviceContext->ClearRenderTargetView(renderTargetView, rgba);
	//Clear the depth stencil view
	deviceContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Set the input layout to the vertex shader
	deviceContext->IASetInputLayout(vertexShader.GetInputLayout());
	//Set the topology to triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//Set the rasterizerState
	deviceContext->RSSetState(rasterizerState_cb);
	//Set the depth stencil state
	deviceContext->OMSetDepthStencilState(dsState, 0);
	//Set the blend state
	deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	//Set the pixel shader sampler state
	deviceContext->PSSetSamplers(0, 1, &samplerState);
	//Set the vertex shader
	deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
	UINT offset = 0;
	for (int i = 0; i < gameObjects.size(); i++) {
		if (gameObjects[i]->GetIsLit()) {
			//Set Pixel shader for objects that use light
			deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);
			//Draw the object
			gameObjects[i]->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
		}
		else {
			////Set Pixel for objects not affected by light
			deviceContext->PSSetShader(pixelShader_noLight.GetShader(), NULL, 0);
			//Draw the object
			gameObjects[i]->Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
		}
	}
	//Set Pixel shader for objects that use light
	deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);
	//Draw the map
	map.Draw(camera);
	
	////Set Pixel for objects not affected by light
	deviceContext->PSSetShader(pixelShader_noLight.GetShader(), NULL, 0);
	light.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	terrain.Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	//Set pixel and vertex shader for the skybox
	deviceContext->VSSetShader(vertexShader_skybox.GetShader(), NULL, 0);
	deviceContext->PSSetShader(pixelShader_skybox.GetShader(), NULL, 0);
	//Draw the skybox
	skybox.Draw(camera);

	//Create a full string out of the strings passed in
	std::string fullString;
	std::wstring tmp;
	for (int i = 0; i < strings.size(); i++) {
		fullString.append(strings[i]);
		fullString.append("\n");
	}
	tmp = std::wstring(fullString.begin(), fullString.end());
	//Start casting to screen
	spriteBatch->Begin();
	//Draw on the screen
	spriteFont->DrawString(spriteBatch, tmp.c_str(), XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, XMFLOAT2(0,0), XMFLOAT2(1, 1));
	//Stop casting to screen
	spriteBatch->End();

	//Present all objects
	swapChain->Present(0, NULL);
}

//Get the window width
int Graphics::GetWindowWidth()
{
	return windowWidth;
}
//Get the window height
int Graphics::GetWindowHeight()
{
	return windowHeight;
}

//Initialise Direct X 
bool Graphics::InitialiseD3D(HWND hWnd)
{
	HRESULT hr = S_OK;
	//Get the window size without borders
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	//Store the window height
	windowWidth = width;
	windowHeight = height;

	UINT createDeviceFlags = 0;
	//Create driver types
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP, 
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	//Create feature levels
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	//Create the swap chain desc
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	//For driver types
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		//Create the device and the swapchain
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL,
			createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &swapChain,
			&device, &featureLevel, &deviceContext);
		//If Succeded, break out of loop
		if (SUCCEEDED(hr))
			break;
	}
	//If failed, return false
	if (FAILED(hr)) return false;
	
	//Create a 2D backbuffer
	ID3D11Texture2D* pBackBufferTexture;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),(LPVOID*)&pBackBufferTexture);
	//if failed return false
	if (FAILED(hr)) return false;

	// Use the back buffer texture pointer to create the render target view
	hr = device->CreateRenderTargetView(pBackBufferTexture, NULL, &renderTargetView);
	//release the backbuffer texture
	pBackBufferTexture->Release();
	//If failed return false
	if (FAILED(hr)) return false;

	//Create Texture Desc
	D3D11_TEXTURE2D_DESC dsDesc;
	dsDesc.Width = width;
	dsDesc.Height = height;
	dsDesc.MipLevels = 1;
	dsDesc.ArraySize = 1;
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsDesc.SampleDesc.Count = 1;
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.Usage = D3D11_USAGE_DEFAULT;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.MiscFlags = 0;

	//Create 2D texture
	hr = device->CreateTexture2D(&dsDesc, NULL, &dsBuffer);
	if (FAILED(hr))return false;
	//Create the depth stencil
	hr = device->CreateDepthStencilView(dsBuffer, NULL, &dsView);
	if (FAILED(hr))return false;
	// Set the render target view
	deviceContext->OMSetRenderTargets(1, &renderTargetView, dsView);

	//Create the depth stencil desc
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(dssDesc));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	//Create the deptch stencil state
	hr = device->CreateDepthStencilState(&dssDesc, &dsState);
	if (FAILED(hr))return false;

	// Set the viewport
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Set the viewport
	deviceContext->RSSetViewports(1, &viewport);

	//Create rasterizer desc
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	//Create the rasterizer state for culling back
	hr = device->CreateRasterizerState(&rasterDesc, &rasterizerState_cb);
	if (FAILED(hr))return false;
	rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
	//Create rasterizer state for culling front 
	hr = device->CreateRasterizerState(&rasterDesc, &rasterizerState_cf);
	if (FAILED(hr))return false;

	//Create the blend Desc
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	//Create target blend desc
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc;
	ZeroMemory(&rtBlendDesc, sizeof(rtBlendDesc));
	rtBlendDesc.BlendEnable = true;
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0] = rtBlendDesc;

	//Create the blend desc
	hr = device->CreateBlendState(&blendDesc, &blendState);
	if (FAILED(hr)) return false;

	//Create the sampler desc
	D3D11_SAMPLER_DESC sampleDesc;
	ZeroMemory(&sampleDesc, sizeof(sampleDesc));
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	//Create the sampler state
	hr = device->CreateSamplerState(&sampleDesc, &samplerState);
	if (FAILED(hr))return false;

	//Create a new sprite batch and font for rendering to screen
	spriteBatch = new SpriteBatch(deviceContext);
	spriteFont = new SpriteFont(device, L"res/Fonts/Arial11.spritefont");

	return true;
}

//Initialise the shaders to be used
bool Graphics::InitialiseShaders()
{
	//Initialise element layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numElements = ARRAYSIZE(layout);
	//Find the correct path for shader
	FindShaderFiles();
	//Initialise the vertex shader with the device, file location, the element layout and the number of elements
	if (!vertexShader.Initialise(device, shaderFile + "Vertex.cso", layout, numElements)) return false;
	if (!vertexShader_skybox.Initialise(device, shaderFile + "skybox_vs.cso", layout, numElements)) return false;
	//Initialise the pixel shader with the device and file location
	if (!pixelShader.Initialise(device, shaderFile + "Pixel.cso")) return false;
	if (!pixelShader_noLight.Initialise(device, shaderFile + "noLight_ps.cso")) return false;
	if (!pixelShader_skybox.Initialise(device, shaderFile + "skybox_ps.cso")) return false;
	return true;
}

//Initialise the Scene
bool Graphics::InitialiseScene()
{
	//Load the heightmap of the terrain
	terrain.LoadHeightMap(device, deviceContext, "res/terrain.bmp", 100, vs_constantBuffer);
	//Initialise the terrain
	terrain.Initialise();

	//Initialise the map
	map.Initialise(device, deviceContext, vs_constantBuffer);
	//Load the map
	map.LoadMap("res/MapTest2.bmp");

	//Create vertex constant buffer
	HRESULT hr = vs_constantBuffer.Initialise(device, deviceContext);
	if (FAILED(hr))return 0;

	//Create pixel constant buffer
	hr = ps_constantBuffer.Initialise(device, deviceContext);
	if (FAILED(hr))return 0;

	//Set the light direction, colour and strength
	ps_constantBuffer.data.direction = XMFLOAT3(0.25f, 0.5f, -1.0f);
	ps_constantBuffer.data.lightColor = XMFLOAT3(1.0f, 0.63f, 0.3f);
	ps_constantBuffer.data.lightStrength = 0.5f;

	//Initialise the light and skybox
	light.Initialise(device, deviceContext, vs_constantBuffer);
	skybox.Initialise(device, deviceContext);

	return true;
}

//Find the shader files
void Graphics::FindShaderFiles()
{
#pragma region DetermineShaderPath
	//Where direct X 11 does not support shaders in one fx file anymore, we must find the .cso files for both the vertex and pixel, the location changes depending on what platform and configuration is being used.
	if (IsDebuggerPresent())
	{
#ifdef _DEBUG
#ifdef _WIN64
		shaderFile = "x64/Debug/";
#else 
		shaderFile = "Debug/";
#endif 
#else 
#ifdef _WIN64
		shaderFile = "x64/Release/";
#else 
		shaderFile = "Release/";
#endif
#endif
	}
}
