#include "Transform.hpp"


Transform::Transform(GameObject* gameObject) :
	Component(gameObject),
	_parent(nullptr),
	_matrix(Matrix::identity),
	_scale(1.0f, 1.0f, 1.0f)
{
	for (int i = 0; i < Num_Notify; i++)
	{
		_notify[i] = false;
	}
}


Transform::~Transform()
{
	while (!_children.empty())
	{
		Transform* last = _children.back();
		_children.pop_back();
		delete(last);
	}
	if (_parent)_parent->removeChild(this);
}


void Transform::addChild(Transform* child)
{
	if (child->_parent == this)return;
	if (child->_parent)child->_parent->removeChild(child);

	Matrix tr = ~getTRMatrix()*child->getTRMatrix();
	Matrix s = ~getSMatrix()*child->getSMatrix();

	child->setPosition(Matrix::extractTranslation(tr));
	child->setRotation(Matrix::extractRotation(tr));
	child->setScale(Matrix::extractScale(s));

	child->_parent = this;
	_children.push_back(child);
}


void Transform::removeChild(Transform* child)
{
	for (auto it = _children.begin(); it != _children.end(); ++it)
	{
		if (*it == child)
		{
			Matrix tr = child->getTRMatrix();
			Matrix s = child->getSMatrix();

			child->setPosition(Matrix::extractTranslation(tr));
			child->setRotation(Matrix::extractRotation(tr));
			child->setScale(Matrix::extractScale(s));

			_children.erase(it);
			child->_parent = nullptr;

			return;
		}
	}
}


void Transform::translate(const Vector3& translation)
{
	notifyChildren(Position);
	_position += translation;
}


void Transform::setPosition(const Vector3& position)
{
	notifyChildren(Position);
	_position = position;
}


void Transform::rotate(float x, float y, float z)
{
	rotate(Quaternion(Vector3(x, y, z)));
}


void Transform::rotate(const Vector3& rotation)
{
	rotate(Quaternion(rotation));
}


void Transform::rotate(const Quaternion& rotation)
{
	notifyChildren(Rotation);
	_rotation = rotation * _rotation;
}


void Transform::setRotation(float x, float y, float z)
{
	setRotation(Quaternion(Vector3(x, y, z)));
}


void Transform::setRotation(const Vector3& rotation)
{
	setRotation(Quaternion(rotation));
}


void Transform::setRotation(const Quaternion& rotation)
{
	notifyChildren(Rotation);
	_rotation = rotation;
}


void Transform::setScale(const Vector3& scale)
{
	notifyChildren(Scale);
	_scale = scale;
}


Matrix Transform::getTRMatrix()
{
	if (_notify[Position] | _notify[Rotation])
	{
		_notify[Position] = _notify[Rotation] = false;

		Matrix t = Matrix::createTranslation(_position);
		Matrix r = Matrix::createRotation(_rotation);

		_trMatrix = t * r;
		if (_parent != nullptr)
		{
			_trMatrix = _parent->getTRMatrix() * _trMatrix;
		}
	}
	return _trMatrix;
}


Matrix Transform::getSMatrix()
{
	if (_notify[Scale])
	{
		_notify[Scale] = false;

		_sMatrix = Matrix::createScale(_scale);
		if (_parent != nullptr)
		{
			_sMatrix *= _parent->getSMatrix();
		}
	}
	return _sMatrix;
}


const Matrix& Transform::getMatrix()
{
	if (_notify[Position] | _notify[Rotation] | _notify[Scale])
	{
		_matrix = getTRMatrix() * getSMatrix();
	}
	return _matrix;
}


void Transform::notifyChildren(NotifyType type)
{
	_notify[type] = true;
	for (auto child : _children)
	{
		child->notifyChildren(type);
	}
}


const Vector3 Transform::getWorldPosition()
{
	Vector3 position = _position;
	if (_parent != nullptr)
	{
		position += _parent->getWorldPosition();
	}
	return position;
}