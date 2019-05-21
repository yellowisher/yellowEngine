#ifndef __H_CAMERA__
#define __H_CAMERA__

#include "Component.hpp"
#include "Transform.hpp"
#include "Matrix.hpp"

class Camera : public Component, public Transform::Listener
{
public:
	enum Type
	{
		Perspective,
		Orthographic
	};

	Camera(GameObject* gameObject);
	~Camera();

	void setPerspective(float fieldOfView, float zNear, float zFar);
	void setOrthographic(float zNear, float zFar);

	const Matrix& getPVMatrix();
	void onTransformChanged(Transform* transform);

private:
	Type _type;
	float _zNear;
	float _zFar;

	Matrix _pMatrix;
	Matrix _vMatrix;
	Matrix _pvMatrix;

	void computePVMatrix();
	void setZRange(float zNear, float zFar);
};

#endif