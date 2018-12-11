#include "Mesh.h"
#include <iostream>
#include <fstream>
#include "Texture.h"
#include <vector>
#ifndef GL_UNSIGNED_SHORT_5_6_5
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define D_SCL_SECURE_NO_WARNINGS
#endif

Mesh::Mesh(char* path, char* texturePath)
{
	string pathAsString(path);
	int i = pathAsString.find_first_of('.');
	name = to_string(pathAsString.copy(path, 0, i));

	ismaterial = false;
	isnormals = false;
	istexture = false;

	usesUV = false;
	usesMaterial = false;
	listCompiled = false;
	offsets = Transform();
	threeDSTexture = nullptr;
	tris_qty = 0;
	quads_qty = 0;

	meshPath = path;
	
	if (!Load(path, texturePath))
		cout << "Failed to load model " << path << endl;
}


Mesh::~Mesh()
{}

bool Mesh::Load(char* path, char* texturePath)
{
	bool success = false;
	string pathAsString(path);
	int pl = pathAsString.length();

	if ( (path[pl - 3] == 'o' || path[pl - 3] == 'O') && (path[pl - 2] == 'b' || path[pl - 2] == 'B') && (path[pl - 1] == 'j' || path[pl - 1] == 'J'))
		success = LoadOBJ(path);

	else if ((char)path[pl - 3] == '3' && (path[pl - 2] == 'd' || path[pl - 2] == 'D') && (path[pl - 1] == 's' || path[pl - 1] == 'S') )
		success = Mesh::Load3DS(path, texturePath);

	return success;
}

long Mesh::FileLength(int f)
{
	struct stat buf;
	fstat(f, &buf);
	return(buf.st_size);
}

bool Mesh::LoadOBJ(char* fileName)
{
	std::ifstream in(fileName);     //open the model file
	if (!in.is_open())
	{
		std::cout << "Not opened" << std::endl; //if it's not opened then error message, and return with -1
		return false;
	}


	fileType = OBJ;

	char buf[256];  //temp buffer
	int curmat = 0;   //the current (default) material is 0, it's used, when we read the faces
	while (!in.eof())
	{
		in.getline(buf, 256);    //while we are not in the end of the file, read everything as a string to the coord vector
		coord.push_back(new std::string(buf));
	}
	for (unsigned int i = 0;i<coord.size();i++) //and then go through all line and decide what kind of line it is
	{
		if ((*coord[i])[0] == '#') //if it's a comment
			continue;       //we don't have to do anything with it
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == ' ')     //if a vertex
		{
			float tmpx, tmpy, tmpz;
			sscanf_s(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);       //read the 3 floats, which makes up the vertex
			vertex.push_back(new Vector3D(tmpx, tmpy, tmpz));       //and put it in the vertex vector
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n')    //if it's a normal vector
		{
			float tmpx, tmpy, tmpz;
			sscanf_s(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			normals.push_back(new Vector3D(tmpx, tmpy, tmpz));      //basically do the same
			isnormals = true;
		}
		else if ((*coord[i])[0] == 'f')   //if it's a face
		{
			int a, b, c, d, e;
			if (count(coord[i]->begin(), coord[i]->end() - 1, ' ') == 4)     //if this is a quad
			{
				if (coord[i]->find("//") != std::string::npos)     //if it contains a normal vector, but not contain texture coordinate
				{
					sscanf_s(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b, &e, &b);
					faces.push_back(new objFace(b, a, c, d, e, 0, 0, 0, 0, curmat));
				}
				else if (count(coord[i]->begin(), coord[i]->end(), '/') == 4)        //if we have texture coordinate but no normal vectors
				{
					int t[4];
					sscanf_s(coord[i]->c_str(), "f %d/%d %d/%d %d/%d %d/%d", &a, &t[0], &b, &t[1], &c, &t[2], &d, &t[3]);
					faces.push_back(new objFace(b, a, b, c, d, t[0], t[1], t[2], t[3], curmat));
				}
				else if (coord[i]->find("/") != std::string::npos)        //if we have texture coordinate and normal vectors
				{
					int t[4];       //texture coordinates
									//read in this form, and put to the end of the vector
					sscanf_s(coord[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &a, &t[0], &b, &c, &t[1], &b, &d, &t[2], &b, &e, &t[3], &b);
					faces.push_back(new objFace(b, a, c, d, e, t[0], t[1], t[2], t[3], curmat));
				}
				else {
					//else we don't have normal vectors nor texture coordinate
					sscanf_s(coord[i]->c_str(), "f %d %d %d %d", &a, &b, &c, &d);
					faces.push_back(new objFace(-1, a, b, c, d, 0, 0, 0, 0, curmat));
				}
				quads_qty++;
			}
			else 
			{  //if it's a triangle
					//do the same, except we use one less vertex/texture coorinate/face number
				if (coord[i]->find("//") != std::string::npos)
				{
					sscanf_s(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b);
					faces.push_back(new objFace(b, a, c, d, 0, 0, 0, curmat));
				}
				else if (count(coord[i]->begin(), coord[i]->end(), '/') == 3)        //if we have texture coordinate but no normal vectors
				{
					int t[4];
					sscanf_s(coord[i]->c_str(), "f %d/%d %d/%d %d/%d", &a, &t[0], &b, &t[1], &c, &t[2]);
					faces.push_back(new objFace(b, a, b, c, t[0], t[1], t[2], curmat));
				}
				else if (count(coord[i]->begin(), coord[i]->end(), '/') == 6)
				{
					int t[3];
					sscanf_s(coord[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &t[0], &b, &c, &t[1], &b, &d, &t[2], &b);
					faces.push_back(new objFace(b, a, c, d, t[0], t[1], t[2], curmat));
				}
				else {
					sscanf_s(coord[i]->c_str(), "f %d %d %d", &a, &b, &c);
					faces.push_back(new objFace(-1, a, b, c, 0, 0, 0, curmat));
				}
				tris_qty++;
			}
		}
		else if ((*coord[i])[0] == 'u' && (*coord[i])[1] == 's' && (*coord[i])[2] == 'e')     //use material material_name
		{
			char tmp[200];
			sscanf_s(coord[i]->c_str(), "usemtl %s", tmp, 128);      //read the name of the material to tmp
			for (unsigned int i = 0;i<materials.size();i++)     //go through all of the materials
			{
				if (strcmp(materials[i]->name.c_str(), tmp) == 0)   //and compare the tmp with the name of the material
				{
					curmat = i;       //if it's equal then set the current material to that
					break;
				}
			}
		}
		else if ((*coord[i])[0] == 'm' && (*coord[i])[1] == 't' && (*coord[i])[2] == 'l' && (*coord[i])[3] == 'l')      //material library, a file, which contain
																														//all of the materials
		{
			char filen[200];
			sscanf_s(coord[i]->c_str(), "mtllib %s", filen, 128);    //read the filename
			std::ifstream mtlin(filen);     //open the file
			if (!mtlin.is_open())    //if not opened error message, clean all memory, return with -1
			{
				std::cout << "connot open the material file " << filen << std::endl;
				Clean();
				return false;
			}
			ismaterial = true;        //we use materials
			std::vector<std::string> tmp;//contain all of the line of the file
			char c[200];
			int x;
			int y;
			int z;
			while (!mtlin.eof())
			{
				mtlin.getline(c, 200);   //read all lines to tmp
				tmp.push_back(c);
			}
			char name[200]; //name of the material
			char filename[200];     //filename of the texture
			float amb[3], dif[3], spec[3], alpha, ns, ni;        //Fs, shininess, and something else
			int illum;
			unsigned int texture;
			bool ismat = false;       //do we already have a material read in to these variables?
			strcpy_s(filename, "\0");  //set filename to nullbyte character
			for (unsigned int i = 0;i<tmp.size();i++) //go through all lines of the mtllib file
			{
				x = 0;
				y = 1;
				z = 2;
				if (tmp[i][x] == '#')      //we don't care about comments
					continue;
				if (tmp[i][x] == '\t')
				{
					x++;
					y++;
					z++;
				}
				if (tmp[i][x] == 'n' && tmp[i][y] == 'e' && tmp[i][z] == 'w')  //new material
				{
					if (ismat)       //if we have a material
					{
						if (strcmp(filename, "\0") != 0)    //if we have a texture
						{
							materials.push_back(new Material(name, alpha, ns, ni, dif, amb, spec, illum, texture, filename)); //push back
							strcpy_s(filename, "\0");
						}
						else {
							materials.push_back(new Material(name, alpha, ns, ni, dif, amb, spec, illum, -1, filename));              //push back, but use -1 to texture              
						}
					}
					ismat = false;    //we start from a fresh material
					sscanf_s(tmp[i].c_str(), "newmtl %s", name, 128);        //read in the name
				}
				else if (tmp[i][x] == 'N' && tmp[i][y] == 's')      //the shininess
				{
					sscanf_s(tmp[i].c_str(), "Ns %f", &ns);
					ismat = true;
				}
				else if (tmp[i][x] == 'K' && tmp[i][y] == 'a')      //the ambient
				{
					sscanf_s(tmp[i].c_str(), "Ka %f %f %f", &amb[0], &amb[1], &amb[2]);
					ismat = true;
				}
				else if (tmp[i][x] == 'K' && tmp[i][y] == 'd')      //the diffuse
				{
					sscanf_s(tmp[i].c_str(), "Kd %f %f %f", &dif[0], &dif[1], &dif[2]);
					ismat = true;
				}
				else if (tmp[i][x] == 'K' && tmp[i][y] == 's')      //the specular
				{
					sscanf_s(tmp[i].c_str(), "Ks %f %f %f", &spec[0], &spec[1], &spec[2]);
					ismat = true;
				}
				else if (tmp[i][x] == 'N' && tmp[i][y] == 'i')      //the I don't know what is this
				{
					sscanf_s(tmp[i].c_str(), "Ni %f", &ni);
					ismat = true;
				}
				else if (tmp[i][x] == 'd' && tmp[i][y] == ' ')      //the alpha
				{
					sscanf_s(tmp[i].c_str(), "d %f", &alpha);
					ismat = true;
				}
				else if (tmp[i][x] == 'i' && tmp[i][y] == 'l')      //the illum (don't ask)
				{
					sscanf_s(tmp[i].c_str(), "illum %d", &illum);
					ismat = true;
				}
				else if (tmp[i][x] == 'm' && tmp[i][y] == 'a')      //and the texture
				{
					if (tmp[i][0] == '\t')
						sscanf_s(tmp[i].c_str(), "\tmap_Kd %s", filename, 128);
					else
						sscanf_s(tmp[i].c_str(), "map_Kd %s", filename, 128);
					texture = LoadTextureObj(filename);  //read the filename, and use the loadTexture function to load it, and get the id.
					ismat = true;
				}
			}
			if (ismat)       //there is no newmat after the last newmat, so we have to put the last material 'manually'
			{
				if (strcmp(filename, "\0") != 0)
				{
					materials.push_back(new Material(name, alpha, ns, ni, dif, amb, spec, illum, texture, filename));
				}
				else {
					materials.push_back(new Material(name, alpha, ns, ni, dif, amb, spec, illum, -1, filename));
				}
			}
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 't')    //back to the obj file, texture coorinate
		{
			float u, v;
			sscanf_s(coord[i]->c_str(), "vt %f %f", &u, &v);     //read the uv coordinate
			texturecoordinate.push_back(new TexCoord(u, 1 - v));       //I push back 1-v instead of normal v, because obj file use the upper left corner as 0,0 coorinate
																	   //but OpenGL use bottom left corner as 0,0, so I convert it
			istexture = true;
		}
	}
	if (materials.size() == 0) //if some reason the material file doesn't contain any material, we don't have material
		ismaterial = false;
	else    //else we have
		ismaterial = true;
	std::cout << vertex.size() << " " << normals.size() << " " << faces.size() << " " << materials.size() << std::endl;     //test purposes
				

	listNum = glGenLists(1);      //I generate a unique identifier for the list
	glNewList(listNum, GL_COMPILE);

	int last = -1;    //the last material (default -1, which doesn't exist, so we use the first material)
	for (unsigned int i = 0;i<faces.size();i++) //go throught all faces
	{
		if (last != faces[i]->mat && ismaterial)   //if we have a meterial AND the last material is not the same
		{
			//set all of the material property
			float diffuse[] = { materials[faces[i]->mat]->dif[0],materials[faces[i]->mat]->dif[1],materials[faces[i]->mat]->dif[2],1.0 };
			float ambient[] = { materials[faces[i]->mat]->amb[0],materials[faces[i]->mat]->amb[1],materials[faces[i]->mat]->amb[2],1.0 };
			float specular[] = { materials[faces[i]->mat]->spec[0],materials[faces[i]->mat]->spec[1],materials[faces[i]->mat]->spec[2],1.0 };
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			glMaterialf(GL_FRONT, GL_SHININESS, materials[faces[i]->mat]->ns);
			last = faces[i]->mat;     //set the current to last
			if (materials[faces[i]->mat]->texture == -1)       //if we don't have texture, disable it, else enable it
				glDisable(GL_TEXTURE_2D);
			else {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, materials[faces[i]->mat]->texture); //and use it
			}
		}
			/*GLfloat white[] = { 1, 1, 1, 1.0f };
			glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
			glMaterialfv(GL_FRONT, GL_AMBIENT, white);*/

		if (faces[i]->four)      //if quad
		{
			glBegin(GL_QUADS);
			if (isnormals)   //if there are normals
				glNormal3f(normals[faces[i]->facenum - 1]->x, normals[faces[i]->facenum - 1]->y, normals[faces[i]->facenum - 1]->z);    //use them

			if (istexture && materials[faces[i]->mat]->texture != -1)  //if there are textures
				glTexCoord2f(texturecoordinate[faces[i]->texcoord[0] - 1]->u, texturecoordinate[faces[i]->texcoord[0] - 1]->v);      //set the texture coorinate

			glVertex3f(vertex[faces[i]->faces[0] - 1]->x, vertex[faces[i]->faces[0] - 1]->y, vertex[faces[i]->faces[0] - 1]->z);

			if (istexture && materials[faces[i]->mat]->texture != -1)
				glTexCoord2f(texturecoordinate[faces[i]->texcoord[1] - 1]->u, texturecoordinate[faces[i]->texcoord[1] - 1]->v);

			glVertex3f(vertex[faces[i]->faces[1] - 1]->x, vertex[faces[i]->faces[1] - 1]->y, vertex[faces[i]->faces[1] - 1]->z);

			if (istexture && materials[faces[i]->mat]->texture != -1)
				glTexCoord2f(texturecoordinate[faces[i]->texcoord[2] - 1]->u, texturecoordinate[faces[i]->texcoord[2] - 1]->v);

			glVertex3f(vertex[faces[i]->faces[2] - 1]->x, vertex[faces[i]->faces[2] - 1]->y, vertex[faces[i]->faces[2] - 1]->z);

			if (istexture && materials[faces[i]->mat]->texture != -1)
				glTexCoord2f(texturecoordinate[faces[i]->texcoord[3] - 1]->u, texturecoordinate[faces[i]->texcoord[3] - 1]->v);

			glVertex3f(vertex[faces[i]->faces[3] - 1]->x, vertex[faces[i]->faces[3] - 1]->y, vertex[faces[i]->faces[3] - 1]->z);
			glEnd();
		}
		else {
			glBegin(GL_TRIANGLES);
			if (isnormals)   //if there are normals
				glNormal3f(normals[faces[i]->facenum - 1]->x, normals[faces[i]->facenum - 1]->y, normals[faces[i]->facenum - 1]->z);

			if (istexture && materials[faces[i]->mat]->texture != -1)
				glTexCoord2f(texturecoordinate[faces[i]->texcoord[0] - 1]->u, texturecoordinate[faces[i]->texcoord[0] - 1]->v);


			glVertex3f(vertex[faces[i]->faces[0] - 1]->x, vertex[faces[i]->faces[0] - 1]->y, vertex[faces[i]->faces[0] - 1]->z);

			if (istexture && materials[faces[i]->mat]->texture != -1)
				glTexCoord2f(texturecoordinate[faces[i]->texcoord[1] - 1]->u, texturecoordinate[faces[i]->texcoord[1] - 1]->v);

			glVertex3f(vertex[faces[i]->faces[1] - 1]->x, vertex[faces[i]->faces[1] - 1]->y, vertex[faces[i]->faces[1] - 1]->z);


			if (istexture && materials[faces[i]->mat]->texture != -1)
				glTexCoord2f(texturecoordinate[faces[i]->texcoord[2] - 1]->u, texturecoordinate[faces[i]->texcoord[2] - 1]->v);

			glVertex3f(vertex[faces[i]->faces[2] - 1]->x, vertex[faces[i]->faces[2] - 1]->y, vertex[faces[i]->faces[2] - 1]->z);
			glEnd();
		}
	}

	glEndList();

	Clean();

	return true;
}


bool Mesh::Load3DS(char* fileName, char* texturePath)
{
	string paths(fileName);

	if (texturePath != '\0')
		LoadTexture(texturePath);

	int i; //Index variable
	FILE *inFile; //File pointer
	unsigned short chunkId, s_temp; //Chunk identifier unsigned
	int chunkLength; //Chunk length
	unsigned char name; //Char variable
	unsigned short size; //Number of elements in each chunk
	unsigned short faceFlag; //Flag that stores some face information
	if ((inFile = fopen(fileName, "rb")) == NULL)
		return false; //Open the file

	fileType = THREEDS;

	while (ftell(inFile) < FileLength(_fileno(inFile))) //Loop to scan the whole file
	{
		fread(&chunkId, 2, 1, inFile); //Read the chunk header
		fread(&chunkLength, 4, 1, inFile); //Read the length of the chunk
										   //getchar(); //Insert this command for debug (to wait for keypress for each chuck reading)
		switch (chunkId)
		{
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн MAIN3DS -нн-нн
			// Description: Main chunk, contains all the other chunks
			// Chunk ID: 4d4d
			// Chunk Length: 0 + sub chunks
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн
		case 0x4d4d:
			break;
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн EDIT3DS
			// Description: 3D Editor chunk, objects layout info
			// Chunk ID: 3d3d (hex)
			// Chunk Length: 0 + sub chunks
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн
		case 0x3d3d:
			break;
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн EDIT_OBJECT -нн-нн-
			// Description: Object block, info for each object
			// Chunk ID: 4000 (hex)
			// Chunk Length: len(object name) + sub chunks
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн
		case 0x4000:
			i = 0;
			do
			{
				fread(&name, 1, 1, inFile);
				meshName[i] = name;
				i++;
			} while (name != '\0' && i<20);
			break;
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн OBJ_TRIMESH -нн-нн-
			// Description: Triangular mesh, contains chunks for 3d mesh info
			// Chunk ID: 4100 (hex)
			// Chunk Length: 0 + sub chunks
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн
		case 0x4100:
			break;
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн TRI_VERTEXL -нн-нн-
			// Description: Vertices list
			// Chunk ID: 4110 (hex)
			// Chunk Length: 1 x unsigned short (number of vertices)
			// + 3 x float (vertex coordinates) x (number of vertices)
			// + sub chunks
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нннн
		case 0x4110:
			fread(&size, sizeof(unsigned short), 1, inFile);
			vertices_qty = size;
			for (i = 0; i<size; i++)
			{
				fread(&vertices[i].x, sizeof(float), 1, inFile);
				fread(&vertices[i].y, sizeof(float), 1, inFile);
				fread(&vertices[i].z, sizeof(float), 1, inFile);
			}
			break;
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн TRI_FACEL1 -нн-нн-нн
			// Description: triangles (faces) list
			// Chunk ID: 4120 (hex)
			// Chunk LengtH: 1 x unsigned short (number of triangles)
			// + 3 x unsigned short (triangle points) x(number of triangles)
			// + sub chunks
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн
		case 0x4120:
			fread(&size, sizeof(unsigned short), 1, inFile);
			tris_qty = size;
			for (i = 0; i<size; i++)
			{
				fread(&s_temp, sizeof(unsigned short), 1, inFile);
				indices[i].a = s_temp;
				fread(&s_temp, sizeof(unsigned short), 1, inFile);
				indices[i].b = s_temp;
				fread(&s_temp, sizeof(unsigned short), 1, inFile);
				indices[i].c = s_temp;
				fread(&faceFlag, sizeof(unsigned short), 1, inFile);
			}
			break;
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн TRI_MAPPINGCOORS -нн-нн-нн
			// Description: Vertices list
			// Chunk ID: 4140 (hex)
			// Chunk Length: 1 x unsigned short (number of mapping points)
			// + 2 x float (mapping coordinates) x (number of mapping points)
			// + sub chunks
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн
		case 0x4140:
			fread(&size, sizeof(unsigned short), 1, inFile);
			if (size >= 1)
				usesUV = true;
			for (i = 0; i < size; i++)
			{
				fread(&texCoords[i].u, sizeof(float), 1, inFile);
				fread(&texCoords[i].v, sizeof(float), 1, inFile);
			}
			break;
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн Skip unknown chunks -нн-нн-нн-нн
			//We need to skip all the chunks that currently we don't use
			//We use the chunk lenght information to set the file pointer
			//to the same level next chunk
			//-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн-нн--н-нн-нн-нн-нн-нн-нн
		default:
			fseek(inFile, chunkLength - 6, SEEK_CUR);
		}
	}
	fclose(inFile); //Close the file.

	CompileList();

	return true;
}

void Mesh::CompileList()
{
	listNum = glGenLists(1);
	glNewList(listNum, GL_COMPILE);


	float ambient[] = { 0.80f, 0.8f, 0.8f, 1.0f };
	float diffuse[] = { 0.80f, 0.8f, 0.8f, 1.0f };
	float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float shininess = 100.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	if (threeDSTexture != nullptr)
	{
		if (threeDSTexture->GetID() > 0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, threeDSTexture->GetID());
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			usesMaterial = true;
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
			usesMaterial = false;
		}
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
		usesMaterial = false;
	}

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < tris_qty; i++)
	{
		if (usesMaterial)
		{
			glTexCoord2f(
				texCoords[indices[i].a].u,
				texCoords[indices[i].a].v);
		}

		glVertex3f(
			vertices[indices[i].a].x,
			vertices[indices[i].a].y,
			vertices[indices[i].a].z);

		if (usesMaterial)
		{
			glTexCoord2f(
				texCoords[indices[i].b].u,
				texCoords[indices[i].b].v);
		}

		glVertex3f(
			vertices[indices[i].b].x,
			vertices[indices[i].b].y,
			vertices[indices[i].b].z);

		if (usesMaterial)
		{
			glTexCoord2f(
				texCoords[indices[i].c].u,
				texCoords[indices[i].c].v);
		}

		glVertex3f(
			vertices[indices[i].c].x,
			vertices[indices[i].c].y,
			vertices[indices[i].c].z);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);

	glEnd();
	glEndList();

	listCompiled = true;
}

bool Mesh::LoadTexture(char* path, int width, int height)
{
	//if (texture != NULL)
	//	delete texture;
	threeDSTexture = new Texture(path, width, height);
	if (listCompiled)
		CompileList();
	if (threeDSTexture != NULL && threeDSTexture != nullptr)
		return true;
	return false;
}

unsigned int Mesh::LoadTextureObj(const char* filename) //Only works with BMP files, with R5 B6 G5 formatting
{
	string fileString(filename);
	unsigned int num;
	glGenTextures(1, &num);
	SDL_Surface* img = SDL_LoadBMP(filename);
	if (SDL_GetError() != "Couldn't open " + fileString)
	{
		glBindTexture(GL_TEXTURE_2D, num);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, img->pixels);
		glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		SDL_FreeSurface(img);
		texture.push_back(num);

		return num;
	}
	else return -1;
}

#include <windows.h>
#include <GL\glu.h>

void Mesh::Clean()
{
	for (unsigned int i = 0;i<coord.size();i++)
		delete coord[i];
	for (unsigned int i = 0;i<faces.size();i++)
		delete faces[i];
	for (unsigned int i = 0;i<normals.size();i++)
		delete normals[i];
	for (unsigned int i = 0;i<vertex.size();i++)
		delete vertex[i];
	for (unsigned int i = 0;i<materials.size();i++)
		delete materials[i];
	for (unsigned int i = 0;i<texturecoordinate.size();i++)
		delete texturecoordinate[i];

	coord.clear();
	faces.clear();
	normals.clear();
	vertex.clear();
	materials.clear();
	texturecoordinate.clear();
}