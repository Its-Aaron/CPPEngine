#include "Transform.h"

Transform::Transform()
{
	position = Vector3D();
	scale = Vector3D(1, 1, 1);
	rotation = Vector3D();

	componentTypeName = "Transform";
}

Transform::Transform(Vector3D _position)
{
	position = _position;
	scale = Vector3D(1, 1, 1);
	rotation = Vector3D();

	componentTypeName = "Transform";
}

Transform::Transform(Vector3D _position, Vector3D _rotation)
{
	position = _position;
	scale = Vector3D(1, 1, 1);
	rotation = _rotation;

	componentTypeName = "Transform";
}

Transform::Transform(Vector3D _position, Vector3D _scale, Vector3D _rotation)
{
	position = _position;
	scale = _scale;
	rotation = _rotation;

	componentTypeName = "Transform";
}

Transform::~Transform()
{
}

Transform Transform::operator+ (const Transform other)
{
		Transform sum;

		sum.position = position + other.position;
		sum.scale.x = other.scale.x * scale.x;
		sum.scale.y = other.scale.y * scale.y;
		sum.scale.z = other.scale.z * scale.z;
		sum.rotation = rotation + other.rotation;

		return sum;
}

Transform Transform::operator* (const Transform other)
{
	Transform sum;

	sum.position = position * other.position;
	sum.scale.x = other.scale.x * scale.x;
	sum.scale.y = other.scale.y * scale.y;
	sum.scale.z = other.scale.z * scale.z;
	sum.rotation = rotation * other.rotation;

	return sum;
}

Vector3D Transform::ForwardVector() 
{ 
	return Vector3D(
		(float)(cos(DegToRad(rotation.z)) * sin(DegToRad(rotation.y))),
		(float)sin(DegToRad(rotation.z)),
		(float)(cos(DegToRad(rotation.z)) * cos(DegToRad(rotation.y))));
}


void Transform::LookAt(Vector3D point)
{
	Vector3D otherF = point - position;

	Vector3D newRot = Vector3D
	(

	);
}


Vector3D Transform::RightVector()
{
	return Vector3D(
		(float)sin(DegToRad(rotation.y) - 3.14f / 2.0f),
		0, 
		(float)cos(DegToRad(rotation.y) - 3.14f / 2.0f));
}

Vector3D Transform::UpVector()
{
	return Vector3D(
		(RightVector().y * ForwardVector().z) - (RightVector().z * ForwardVector().y),
		(RightVector().z * ForwardVector().x) - (RightVector().x * ForwardVector().z),
		(RightVector().x * ForwardVector().y) - (RightVector().y * ForwardVector().x));
}