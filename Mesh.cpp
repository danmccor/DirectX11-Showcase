#include "Mesh.h"

//Constructor for meshes
Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture> & textures, const XMMATRIX& transformMat)
{
	this->deviceContext = deviceContext;
	this->textures = textures;
	this->transformMat = transformMat;
	this->vertices = vertices;
	HRESULT hr = vertexBuffer.Initialise(device, vertices.data(), vertices.size());

	hr = indexBuffer.Initialise(device, indices.data(), indices.size());
}
//Copy constructor for meshes
Mesh::Mesh(const Mesh& mesh)
{
	deviceContext = mesh.deviceContext;
	indexBuffer = mesh.indexBuffer;
	vertexBuffer = mesh.vertexBuffer;
	textures = mesh.textures;
	transformMat = mesh.transformMat;

}

//Draw the mesh
void Mesh::Draw()
{
	UINT offset = 0;
	//For each texture a mesh has
	for (int i = 0; i < textures.size(); i++) {
		if (textures[i].GetType() == aiTextureType::aiTextureType_DIFFUSE) {
			//Set the shader resource view to that texture
			deviceContext->PSSetShaderResources(0, 1, textures[i].GetTextureResourceViewAddress());
			break;
		}
	}
	//Draw the mesh
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddress(), vertexBuffer.StridePtr(), &offset);
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(indexBuffer.BufferSize(), 0, 0);
}
