#include "Camera.hpp"
#include "System.hpp"
#include "GameObject.hpp"

Camera::Camera(GameObject* gameObject) :
	Component(gameObject),
	_matrixPulled(false)
{
	setPerspective(60.0f, 0.01f, 100.0f);
	transformChangeListener.setParent(this);
	transform->transformChangeNotifier.addListener(&transformChangeListener);
}


Camera::~Camera()
{
}


void Camera::setPerspective(float fov, float zNear, float zFar)
{
	_type = Type_Perspective;
	_fov = fov;
	_zNear = zNear;
	_zFar = zFar;
	dirty(Dirty_Projection);
}


void Camera::setOrthographic(float zNear, float zFar)
{
	_type = Type_Orthographic;
	_zNear = zNear;
	_zFar = zFar;
	dirty(Dirty_Projection);
}


void Camera::setZNear(float zNear)
{
	dirty(Dirty_Projection);
	_zNear = zNear;
}


float Camera::getZNear()
{
	return _zNear;
}


void Camera::setZFar(float zFar)
{
	dirty(Dirty_Projection);
	_zFar = zFar;
}


float Camera::getZFar()
{
	return _zFar;
}


void Camera::setFov(float fov)
{
	dirty(Dirty_Projection);
	_fov = fov;
}


float Camera::getFov()
{
	return _fov;
}


const Matrix& Camera::getPMatrix()
{
	if (_dirtyBits & Dirty_Projection)
	{
		_dirtyBits &= ~Dirty_Projection;
		if (_type == Type_Perspective)
		{
			_pMatrix = Matrix::createPerspective(_fov, System::getInstance()->getAspectRatio(), _zNear, _zFar);
		}
		else
		{
			_pMatrix = Matrix::createOrthographic(System::getInstance()->getWidth(), System::getInstance()->getHeight(), _zNear, _zFar);
		}
	}
	return _pMatrix;
}


const Matrix& Camera::getVMatrix()
{
	if (_dirtyBits & Dirty_View)
	{
		_dirtyBits &= ~Dirty_View;
		_vMatrix = transform->getTRMatrix();
		_vMatrix = ~_vMatrix;
	}
	return _vMatrix;
}


bool Camera::matrixPulled()
{
	return _matrixPulled;
}


const Matrix& Camera::getMatrix(bool pulling)
{
	if (pulling)_matrixPulled = true;
	if (_dirtyBits != Dirty_None)
	{
		_dirtyBits &= ~Dirty_Matrix;
		_pvMatrix = getPMatrix() * getVMatrix();
	}
	return _pvMatrix;
}


void Camera::notify(Event event, void* sender)
{
	switch (event)
	{
		case Event_TransformChanged:
			dirty(Dirty_View);
			break;
	}
}


void Camera::dirty(char dirtyBits)
{
	_matrixPulled = false;
	_dirtyBits |= dirtyBits | Dirty_Matrix;
}