#ifndef __H_CAMERA__
#define __H_CAMERA__

#include "Component.hpp"
#include "Transform.hpp"
#include "Matrix.hpp"
#include "Event.hpp"

class Camera : public Component, public INotifiable
{
public:
	enum Type
	{
		Type_Perspective,
		Type_Orthographic
	};

	Camera(GameObject* gameObject);
	virtual ~Camera();

	void setPerspective(float fov, float zNear, float zFar);
	void setOrthographic(float zNear, float zFar);

	void setZNear(float zNear);
	void setZFar(float zFar);
	void setFov(float fov);
	float getZNear();
	float getZFar();
	float getFov();

	const Matrix& getMatrix(bool pulling = false);
	bool matrixPulled();
	void notify(Event event, void* sender) override;

private:
	enum DirtyBit
	{
		Dirty_None = 0,
		Dirty_View = 1,
		Dirty_Projection = 2,
		Dirty_Matrix = 4
	};
	char _dirtyBits;

	EventListener _transformChangeListener;

	Type _type;
	float _zNear;
	float _zFar;
	float _fov;

	Matrix _pMatrix;
	Matrix _vMatrix;
	Matrix _pvMatrix;
	bool _matrixPulled;

	void dirty(char dirtyBits);
	const Matrix& getPMatrix();
	const Matrix& getVMatrix();
};

#endif