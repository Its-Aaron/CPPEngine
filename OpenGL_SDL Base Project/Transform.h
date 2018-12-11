#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "Component.h"
#include <math.h>

class Transform : public Component
{
public:
	Transform();
	Transform(Vector3D _position);
	Transform(Vector3D _position, Vector3D _rotation);
	Transform(Vector3D _position, Vector3D _scale, Vector3D _rotation);
	~Transform();

	Transform operator+ (const Transform other);
	Transform operator* (const Transform other);

	Transform* parent;

	Vector3D position;
	Vector3D scale;
	Vector3D rotation;

	void PrintPosition() { cout << "Position\tx - " << position.x << "\t\t| y - " << position.y << "\t\t| z - " << position.z << endl; }
	void PrintRotation() { cout << "Rotation\tx - " << rotation.x << "\t\t| y - " << rotation.y << "\t\t| z - " << rotation.z << endl; }
	void PrintScale() { cout << "Scale\t\tx - " << scale.x << "\t\t| y - " << scale.y << "\t\t| z - " << scale.z << endl; }

	double DegToRad(double degrees) { return (degrees * 3.14f) / 180; }
	double RadToDeg(double radians) { return (radians * 180) / 3.14f; }

	Vector3D ForwardVector();
	Vector3D RightVector();
	//Vector3D ForwardVector() { return Vector3D(cos(rotation.x) * cos(rotation.y), cos(rotation.x) * sin(rotation.y), sin(rotation.x)); } https://stackoverflow.com/questions/45664697/calculating-forward-vector-given-rotation-in-3d
	Vector3D UpVector();

	void LookAt(Vector3D point);
};
 
#endif