#include "Camera.hpp"
#include "System.hpp"
#include "GameObject.hpp"

Camera::Camera(GameObject* gameObject) :Component(gameObject), _vMatrix(Matrix::identity)
{
	setPerspective(45.0f, 0.01f, 100.0f);
	getGameObject()->transform->addListener(this);
}


Camera::~Camera()
{
	if (getGameObject()->transform)getGameObject()->transform->removeListener(this);
}


void Camera::setPerspective(float fieldOfView, float zNear, float zFar)
{
	_type = Perspective;
	setZRange(zNear, zFar);
	_pMatrix = Matrix::createPerspective(fieldOfView, System::getInstance()->getAspectRatio(), zNear, zFar);
	computePVMatrix();
}


void Camera::setOrthographic(float zNear, float zFar)
{
	_type = Orthographic;
	setZRange(zNear, zFar);
	_pMatrix = Matrix::createOrthographic(System::getInstance()->getWidth(), System::getInstance()->getHeight(), zNear, zFar);
	computePVMatrix();
}


void Camera::setZRange(float zNear, float zFar)
{
	_zNear = zNear;
	_zFar = zFar;
}


const Matrix& Camera::getPVMatrix()
{
	return _pvMatrix;
}


void Camera::onTransformChanged(Transform* transform)
{
	_vMatrix = transform->getTRMatrix();
	_vMatrix = ~_vMatrix;
	computePVMatrix();
}


void Camera::computePVMatrix()
{
	_pvMatrix = _pMatrix * _vMatrix;
}