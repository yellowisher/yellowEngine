#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Component/GameObject.hpp"
#include "yellowEngine/Component/Camera.hpp"


namespace yellowEngine
{
	COMPONENT_IMPL(Camera)

	Camera* Camera::__mainCamera;


	Camera* Camera::getMainCamera()
	{
		return __mainCamera;
	}


	Camera::Camera(GameObject* gameObject) :
		Component(gameObject)
	{
		if (__mainCamera == nullptr)
		{
			__mainCamera = this;
		}
		setPerspective(60.0f, 0.01f, 100.0f);
	}


	void Camera::onCreate()
	{
		_transformChangeListener.setParent(this);
		transform->transformChangeNotifier.addListener(&_transformChangeListener);
	}


	void Camera::onDestroy()
	{
	}


	void Camera::onValueChanged()
	{
		dirty(Dirty_Matrix);
	}


	Camera::~Camera()
	{
	}


	void Camera::setPerspective(float fov, float zNear, float zFar)
	{
		_type = CameraType_Perspective;
		_fov = fov;
		_zNear = zNear;
		_zFar = zFar;
		dirty(Dirty_Projection);
	}


	void Camera::setOrthographic(float zNear, float zFar)
	{
		_type = CameraType_Orthographic;
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
			if (_type == CameraType_Perspective)
			{
				_pMatrix = Matrix::createPerspective(_fov, Display::aspectRatio, _zNear, _zFar);
			}
			else
			{
				_pMatrix = Matrix::createOrthographic(Display::width, Display::height, _zNear, _zFar);
			}
		}
		return _pMatrix;
	}


	const Matrix& Camera::getVMatrix()
	{
		if (_dirtyBits & Dirty_View)
		{
			_dirtyBits &= ~Dirty_View;
			_vMatrix = transform->getMatrix();
			_vMatrix = ~_vMatrix;
		}
		return _vMatrix;
	}


	const Matrix& Camera::getMatrix()
	{
		if (_dirtyBits & Dirty_Matrix)
		{
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
		_dirtyBits |= dirtyBits;
	}
}