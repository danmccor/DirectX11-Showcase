#include "Terrain.h"

bool Terrain::LoadHeightMap(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string fileName, float heightScale, ConstantBuffer<vs_cBuffer>& vs_ConstantBuffer)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->vs_ConstantBuffer = &vs_ConstantBuffer;
	this->heightScale = heightScale;

	//Create a file
	FILE* file;
	//Create the bitmap file headers
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	//Local variable for the image size
	__int64 imageSize;
	//Height set as int64 to prevent data loss
	__int64 height;
	//Open the file as readable
	fopen_s(&file, fileName.c_str(), "r");
	//If file is NULL, return false
	if (file == NULL) {
		return false;
	}
	//Read the files into the headers
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, file);
	//Set the width and length of the bitmap
	width = bmpInfo.biWidth;
	length = bmpInfo.biHeight;
	//Image size, width * length * number of bytes per pixel
	const __int64 y = (__int64)width * (__int64)length * (__int64)3;
	imageSize = y;
	//Bitmap image = the image size
	unsigned char* bmpImage = new unsigned char[imageSize];
	//Find the offbits
	fseek(file, bmpFile.bfOffBits, SEEK_SET);
	//Read the bitmap image
	fread(bmpImage, 1, imageSize, file);
	//Close the file
	fclose(file);
	//Resize vector of vector to match width and height
	for (int i = 0; i < length; i++) {
		std::vector<float> tmp;
		newHeights.push_back(tmp);
	}

	//Set K to 0
	int k = 0;
	//For j is less than length
	for (int j = 0; j < length; j++) {
		//for i is less than width
		for (int i = 0; i < width; i++) {
			//Create a vertex for storing positions
			Vertex vertex;
			//Height becomes the value at image[k]
			height = bmpImage[k];
			//Get positions, multiplied by the scale
			vertex.pos = XMFLOAT3(i * heightScale, height * heightScale, j * heightScale);
			//Get normals multiplied by scale
			vertex.normal.x = i * heightScale;
			vertex.normal.y = j* heightScale;
			vertex.normal.z = 0.0f;
			//Push back height into vertex
			newHeights[j].push_back(height * heightScale);

			//Create texture coordinates
			vertex.texCoord.x = (float)j / ((float)width - 1);
			vertex.texCoord.y = (float)i / ((float)length - 1);
			//Push back vertex
			vertices.push_back(vertex);
			//k + 3
			k += 3;
		}
	}
	//For j is less than length
	for (int j = 0; j < length; j++) {
		//I is less than with
		for (int i = 0; i < width; i++) {
			//Create indices
			int topleft = (j * width) + i;
			int topright = topleft + 1;
			int bottomleft = ((j + 1) * width) + i;
			int bottomright = bottomleft + 1;
			indices.push_back(topleft);
			indices.push_back(bottomleft);
			indices.push_back(topright);
			indices.push_back(topright);
			indices.push_back(bottomleft);
			indices.push_back(bottomright);
		}
	}
	//Push back a texture
	textures.push_back(Texture(device, Colour(255, 160.65f, 76.5f), aiTextureType::aiTextureType_DIFFUSE));
	//Push back the mesh
	mesh = new Mesh(device, deviceContext, vertices, indices, textures, XMMatrixIdentity());
	
	return true;
}

bool Terrain::Initialise()
{
	//Initialise terrain position and rotation
	SetPosition(-100.0f, 0.0f, -100.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	this->UpdateWorldMatrix();
	return true;
}

float Terrain::GetRelHeight(int x, int z)
{
	//Get the height of the pixel at coordinates
	float terrainX = (x / heightScale) - GetPositionFloat3().x;
	float terrainZ = (z / heightScale) - GetPositionFloat3().z;
	//Prevent vector going out of range
	if ((terrainX < width && terrainZ < length) && (terrainX > 0 && terrainZ > 0)) {
		return newHeights[(__int64)terrainZ][(__int64)terrainX];
	}
	else {
		float outOfBounds = -255;
		return outOfBounds;
	}
}

void Terrain::Draw(const XMMATRIX& viewProjectionMatrix)
{
	//Draw the terrain
	deviceContext->VSSetConstantBuffers(0, 1, vs_ConstantBuffer->GetAddress());
	vs_ConstantBuffer->data.wvpMat = mesh->GetTransformMatrix() * worldMat * viewProjectionMatrix;
	vs_ConstantBuffer->data.worldMat = mesh->GetTransformMatrix() * worldMat;

	vs_ConstantBuffer->ApplyChanges();
	mesh->Draw();
}
