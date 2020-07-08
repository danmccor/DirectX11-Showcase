#pragma once
#include "cBufferTypes.h"
#include "Vertex.h"
#include "Index.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "include/assimp/Importer.hpp"
#include "include/assimp/postprocess.h"
#include "include/assimp/scene.h"

using namespace DirectX;

class Mesh
{
public:
	//Create the mesh file
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture> & textures, const XMMATRIX& transformMat);
	//Constructor for copying mesh
	Mesh(const Mesh& mesh);
	//Return the mesh tranform matrix
	const XMMATRIX& GetTransformMatrix() {return transformMat;};
	//Get the number of vertices in the mesh
	int GetNumberOfVertices() { return static_cast<int>(vertices.size()); };
	//Draw the mesh
	void Draw();
private: 
	//Set variables for the mesh
	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;
	ID3D11DeviceContext* deviceContext;
	std::vector<Texture> textures;
	XMMATRIX transformMat;
	std::vector<Vertex> vertices;
};

