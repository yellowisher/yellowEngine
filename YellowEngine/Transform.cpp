#include "Transform.hpp"


Transform::Transform(GameObject* gameObject) :
	Component(gameObject),
	_matrix(Matrix::identity),
	_dirty(false),
	_localScale(1.0f, 1.0f, 1.0f)
{
}


Transform::~Transform()
{
}


void Transform::translate(Vector3 translation)
{
	_dirty = true;
	_position += translation;
	_localPosition += translation;
}


void Transform::setPosition(Vector3 position, Type type)
{
	_dirty = true;
	Vector3 diff;
	if (type == World)
	{
		diff = position - _position;
		_position = position;
		_localPosition += diff;
	}
	else
	{
		diff = position - _localPosition;
		_localPosition = position;
		_position += diff;
	}
}


void Transform::rotate(Vector3 rotation)
{
	rotate(Quaternion(rotation));
}


void Transform::rotate(Quaternion rotation)
{
	_dirty = true;
	_rotation = rotation * _rotation;
	_localRotation = rotation * _localRotation;
}


void Transform::setRotation(Vector3 rotation, Type type)
{
	setRotation(Quaternion(rotation), type);
}


void Transform::setRotation(Quaternion rotation, Type type)
{
	_dirty = true;
	Quaternion diff;
	if (type == World)
	{
		// should be normalized
		diff = rotation * _rotation.conjugate();
		_rotation = rotation;
		_localRotation = diff * _localRotation;
	}
	else
	{
		diff = rotation * _localRotation.conjugate();
		_localRotation = rotation;
		_rotation = diff * _rotation;
	}
}


void Transform::setScale(Vector3 scale)
{
	_dirty = true;
	_scale = scale;
	_localScale = scale;
}


const Matrix& Transform::getMatrix()
{
	if (_dirty)
	{
		_dirty = false;
		Matrix scale = Matrix::identity;
		scale._m2[0][0] = _localScale.x;
		scale._m2[1][1] = _localScale.y;
		scale._m2[2][2] = _localScale.z;

		Matrix rotation = _localRotation.toMatrix();

		Matrix translation = Matrix::identity;
		translation._m[12] = _localPosition.x;
		translation._m[13] = _localPosition.y;
		translation._m[14] = _localPosition.z;

		_matrix = translation * rotation * scale;
	}
	return _matrix;
}
