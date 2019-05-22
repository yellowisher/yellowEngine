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
		Type_Perspective,
		Type_Orthographic
	};

	Camera(GameObject* gameObject);
	~Camera();

	void setPerspective(float fov, float zNear, float zFar);
	void setOrthographic(float zNear, float zFar);

	void setZNear(float zNear);
	void setZFar(float zFar);
	void setFov(float fov);
	float getZNear();
	float getZFar();
	float getFov();

	const Matrix& getMatrix();
	void onTransformChanged(Transform* transform);

private:
	enum DirtyBit
	{
		Dirty_None = 0,
		Dirty_View = 1,
		Dirty_Projection = 2
	};
	char _dirtyBits;

	Type _type;
	float _zNear;
	float _zFar;
	float _fov;

	Matrix _pMatrix;
	Matrix _vMatrix;
	Matrix _pvMatrix;

	void dirty(char dirtyBits);
	const Matrix& getPMatrix();
	const Matrix& getVMatrix();
};

#endif