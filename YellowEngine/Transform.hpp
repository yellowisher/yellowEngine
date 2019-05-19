#ifndef __H_TRANSFORM__
#define __H_TRANSFORM__

#include "Component.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

class Transform : public Component
{
public:
	void translate(Vector3 vector3);
	void rotate(Vector3 vector);
	void rotate(Quaternion quaternion);
	void setScale(Vector3 scale);

protected:
	Transform();

	Transform(GameObject* gameObject);
	virtual ~Transform();

private:
	Vector3 _position;
	Vector3 _scale;
	Quaternion _rotation;
};

#endif