#ifndef _MESH_H
#define _MESH_H

#include <SDL.h>
#include "Commons.h"
#include <vector>
#include "Transform.h"
class Texture;

enum FILETYPE
{
	THREEDS,
	OBJ
};

using namespace std;

class Mesh
{
public:
	Mesh(char* path, char* texturePath = '\0');
	~Mesh();

	int GetListNumber() { return listNum; }
	int GetTris() { return tris_qty; }
	int GetQuads() { return quads_qty; }

	Transform offsets;
	string name = "mesh";
private:
	int vertices_qty;
	int normals_qty;
	int objFace_qty;
	int tris_qty;
	int quads_qty;
	Vector3D vertices[MAX_VERTICES];
	Triangle indices[MAX_POLYGONS];
	objFace objFaces[MAX_POLYGONS];
	TexCoord texCoords[MAX_VERTICES];
	Texture* threeDSTexture;

	string meshPath;
	FILETYPE fileType;

	vector<string*> coord;  
	vector<Vector3D*> vertex;     
	vector<objFace*> faces;    
	vector<Vector3D*> normals;  
	vector<unsigned int> texture;    
	vector<Material*> materials;  
	vector<TexCoord*> texturecoordinate;   
	bool ismaterial, isnormals, istexture; 
	bool usesMaterial;
	char meshName[20];

	bool usesUV;
	int listNum;
	bool listCompiled;

	bool LoadTexture(char* path, int width = 0, int height = 0);
	void CompileList();
	unsigned int LoadTextureObj(const char* filename);
	void Clean();
	bool Load(char* path, char* texturePath = '\0');
	bool LoadOBJ(char* fileName);
	bool Load3DS(char* fileName, char* texturePath = '\0');
	long FileLength(int f);

};

#endif