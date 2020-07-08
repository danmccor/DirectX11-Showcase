#include "Model.h"

//Initialise the model
bool Model::Initialise(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<vs_cBuffer>& vs_ConstantBuffer)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->vs_ConstantBuffer = &vs_ConstantBuffer;
	this->filePath = filePath;

	//Load the model
	LoadModel(filePath);

	return true;
}

//Draw the model
void Model::Draw(const XMMATRIX& worldMat, const XMMATRIX& viewProjectionMatrix)
{
	//Set the constant buffer
	deviceContext->VSSetConstantBuffers(0, 1, vs_ConstantBuffer->GetAddress());
	//Loop through all meshes
	for (int i = 0; i < meshes.size(); i++) {
		//Set the constant buffer worldviewproject
		vs_ConstantBuffer->data.wvpMat = meshes[i].GetTransformMatrix() * worldMat * viewProjectionMatrix;
		//Set the constant buffers world mat
		vs_ConstantBuffer->data.worldMat = meshes[i].GetTransformMatrix() * worldMat;
		//Apply the changes
		vs_ConstantBuffer->ApplyChanges();
		//Draw
		meshes[i].Draw();
	}
}

//Create the models bounding box
void Model::CreateBoundingBox()
{
	//Create a cube from the minumum and maximum x, y and z axis
	boundingBox.push_back(XMFLOAT3(minimumPos.x, minimumPos.y, minimumPos.z));
	boundingBox.push_back(XMFLOAT3(minimumPos.x, maximumPos.y, minimumPos.z));
	boundingBox.push_back(XMFLOAT3(maximumPos.x, maximumPos.y, minimumPos.z));
	boundingBox.push_back(XMFLOAT3(maximumPos.x, minimumPos.y, minimumPos.z));

	boundingBox.push_back(XMFLOAT3(minimumPos.x, minimumPos.y, maximumPos.z));
	boundingBox.push_back(XMFLOAT3(maximumPos.x, minimumPos.y, maximumPos.z));
	boundingBox.push_back(XMFLOAT3(maximumPos.x, maximumPos.y, maximumPos.z));
	boundingBox.push_back(XMFLOAT3(minimumPos.x, maximumPos.y, maximumPos.z));
}



bool Model::LoadModel(const std::string& filePath)
{
	//Assimp importer
	Assimp::Importer importer;

	//Use the importer to load the scene from the file path
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (scene == NULL) return false;
	//Process the first node in the scene, pass in an identity matrix as nodes first transform
	ProcessNode(scene->mRootNode, scene, XMMatrixIdentity());
	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransform)
{
	//The node variable from assimp contains its transform in column major format. We can access this through its component 'a1' as it directly converts to a XMMATRIX.
	//We then transpose it to a row major format.
	//We multiply this by the parents transform matrix to keep the object in line.
	///This is necessary because each 'node' inside the model has its own local transform and if we put that directly into the world it will be out of place.
	XMMATRIX transformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransform;

	//Loop through meshes in the node
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		//Process the mesh in the node and push it back
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, transformMatrix));
	}
	//Search for children nodes
	for (UINT i = 0; i < node->mNumChildren; i++) {
		//Process the children nodes 
		ProcessNode(node->mChildren[i], scene, transformMatrix);
	}
	//After we finish processing all the nodes and their meshes, we will have the minumum and maximum boundaries for a bounding box
	CreateBoundingBox();
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix)
{
	//Initiate vertices and indices
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	//for the number of vertices in the mesh
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		//Set the x position of the vertex in the mesh
		vertex.pos.x = mesh->mVertices[i].x;
		//Check if its the minumum x position
		if (minimumPos.x == NULL || vertex.pos.x < minimumPos.x) {
			//If so, set it as minimum pos
			minimumPos.x = vertex.pos.x;
		}
		//Check if maximum
		if (maximumPos.x == NULL || vertex.pos.x > maximumPos.x) {
			//Set as maximum
			maximumPos.x = vertex.pos.x;
		}
		//Set the y position of the vertex in the mesh
		vertex.pos.y = mesh->mVertices[i].y;
		//Check if its the minimum
		if (minimumPos.y == NULL || vertex.pos.y < minimumPos.y) {
			//set it as the minimum
			minimumPos.y = vertex.pos.y;
		}
		//Check if its the maximum
		if (maximumPos.y == NULL || vertex.pos.y > maximumPos.y) {
			//Set it as the maximum
			maximumPos.y = vertex.pos.y;
		}
		//Set the z position of the vertices in the mesh
		vertex.pos.z = mesh->mVertices[i].z;
		//check if minimum
		if (minimumPos.z == NULL || vertex.pos.z < minimumPos.z) {
			//set as minimum
			minimumPos.z = vertex.pos.z;
		}
		//Check if maximum
		if (maximumPos.z == NULL || vertex.pos.z > maximumPos.z) {
			//set as maximum
			maximumPos.z = vertex.pos.z;
		}
		//If the mesh has normals
		if (mesh->HasNormals()) {
			//Set the Normals in the vertex
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		//Check if mesh has texture coordinates
		if (mesh->mTextureCoords[0]) {
			//Set texture coordinates
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}
		//Push back the vertex
		vertices.push_back(vertex);
		
	}
	//loop through faces
	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		//Set the face
		aiFace face = mesh->mFaces[i];
		//Add the number of indices from face
		numOfIndices += face.mNumIndices;
		//Loop through indices in face
		for (UINT j = 0; j < face.mNumIndices; j++) {
			//Push back the indices
			indices.push_back(face.mIndices[j]);
		}
	}
	//Create vector of texture
	std::vector<Texture> textures;
	//Get the material from the scene
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//Load the texture 
	std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
	//Inset the diffuse textures at the end of the texture vector
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	//Return the mesh
	return Mesh(device, deviceContext, vertices, indices, textures, transformMatrix);
}

TextureStorageType Model::DetermineTextureStorageType(const aiScene* scene, aiMaterial* material, unsigned int index, aiTextureType type)
{
	//If the model has no diffuse textures, return type none
	if (material->GetTextureCount(type) == 0) return TextureStorageType::None;

	//Create texture path
	aiString path;
	//Get the texture path
	material->GetTexture(type, index, &path);
	//Get the path as a string
	std::string texturePath = path.C_Str();

	//If texture path begins with *, it is an embedded index texture
	if (texturePath[0] == '*') {
		//If the height is 0
		if (scene->mTextures[0]->mHeight == 0) {
			//It is an embedded index compressed
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else {
			//It is an embedded index non compressed
			return TextureStorageType::EmbeddedIndexNonCompressed;
		}
	}
	//If scene->GetEmbeddedTexture returns something
	if (auto tex = scene->GetEmbeddedTexture(texturePath.c_str())) {
		//If height is 0
		if (tex->mHeight == 0) {
			//It is a Embedded compressed texture
			return TextureStorageType::EmbeddedCompressed;
		}
		else {
			//Else it is not compressed
			return TextureStorageType::EmbeddedNonCompressed;
		}
	}
	//If texture path has a '.' in it
	if (texturePath.find('.') != std::string::npos)
	{
		//It is a disk texture
		return TextureStorageType::Disk;
	}
	//Return the storage type
	return TextureStorageType();
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene)
{
	//Create a vector of material Textures
	std::vector<Texture> materialTextures;
	//Create the texture storage type and set it to invalid
	TextureStorageType storeType = TextureStorageType::Invalid;
	//Get the texture count
	unsigned int textureCount = material->GetTextureCount(type);

	//If the texture count is 0
   	if (textureCount == 0) {
		//The storage type is set to non
		storeType = TextureStorageType::None;
		//The colour is set to black
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);

		//Switch though the types of textures
		switch (type) {
			//If it is diffuse
		case aiTextureType_DIFFUSE:
		{
			//Get the material Colours
			material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			//If it is black
			if (aiColor.IsBlack()) {
				//Push back unhandled texture colour
				materialTextures.push_back(Texture(device, Colours::UnloadedTextureColour, type));
				//Return the texture vector
				return materialTextures;
			}
			//If it is not black, push back the texture with the colours
			materialTextures.push_back(Texture(device, Colour(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), type));
			//Return the material textures
			return materialTextures;
		}
		default:
		{
			//Push back unhandled texture colour as default
			materialTextures.push_back(Texture(device, Colours::UnhandledTextureColour, type));
			return materialTextures;
		}
		}
	}
	//If there are textures
	else {
		//Loop through textures
		for (UINT i = 0; i < textureCount; i++) {
			//Create path
			aiString path;
			//Get the texture
			material->GetTexture(type, i, &path);
			//Determine its storage type
			TextureStorageType storeType = DetermineTextureStorageType(scene, material, i, type);
			//Switch statement for storage type
			switch (storeType) {
				//If it is embedded compressed
				case TextureStorageType::EmbeddedCompressed:
				{
					//Get the embedded texture through the path
					const aiTexture* texture = scene->GetEmbeddedTexture(path.C_Str());
					//Create a texture
					Texture embeddedTexture(device, reinterpret_cast<uint8_t*>(texture->pcData), texture->mWidth, type);
					//Push back the texture
					materialTextures.push_back(embeddedTexture);
					break;
				}
				//If it is EmbeddedIndexCompressed
				case TextureStorageType::EmbeddedIndexCompressed:
				{
					//Get the texture index 
					int index = GetTextureIndex(&path);
					//Create a texture
					Texture embeddedTextureIndex(device, reinterpret_cast<uint8_t*>(scene->mTextures[index]->pcData), scene->mTextures[index]->mWidth, type);
					//Push back the texture
					materialTextures.push_back(embeddedTextureIndex);
					break;
				}
				//If the texture is a disk file
				case TextureStorageType::Disk:
				{
					//Set the path to be inside of "res/tex/"
					std::string name = "res/tex/";
					//Append the path to this string
					name.append(std::string(path.C_Str()));
					//Create a texture
					Texture diskTexture(device, name, type);
					//Push back the texture
					materialTextures.push_back(diskTexture);
					break;
				}
			}
		}
		//If the size is 0
		if (materialTextures.size() == 0){
			//Push back the unhandled texture 
			materialTextures.push_back(Texture(device, Colours::UnhandledTextureColour, type));
		}
		//Return textures
		return materialTextures;
	}
}

int Model::GetTextureIndex(aiString* string)
{
	//Return the index. There is always a '*' at the start of the string so we return the second character in the string, which will be the index
	return atoi(&string->C_Str()[1]);
}
