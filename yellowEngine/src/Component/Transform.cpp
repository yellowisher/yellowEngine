#include "yellowEngine/Component/GameObject.hpp"
#include "yellowEngine/Component/Transform.hpp"


namespace yellowEngine
{
	COMPONENT_IMPL(Transform)


	Transform* Transform::Root = (new GameObject("Root"))->transform;


	Transform::Transform(GameObject* gameObject) :
		Component(gameObject),
		position(_position),
		scale(_scale),
		rotation(_rotation),
		_parent(Root),
		_scale(1.0f, 1.0f, 1.0f),
		_dirtyBits(Dirty_All)
	{
		if (Root != nullptr)
		{
			Root->_children.push_back(this);
		}
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
			delete(last->gameObject);
		}
		if (_parent)_parent->removeChild(this);
	}


	Transform* Transform::getParent()
	{
		return _parent;
	}


	void Transform::addChild(Transform* child)
	{
		if (child->_parent == this)return;
		child->_parent->removeChild(child);

		Matrix childToParent = getInverseMatrix() * child->getMatrix();

		child->setPosition(childToParent.extractTranslation());
		child->setRotation(childToParent.extractRotation());
		child->setScale(childToParent.extractScale());

		child->_parent = this;
		_children.push_back(child);
	}


	void Transform::removeChild(Transform* child)
	{
		for (auto it = _children.begin(); it != _children.end(); ++it)
		{
			if (*it == child)
			{
				Matrix childToParent = child->getMatrix();

				child->setPosition(childToParent.extractTranslation());
				child->setRotation(childToParent.extractRotation());
				child->setScale(childToParent.extractScale());

				_children.erase(it);
				child->_parent = Root;

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


	const std::vector<Transform*>& Transform::getChildren()
	{
		return _children;
	}


	int Transform::getChildCount()
	{
		return (int)_children.size();
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


	const Matrix& Transform::getLocalMatrix()
	{
		if (_dirtyBits | Dirty_LocalMatrix)
		{
			_dirtyBits &= ~Dirty_LocalMatrix;
			_localMatrix =
				Matrix::createTranslation(position) *
				Matrix::createRotation(rotation) *
				Matrix::createScale(scale);
		}
		return _localMatrix;
	}


	const Matrix& Transform::getParentMatrix()
	{
		if (_dirtyBits | Dirty_Parent)
		{
			_dirtyBits &= ~Dirty_Parent;
			if (_parent != Root)
			{
				_parentMatrix = _parent->getMatrix();
			}
		}
		return _parentMatrix;
	}


	const Matrix& Transform::getMatrix()
	{
		if (_dirtyBits | Dirty_Matrix)
		{
			_matrix = getParentMatrix() * getLocalMatrix();
		}
		return _matrix;
	}


	const Matrix& Transform::getInverseMatrix()
	{
		if (_dirtyBits | Dirty_InverseMatrix)
		{
			_dirtyBits &= ~Dirty_LocalMatrix;
			_inverseMatrix = ~getMatrix();
		}
		return _inverseMatrix;
	}


	Matrix Transform::getMatrix(Transform* until)
	{
		Matrix matrix = getLocalMatrix();
		if (_parent != until)
		{
			matrix = _parent->getMatrix(until) * matrix;
		}
		return matrix;
	}


	void Transform::dirty(int dirtyBits)
	{
		dirtyBits |= Dirty_InverseMatrix;
		_dirtyBits |= dirtyBits;

		for (auto child : _children)
		{
			child->dirty(Dirty_Parent);
		}
		transformChanged();
	}


	const Vector3 Transform::getWorldPosition()
	{
		return getMatrix().extractTranslation();
	}


	const Quaternion Transform::getWorldRotation()
	{
		return getMatrix().extractRotation();
	}


	const Vector3 Transform::getUp()
	{
		Quaternion rotation = getMatrix().extractRotation();
		Vector3 up = rotation * Vector3::up;
		up.normalize();
		return up;
	}


	const Vector3 Transform::getRight()
	{
		Quaternion rotation = getMatrix().extractRotation();
		Vector3 right = rotation * Vector3::right;
		right.normalize();
		return right;
	}


	const Vector3 Transform::getForward()
	{
		Quaternion rotation = getMatrix().extractRotation();
		Vector3 forward = rotation * Vector3::forward;
		forward.normalize();
		return forward;
	}


	void Transform::transformChanged()
	{
		transformChangeNotifier.notify(Event_TransformChanged);
	}
}