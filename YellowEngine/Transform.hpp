#ifndef __H_TRANSFORM__
#define __H_TRANSFORM__

#include "Component.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

class Transform : public Component
{
	friend class GameObject;

public:
	enum Type
	{
		World,
		Local
	};

	Transform(GameObject* gameObject);
	virtual ~Transform();

	void translate(Vector3 translation);
	void setPosition(Vector3 position, Type type = World);
	void rotate(Vector3 rotation);
	void rotate(Quaternion rotation);
	void setRotation(Vector3 rotation, Type type = World);
	void setRotation(Quaternion rotation, Type type = World);
	void setScale(Vector3 scale);

	const Matrix& getMatrix();

private:
	bool _dirty;
	Matrix _matrix;

	Vector3 _position;
	Vector3 _scale;
	Quaternion _rotation;

	Vector3 _localPosition;
	Vector3 _localScale;
	Quaternion _localRotation;
};

#endif