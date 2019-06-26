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
		BEGIN_COMPONENT(Camera)
			PROPERTY(Camera, Type, _type, "Type")
			PROPERTY(Camera, float, _zNear, "Near")
			PROPERTY(Camera, float, _zFar, "Far")
			PROPERTY(Camera, float, _fov, "Field of View")
		END_COMPONENT


	private:
		static Camera* __mainCamera;

	public:
		enum Type
		{
			Type_Perspective,
			Type_Orthographic
		};
		static Camera* getMainCamera();

		Camera(GameObject* gameObject);
		virtual ~Camera();

		void onCreate() override;
		void onDestroy() override;
		void onValueChanged() override;

		void setPerspective(float fov, float zNear, float zFar);
		void setOrthographic(float zNear, float zFar);

		void setZNear(float zNear);
		void setZFar(float zFar);
		void setFov(float fov);
		float getZNear();
		float getZFar();
		float getFov();

		const Matrix& getMatrix();
		void notify(Event event, void* sender) override;

	private:
		enum DirtyBit
		{
			Dirty_None = 0,
			Dirty_View = 1,
			Dirty_Projection = 2,
			Dirty_Matrix = Dirty_View | Dirty_Projection
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

		void dirty(char dirtyBits);
		const Matrix& getPMatrix();
		const Matrix& getVMatrix();
	};
}

#endif