#include "yellowEngine/Component/GameObject.hpp"
#include "yellowEngine/Component/Transform.hpp"


namespace yellowEngine
{
	Transform::Transform(GameObject* gameObject) :
		Component(gameObject),
		position(_position),
		scale(_scale),
		rotation(_rotation),
		_parent(nullptr),
		_scale(1.0f, 1.0f, 1.0f),
		_dirtyBits(Dirty_All)
	{
	}


	void Transform::onCreate()
	{
		transformChangeNotifier.setSender(this);
	}


	void Transform::onDestroy()
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

		// test needed
		Matrix tr = getInverseTRMatrix() * child->getTRMatrix();
		Matrix s = ~getSMatrix() * child->getSMatrix();

		child->setPosition(tr.extractTranslation());
		child->setRotation(tr.extractRotation());
		child->setScale(s.extractScale());

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

				child->setPosition(tr.extractTranslation());
				child->setRotation(tr.extractRotation());
				child->setScale(s.extractScale());

				_children.erase(it);
				child->_parent = nullptr;

				return;
			}
		}
	}


	Transform* Transform::findChild(const std::string& name)
	{
		for (auto child : _children)
		{
			if (child->gameObject->getName() == name)
			{
				return child;
			}
		}
		return nullptr;
	}


	Transform* Transform::getChild(int index)
	{
		// assert index?
		return _children[index];
	}


	void Transform::translate(const Vector3& translation)
	{
		_position += translation;
		dirty(Dirty_Translation);
	}


	void Transform::translate(float x, float y, float z)
	{
		translate(Vector3(x, y, z));
	}


	void Transform::setPosition(const Vector3& position)
	{
		_position = position;
		dirty(Dirty_Translation);
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
		_rotation = rotation * _rotation;
		dirty(Dirty_Rotation);
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
		_rotation = rotation;
		dirty(Dirty_Rotation);
	}


	void Transform::setScale(const Vector3& scale)
	{
		_scale = scale;
		dirty(Dirty_Scale);
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


	const Matrix& Transform::getInverseTRMatrix()
	{
		if (_dirtyBits | Dirty_Inverse_Translation_Rotation)
		{
			_dirtyBits &= ~Dirty_Inverse_Translation_Rotation;

			_itrMatrix = ~getTRMatrix();
		}
		return _itrMatrix;
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
		if (dirtyBits & Dirty_Translation_Rotation)_dirtyBits |= Dirty_Inverse_Translation_Rotation;

		for (auto child : _children)
		{
			child->dirty(dirtyBits);
		}
		// move to getMatrix?
		transformChanged();
	}


	const Vector3 Transform::getWorldPosition()
	{
		return getTRMatrix().extractTranslation();
	}


	const Quaternion Transform::getWorldRotation()
	{
		return getTRMatrix().extractRotation();
	}


	const Vector3 Transform::getUp()
	{
		Quaternion rotation = getTRMatrix().extractRotation();
		Vector3 up = rotation * Vector3::up;
		up.normalize();
		return up;
	}


	const Vector3 Transform::getRight()
	{
		Quaternion rotation = getTRMatrix().extractRotation();
		Vector3 right = rotation * Vector3::right;
		right.normalize();
		return right;
	}


	const Vector3 Transform::getForward()
	{
		Quaternion rotation = getTRMatrix().extractRotation();
		Vector3 forward = rotation * Vector3::forward;
		forward.normalize();
		return forward;
	}


	void Transform::transformChanged()
	{
		transformChangeNotifier.notify(Event_TransformChanged);
	}
}