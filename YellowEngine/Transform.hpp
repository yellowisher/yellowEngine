#ifndef __H_TRANSFORM__
#define __H_TRANSFORM__

#include <list>

#include "Component.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

class Transform : public Component
{
public:
	Transform(GameObject* gameObject);
	virtual ~Transform();

	void addChild(Transform* child);
	void removeChild(Transform* child);

	void translate(const Vector3& translation);
	void setPosition(const Vector3& position);
	void rotate(float x, float y, float z);
	void rotate(const Vector3& rotation);
	void rotate(const Quaternion& rotation);
	void setRotation(float x, float y, float z);
	void setRotation(const Vector3& rotation);
	void setRotation(const Quaternion& rotation);
	void setScale(const Vector3& scale);

	const Matrix& getMatrix();
	const Vector3 getWorldPosition();

private:
	enum NotifyType
	{
		Position,
		Rotation,
		Scale,
		Num_Notify
	};

	Transform* _parent;
	std::list<Transform*> _children;

	Matrix _matrix;
	Matrix _trMatrix;
	Matrix _sMatrix;
	bool _notify[Num_Notify];

	Vector3 _position;
	Vector3 _scale;
	Quaternion _rotation;

	Matrix getTRMatrix();
	Matrix getSMatrix();
	void notifyChildren(NotifyType type);
};

#endif