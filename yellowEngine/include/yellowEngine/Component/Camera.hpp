#ifndef __H_CAMERA__
#define __H_CAMERA__

#include "yellowEngine/System/Event.hpp"
#include "yellowEngine/System/EventListener.hpp"
#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Component/Transform.hpp"
#include "yellowEngine/Math/Matrix.hpp"

// TODO: add logic for multi camera rendering

namespace yellowEngine
{
	class Camera : public Component, public INotifiable
	{
		BEGIN_COMPONENT_NO_CTOR(Camera)
			PROPERTY(Camera, bool, _isMainCamera, "Main Camera")
			PROPERTY(Camera, CameraType, _type, "Type")
			PROPERTY(Camera, float, _zNear, "Near")
			PROPERTY(Camera, float, _zFar, "Far")
			PROPERTY(Camera, float, _fov, "Field of View")
			PROPERTY(Camera, float, _zoom, "Zoom")

			BEGIN_ENUM(Camera, CameraType)
				ENUM("Perspective")
				ENUM("Orthographic")
			END_ENUM
		END_COMPONENT


	private:
		static Camera* __mainCamera;

	public:
		enum CameraType
		{
			CameraType_Perspective,
			CameraType_Orthographic
		};
		static Camera* getMainCamera();
		static Camera* currentCamera;

		Camera(GameObject* gameObject);
		virtual ~Camera();

		virtual void onCreate() override;
		virtual void onDestroy() override;
		virtual void onValueChanged() override;

		void setPerspective(float fov, float zNear, float zFar);
		void setOrthographic(float zNear, float zFar);

		void setZNear(float zNear);
		void setZFar(float zFar);
		void setFov(float fov);
		float getZoom();
		void setZoom(float zoom);
		float getZNear();
		float getZFar();
		float getFov();

		const Matrix& getMatrix();
		const Matrix& getPMatrix();
		const Matrix& getVMatrix();
		void notify(Event event, void* sender) override;

	private:
		enum DirtyBit
		{
			Dirty_None = 0,
			Dirty_View = 1,
			Dirty_Projection = 2,
			Dirty_Matrix = Dirty_View | Dirty_Projection
		};
		unsigned char _dirtyBits;

		EventListener _transformChangeListener;

		bool _isMainCamera;
		CameraType _type;
		float _zNear;
		float _zFar;
		float _fov;
		float _zoom;

		Matrix _pMatrix;
		Matrix _vMatrix;
		Matrix _pvMatrix;

		void dirty(char dirtyBits);
	};
}

#endif