#pragma once
#include <iostream>
#include <SDL_mixer.h>
#include <algorithm>

using namespace std;

#define NOMINMAX

#ifndef _PLAYMYSOUND
#define _PLAYMYSOUND(sound, volume, loops)
	inline void PlayMySound(Mix_Chunk* sound, float volume = 100, bool loops = false) 
	{
		int i = Mix_PlayChannel(-1, sound, loops);
		Mix_Volume(i, (int)max(0.0f, min((volume / 100) * MIX_MAX_VOLUME, 128.0f)));
	}
#endif

	inline float FLerp(float a, float b, float t) { return (a + t * (b - a)); }

enum SCREENS
{
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

enum TEXTALIGNMENT
{
	TLEFT = 0,
	TCENTRE,
	TRIGHT
};

enum COLLIDERTYPE
{
	BOX = 0,
	SPHERE = 1
};

enum GAMESTATE
{
	MENU,
	PAUSED,
	PLAYING,
	WINSCREEN,
	LOSESCREEN,
	TIESCREEN,
	UNKNOWN
};


struct Rect2D
{
	float x;
	float y;
	float width;
	float height;

	Rect2D(float initialX, float initialY, float initialWidth, float initialHeight)
	{
		x = initialX;
		y = initialY;
		width  = initialWidth;
		height = initialHeight;
	}
};

struct Vector2D
{
	float x;
	float y;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float initialX, float initialY)
	{
		x = initialX;
		y = initialY;
	}
};

struct Vector3D 
{
public:
	float x, y, z;

	Vector3D()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3D(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}


	void Print()
	{
		cout << "x: " << x << "\t\ty: " << y << "\t\tz: " << z << endl;
	}

	Vector3D operator+ (const Vector3D other)
	{
		Vector3D sum;

		sum.x = x + other.x;
		sum.y = y + other.y;
		sum.z = z + other.z;

		return sum;
	}

	Vector3D operator- (const Vector3D other)
	{
		Vector3D sum;

		sum.x = x - other.x;
		sum.y = y - other.y;
		sum.z = z - other.z;

		return sum;
	}

	Vector3D operator* (const Vector3D other)
	{
		Vector3D sum;

		sum.x = x * other.x;
		sum.y = y * other.y;
		sum.z = z * other.z;

		return sum;
	}

	Vector3D operator* (const float other)
	{
		Vector3D sum;

		sum.x = x * other;
		sum.y = y * other;
		sum.z = z * other;

		return sum;
	}

	Vector3D operator/ (const Vector3D other)
	{
		Vector3D sum;

		sum.x = x / other.x;
		sum.y = y / other.y;
		sum.z = z / other.z;

		return sum;
	}

	Vector3D operator/ (const float other)
	{
		Vector3D sum;

		sum.x = x / other;
		sum.y = y / other;
		sum.z = z / other;

		return sum;
	}

	Vector3D& operator+= (const Vector3D other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}	
	
	Vector3D& operator+= (const float other)
	{
		x += other;
		y += other;
		z += other;

		return *this;
	}

	Vector3D& operator-= (const Vector3D other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}	
	
	Vector3D& operator-= (const float other)
	{
		x -= other;
		y -= other;
		z -= other;

		return *this;
	}

	Vector3D& operator*= (const Vector3D other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}

	Vector3D& operator*= (const float other)
	{
		x *= other;
		y *= other;
		z *= other;

		return *this;
	}

	Vector3D& operator/= (const Vector3D other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}

	Vector3D& operator/= (const float other)
	{
		x /= other;
		y /= other;
		z /= other;

		return *this;
	}

	float Magnitude()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	Vector3D Normalise()
	{
		return Vector3D(x / Magnitude(), y / Magnitude(), z / Magnitude());
	}

	Vector3D Cross(Vector3D v2)
	{
		Vector3D cross;
		cross.x = (y*v2.z) - (z*v2.y);
		cross.y = -((x*v2.z) - (z*v2.x));
		cross.z = (x*v2.y) - (y*v2.x);
		return cross;
	}

	float Dot(Vector3D v2)
	{
		return (x * v2.x) + (y * v2.y) + (z * v2.z);
	}

	float DistSquared(Vector3D other)
	{
		return 
		(
			((x - other.x) * (x - other.x)) +
			((y - other.y) * (y - other.y)) +
			((z - other.z) * (z - other.z))
		);
	}
};

struct Bounds
{
	Vector3D min;
	Vector3D max;

	void CalculateBounds(Vector3D position, Vector3D dimensions)
	{
		max.x = position.x + dimensions.x / 2;
		min.x = position.x - dimensions.x / 2;
		max.y = position.y + dimensions.y / 2;
		min.y = position.y - dimensions.y / 2;
		max.z = position.z + dimensions.z / 2;
		min.z = position.z - dimensions.z / 2;
	}
};

struct Colour 
{
	float r;
	float g;
	float b;
	
	Colour()
	{
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
	}

	Colour(float initialR, float initialG, float initialB)
	{
		r = initialR;
		g = initialG;
		b = initialB;
	}
};

struct Triangle
{
	int a;
	int b;
	int c;
};

struct objFace {
	int facenum;			//The face number. Subtract 1 for use in indexes
	bool four;				//Whether there are 4 vertices (it's a quad)
	int faces[4];			//The index of vertices for each face. Subtract 1 for use in indexes
	int texcoord[4];		//Texture coordinates. Subtract 1 for use in indexes
	int mat;				//Index of the material used for the face

	objFace()
	{

	}

	objFace(int facen, int f1, int f2, int f3, int t1, int t2, int t3, int m) //Tri constructor
	{
		facenum = facen;
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
		texcoord[0] = t1;
		texcoord[1] = t2;
		texcoord[2] = t3;
		mat = m;
		four = false;
	}

	objFace(int facen, int f1, int f2, int f3, int f4, int t1, int t2, int t3, int t4, int m)  //Quad constructor
	{
		facenum = facen;
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
		faces[3] = f4;
		texcoord[0] = t1;
		texcoord[1] = t2;
		texcoord[2] = t3;
		texcoord[3] = t4;
		mat = m;
		four = true;
	}
};

struct Material
{
	string fileName;
	string name;       //the name of the material
	float alpha, ns, ni;      //some property, alpha, shininess, and some other, which we not used
	float dif[3], amb[3], spec[3];    //the color property (diffuse, ambient, specular)
	int illum;      //illum - we not use it
	int texture;    //the id for the texture, if there is no texture than -1

	Material(const char* na, float al, float n, float ni2, float* d, float* a, float* s, int i, int t, char* fna)
	{
		//fileName = fna;
		name = na;
		alpha = al;
		ns = n;
		ni = ni2;

		dif[0] = d[0];
		dif[1] = d[1];
		dif[2] = d[2];

		amb[0] = a[0];
		amb[1] = a[1];
		amb[2] = a[2];

		spec[0] = s[0];
		spec[1] = s[1];
		spec[2] = s[2];

		illum = i;
		texture = t;
	}
};

struct TexCoord
{
	float u;
	float v;

	TexCoord()
	{
		u = v = 0;
	}

	TexCoord(float _u, float _v)
	{
		u = _u;
		v = _v;
	}
};


#define MAX_VERTICES 35000
#define MAX_POLYGONS 35000
