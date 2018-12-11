#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <Windows.h>
#include <gl\glu.h>
#include <string>

class Texture
{
public:
	Texture(char* _path, int width, int height);
	~Texture();

	bool Load(char* _path, int width, int height);
	bool LoadRAW(char* _path);
	bool LoadTextureTGA(const char* textureFileName);
	GLuint GetID() const { return _ID; }
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
	std::string GetPath() { return path; }

private:
	std::string path;
	GLuint _ID;
	int _width, _height;
};

#endif //_TEXTURE_H