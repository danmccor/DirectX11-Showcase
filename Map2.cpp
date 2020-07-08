#include "Map2.h"

//Initialise the map
bool Map2::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<vs_cBuffer>& vs_constantBuffer) {
	//Set variables
	this->device = device;
	this->deviceContext = deviceContext;
	this->vs_constantBuffer = &vs_constantBuffer;
	return true;
}

bool Map2::LoadMap(std::string fileName)
{
	//Create a file
	FILE* file;
	//Create the bitmap file headers
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	//Local variable for the image size
	int imageSize;
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

	//Set variables for loading onto screen
	float widthGap = 17.5f;
	float lengthGap = 28.5f;
	float scale = 10.0f;
	float BuildingExtraHeight = 56.51f;

	//Image size, width * length * number of bytes per pixel
	imageSize = width * length * 4;
	//Bitmap image = the image size
	unsigned char* bmpImage = new unsigned char[imageSize];
	//Find the offbits
	fseek(file, bmpFile.bfOffBits, SEEK_SET);
	//Read the bitmap image
	fread(bmpImage, 1, imageSize, file);
	//Close the file
	fclose(file);
	
	//Set K to 0
	int k = 0;
	//For j is less than length
	for (int j = 0; j < length; j++) {
		//for i is less than width
		for (int i = 0; i < width; i++) {
			//Height becomes the value at image[k]
			height = (__int64)bmpImage[(__int64)k];

			//Switch case for the heights, depending on what the height is, spawn a different building
			switch (height) {
				case 200:
				{
					//Create the gameobject
					GameObject gameObject;
					//Initialise the game object
					gameObject.Initialise("res/ResidentialBuilding10.obj", device, deviceContext, *vs_constantBuffer, true);
					//Set its position and scale
					gameObject.SetPosition(i * widthGap, 0, j * lengthGap);
					gameObject.SetScale(scale, scale, scale);
					//Create its collision box, taking some off the top of the y
					gameObject.CreateAABBCC(0, -BuildingExtraHeight, 0);
					//Push back the object
					gameObjects.push_back(gameObject);
					//leave switch statement
					break;
				}
				case 190:
				{
					//Create the gameobject
					GameObject gameObject;
					//Initialise the game object
					gameObject.Initialise("res/ResidentialBuilding9.obj", device, deviceContext, *vs_constantBuffer, true);
					//Set its position and scale
					gameObject.SetPosition(i * widthGap, 0, j * lengthGap);
					gameObject.SetScale(scale, scale, scale);
					//Create its collision box, taking some off the top of the y
					gameObject.CreateAABBCC(0, -BuildingExtraHeight, 0);
					//Push back the object
					gameObjects.push_back(gameObject);
					//leave switch statement
					break;
				}
				case 180:
				{
					//Create the gameobject
					GameObject gameObject;
					//Initialise the game object
					gameObject.Initialise("res/ResidentialBuilding8.obj", device, deviceContext, *vs_constantBuffer, true);
					//Set its position and scale
					gameObject.SetPosition(i * widthGap, 0, j * lengthGap);
					gameObject.SetScale(scale, scale, scale);
					//Create its collision box, taking some off the top of the y
					gameObject.CreateAABBCC(0, -BuildingExtraHeight, 0);
					//Push back the object
					gameObjects.push_back(gameObject);
					//leave switch statement
					break;
				}
				case 170:
				{
					//Create the gameobject
					GameObject gameObject;
					//Initialise the game object
					gameObject.Initialise("res/ResidentialBuilding7.obj", device, deviceContext, *vs_constantBuffer, true);
					//Set its position and scale
					gameObject.SetPosition(i * widthGap, 0, j * lengthGap);
					gameObject.SetScale(scale, scale, scale);
					//Create its collision box, taking some off the top of the y
					gameObject.CreateAABBCC(0, -BuildingExtraHeight, 0);
					//Push back the object
					gameObjects.push_back(gameObject);
					//leave switch statement
					break;
				}
				case 160:
				{
					//Create the gameobject
					GameObject gameObject;
					//Initialise the game object
					gameObject.Initialise("res/ResidentialBuilding6.obj", device, deviceContext, *vs_constantBuffer, true);
					//Set its position and scale
					gameObject.SetPosition(i * widthGap, 0, j * lengthGap);
					gameObject.SetScale(scale, scale, scale);
					//Create its collision box, taking some off the top of the y
					gameObject.CreateAABBCC(0, -BuildingExtraHeight, 0);
					//Push back the object
					gameObjects.push_back(gameObject);
					//leave switch statement
					break;
				}
				case 150:
				{
					//Create the gameobject
					GameObject gameObject;
					//Initialise the game object
					gameObject.Initialise("res/ResidentialBuilding5.obj", device, deviceContext, *vs_constantBuffer, true);
					//Set its position and scale
					gameObject.SetPosition(i * widthGap, 0, j * lengthGap);
					gameObject.SetScale(scale, scale, scale);
					//Create its collision box, taking some off the top of the y
					gameObject.CreateAABBCC(0, -BuildingExtraHeight, 0);
					//Push back the object
					gameObjects.push_back(gameObject);
					//leave switch statement
					break;
				}
				//Default as building object 7
				default:
				{
					//Create the gameobject
					GameObject gameObject;
					//Initialise the game object
					gameObject.Initialise("res/ResidentialBuilding7.obj", device, deviceContext, *vs_constantBuffer, true);
					//Set its position and scale
					gameObject.SetPosition(i * widthGap, 0, j * lengthGap);
					gameObject.SetScale(scale, scale, scale);
					//Create its collision box, taking some off the top of the y
					gameObject.CreateAABBCC(0, -BuildingExtraHeight, 0);
					//Push back the object
					gameObjects.push_back(gameObject);
					//leave switch statement
					break;
				}
			
			}
			//k + 4 as there are 4 bytes per pixel
			k += 4;
		}
	}
	return true;
}
//Draw the buildings
void Map2::Draw(Camera camera)
{
	//For each Building created, draw it.
	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i].Draw(camera.GetViewMatrix() * camera.GetProjectionMatrix());
	}
}
