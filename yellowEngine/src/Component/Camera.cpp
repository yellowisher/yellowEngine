#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Component/GameObject.hpp"
#include "yellowEngine/Component/Camera.hpp"


namespace yellowEngine
{
	COMPONENT_IMPL(Camera)

		
	Camera* Camera::__mainCamera;
	Camera* Camera::currentCamera;


	Camera* Camera::getMainCamera()
	{
		return __mainCamera;
	}


	Camera::Camera(GameObject* gameObject) :
		Component(gameObject), _dirtyBits(Dirty_Matrix)
	{
		if (__mainCamera == nullptr)
		{
			__mainCamera = this;
		}
		else
		{
			_isMainCamera = false;
		}
		setPerspective(60.0f, 0.01f, 1000.0f);
		_zoom = 100.0f;
	}


	void Camera::onCreate()
	{
		_transformChangeListener.setParent(this);
		transform->transformChangeNotifier.addListener(&_transformChangeListener);
	}


	void Camera::onDestroy()
	{
		if (__mainCamera == this) __mainCamera = nullptr;
	}


	void Camera::onValueChanged()
	{
		if (_isMainCamera)
		{
			__mainCamera = this;
		}
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


	float Camera::getZoom()
	{
		return _zoom;
	}


	void Camera::setZoom(float zoom)
	{
		_zoom = zoom;
		dirty(Dirty_Projection);
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
				_pMatrix = Matrix::createOrthographic(
					(float)Display::width / _zoom, 
					(float)Display::height / _zoom, _zNear, _zFar);
			}
		}
		return _pMatrix;
	}


	const Matrix& Camera::getVMatrix()
	{
		if (_dirtyBits & Dirty_View)
		{
			_dirtyBits &= ~Dirty_View;

			_vMatrix = transform->getInverseMatrix();
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