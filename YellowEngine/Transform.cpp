#include "Transform.hpp"


Transform::Transform(GameObject* gameObject) :
	Component(gameObject),
	_parent(nullptr),
	_scale(1.0f, 1.0f, 1.0f),
	_dirtyBits(Dirty_None)
{
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
	dirty(Dirty_Translation);
	_position += translation;
}


void Transform::translate(float x, float y, float z)
{
	translate(Vector3(x, y, z));
}


void Transform::setPosition(const Vector3& position)
{
	dirty(Dirty_Translation);
	_position = position;
}


void Transform::setPosition(float x, float y, float z)
{
	setPosition(Vector3(x, y, z));
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
	dirty(Dirty_Rotation);
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
	dirty(Dirty_Rotation);
	_rotation = rotation;
}


void Transform::setScale(const Vector3& scale)
{
	dirty(Dirty_Scale);
	_scale = scale;
}


void Transform::setScale(float x, float y, float z)
{
	setScale(Vector3(x, y, z));
}


const Matrix& Transform::getTRMatrix()
{
	if (_dirtyBits | Dirty_Translation_Rotation)
	{
		_dirtyBits &= ~Dirty_Translation_Rotation;

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


const Matrix& Transform::getSMatrix()
{
	if (_dirtyBits & Dirty_Scale)
	{
		_dirtyBits &= ~Dirty_Scale;
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
	if (_dirtyBits != Dirty_None)
	{
		_dirtyBits &= ~Dirty_Matrix;
		_matrix = getTRMatrix() * getSMatrix();
	}
	return _matrix;
}


void Transform::dirty(char dirtyBits)
{
	_dirtyBits |= dirtyBits | Dirty_Matrix;
	for (auto child : _children)
	{
		child->dirty(dirtyBits);
	}
	transformChanged();
}


const Vector3 Transform::getWorldPosition()
{
	return Matrix::extractTranslation(getTRMatrix());
}


const Vector3 Transform::getUp()
{
	Quaternion rotation = Matrix::extractRotation(getTRMatrix());
	Vector3 up = rotation * Vector3::up;
	up.normalize();
	return up;
}


const Vector3 Transform::getRight()
{
	Quaternion rotation = Matrix::extractRotation(getTRMatrix());
	Vector3 right = rotation * Vector3::right;
	right.normalize();
	return right;
}


const Vector3 Transform::getForward()
{
	Quaternion rotation = Matrix::extractRotation(getTRMatrix());
	Vector3 forward = rotation * Vector3::forward;
	forward.normalize();
	return forward;
}


void Transform::addListener(Listener* listener)
{
	_listeners.push_back(listener);
}


void Transform::removeListener(Listener* listener)
{
	for (auto it = _listeners.begin(); it != _listeners.end(); ++it)
	{
		if (*it == listener)
		{
			_listeners.erase(it);
			return;
		}
	}
}


void Transform::transformChanged()
{
	for (auto listener : _listeners)
	{
		listener->onTransformChanged(this);
	}
}