#pragma once
#include "Shaders.h"
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Camera.h"
#include "GameObject.h"
#include "cBufferTypes.h"
#include "Terrain.h"
#include "Skybox2.h"
#include "Light.h"
#include "Map2.h"


class Graphics
{
public: 
	//Initialise Graphics
	bool Initialise(HWND hWnd);
	//Render the frame
	void RenderFrame(std::vector<GameObject*> gameObjects, std::vector<std::string> strings, Camera& camera);
	//Getters for window heights
	int GetWindowWidth();
	int GetWindowHeight();

	//public constant buffer, device and device context
	ConstantBuffer<vs_cBuffer> vs_constantBuffer;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	//Set variables
	Terrain terrain;
	Light light;
	Skybox2 skybox;
	Map2 map;

private:
	//Private functions to be used
	bool InitialiseD3D(HWND hWnd);
	bool InitialiseShaders();
	bool InitialiseScene();
	void FindShaderFiles();
	
	//Create variables
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;

	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	//Create shaders
	VertexShader vertexShader;
	VertexShader vertexShader_skybox;
	PixelShader pixelShader;
	PixelShader pixelShader_noLight;
	PixelShader pixelShader_skybox;
	
	//Create pixel shader constant buffer
	ConstantBuffer<ps_cBuffer> ps_constantBuffer;
	
	//Variable for texture location
	std::string shaderFile;

	//Variables for states
	ID3D11DepthStencilView* dsView;
	ID3D11Texture2D* dsBuffer;
	ID3D11DepthStencilState* dsState;
	ID3D11RasterizerState* rasterizerState_cb;
	ID3D11RasterizerState* rasterizerState_cf;
	ID3D11BlendState* blendState;
	ID3D11SamplerState* samplerState; 

	//Text to screen variables
	DirectX::SpriteBatch* spriteBatch;
	DirectX::SpriteFont* spriteFont;

	//Variables for window widths and heights
	int windowWidth = 0;
	int windowHeight = 0;
};

