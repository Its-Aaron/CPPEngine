#include "Texture.h"
#include <iostream>
#include <fstream>

using namespace::std;

Texture::Texture(char* _path, int width, int height)
{
	if (!Load(_path, width, height))
		cout << "Error loading texture: " << _path << endl;
}

Texture::~Texture()
{
	glDeleteTextures(1, &_ID);
}

bool Texture::Load(char* _path, int width, int height)
{
	path = _path;
	_width = width;
	_height = height;

	char c = ' ';

	char first, second, third = '\0';
	first = second = third;

	for (int i = 0; _path[i] != '\0'; i++)
	{
		if (first == '\0')
			first = _path[i];
		else if (second == '\0')
			second = _path[i];
		else if (third == '\0')
			third = _path[i];
		else
		{
			first = second;
			second = third;
			third = _path[i];
		}
	}

	if ((first == 't' || first == 'T') && (second == 'g' || second == 'G') && (third == 'a' || third == 'A'))
		LoadTextureTGA(_path);

	else if ((first == 'r' || first == 'R') && (second == 'a' || second == 'A') && (third == 'w' || third == 'W'))
		LoadRAW(_path);

	return true;
}

bool Texture::LoadRAW(char* _path)
{
	char* tempTextureData;
	int fileSize;
	ifstream inFile;
	inFile.open(_path, ios::binary);

	if (!inFile.good())
	{
		cerr << "Can't open texture file " << _path << endl;
		return false;
	}

	inFile.seekg(0, ios::end); //Seek to the end of the file
	fileSize = (int)inFile.tellg(); //Get current position in file (the end) for total file size
	tempTextureData = new char[fileSize]; //Create a new array to store data
	inFile.seekg(0, ios::beg); //Go back to the beginning
	inFile.read(tempTextureData, fileSize);
	inFile.close();

	glGenTextures(1, &_ID); //Get next texture id
	glBindTexture(GL_TEXTURE_2D, _ID); //Bind the textures to the id
	glTexImage2D(GL_TEXTURE_2D, 0, 3, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);

	delete[] tempTextureData; //Clear up the temporary data
	return true;
}

bool Texture::LoadTextureTGA(const char* textureFileName)
{
	char* tempHeaderData = new char[18]; //18 Bytes is TGA Header Size
	char* tempTextureData;
	int fileSize;
	char type, pixelDepth, mode;

	ifstream inFile;

	inFile.open(textureFileName, ios::binary);
	if (!inFile.good())
	{
		cerr << "Can't open texture file " << textureFileName << endl;
		return false;
	}

	//18 Bytes is the size of a TGA Header
	inFile.seekg(0, ios::beg); //Seek back to beginning of file
	inFile.read(tempHeaderData, 18); //Read in all the data in one go

	inFile.seekg(0, ios::end); //Seek to end of file
	fileSize = (int)inFile.tellg() - 18; //Get current position in file - The End, this gives us total file size
	tempTextureData = new char[fileSize]; //Create an new aray to store data
	inFile.seekg(18, ios::beg); //Seek back to beginning of file + 18
	inFile.read(tempTextureData, fileSize); //Read in all the data in one go
	inFile.close(); //Close the file

	type = tempHeaderData[2]; //Get TGA Type out of Header - Must be RGB for this to work
	_width = ((unsigned char)tempHeaderData[13] << 8u) + (unsigned char)tempHeaderData[12]; // Find the width (Combines two bytes into a short)
	_height = ((unsigned char)tempHeaderData[15] << 8u) + (unsigned char)tempHeaderData[14]; //Find the height
	pixelDepth = tempHeaderData[16]; // Find the pixel depth (24/32bpp)

	bool flipped = false;
	if ((int)((tempHeaderData[11] << 8) + tempHeaderData[10]) == 0)
		flipped = true;

	//We only support RGB type
	if (type == 2)
	{
		glGenTextures(1, &_ID); //Get next Texture ID
		glBindTexture(GL_TEXTURE_2D, _ID); //Bind the texture to the ID

		mode = pixelDepth / 8;

		//Note that TGA files are stored as BGR(A) - So we need to specify the format as GL_BGR(A)_EXT
		if (mode == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, tempTextureData);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, tempTextureData);
	}

	delete[] tempHeaderData; //We don't need the header memory anymore
	delete[] tempTextureData; //Clear up the data - We don't need this any more

	return true;
}