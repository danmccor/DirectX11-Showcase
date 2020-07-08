#include <d3d11.h>
#include <dxgi.h>

#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <wrl/client.h>
#include <iostream>
#pragma once

//Create a vertex shader class
class VertexShader
{
public:
	//Initialise the shader
	bool Initialise(ID3D11Device* device, std::string shaderPath, D3D11_INPUT_ELEMENT_DESC * desc, UINT numElements);
	//Return the shader
	ID3D11VertexShader* GetShader() {return shader;};
	//Return the shader buffer
	ID3D10Blob* GetBuffer() {return shaderBuffer;};
	//Get the input layout of the shader
	ID3D11InputLayout* GetInputLayout() { return inputLayout; };

private:
	//Initialise the shader variables
	ID3D11VertexShader* shader = nullptr;
	ID3D10Blob* shaderBuffer = nullptr;
	ID3D11InputLayout* inputLayout;
	LPCWSTR wShaderPath;

};

//Create a class for the pixel shader
class PixelShader
{
public:
	//initialise the shader
	bool Initialise(ID3D11Device* device, std::string shaderPath);
	//Get the shader
	ID3D11PixelShader* GetShader() { return shader; };
	//Get the shader buffer
	ID3D10Blob* GetBuffer() { return shaderBuffer; };
private:
	//Initialise the shader variables
	ID3D11PixelShader* shader = nullptr;
	ID3D10Blob* shaderBuffer = nullptr;
	LPCWSTR wShaderPath;
};

