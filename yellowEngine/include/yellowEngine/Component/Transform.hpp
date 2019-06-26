#ifndef __H_TRANSFORM__
#define __H_TRANSFORM__

#include <string>
#include <vector>

#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Math/Vector4.hpp"
#include "yellowEngine/Math/Matrix.hpp"
#include "yellowEngine/Math/Quaternion.hpp"
#include "yellowEngine/System/Event.hpp"
#include "yellowEngine/System/EventNotifier.hpp"

namespace yellowEngine
{
	class Transform : public Component
	{
		BEGIN_COMPONENT(Transform)
			PROPERTY(Transform, Vector3, _position, "Position")
			PROPERTY(Transform, Vector3, _scale, "Scale")
			PROPERTY(Transform, Quaternion, _rotation, "Rotation")
		END_COMPONENT

	public:
		static Transform* Root;

		const Vector3& position;
		const Vector3& scale;
		const Quaternion& rotation;

		EventNotifier transformChangeNotifier;

		Transform(GameObject* gameObject);
		virtual ~Transform();

		void onCreate() override;
		void onDestroy() override;
		void onValueChanged() override;

		Transform* getParent();
		void addChild(Transform* child);
		void removeChild(Transform* child);
		Transform* findChild(const std::string& name);
		Transform* getChild(int index);
		const std::vector<Transform*>& getChildren();
		int getChildCount();

		void translate(const Vector3& translation);
		void translate(float x, float y, float z);
		void setPosition(const Vector3& position);
		void setPosition(float x, float y, float z);
		void rotate(float x, float y, float z);
		void rotate(const Vector3& rotation);
		void rotate(const Quaternion& rotation);
		void setRotation(float x, float y, float z);
		void setRotation(const Vector3& rotation);
		void setRotation(const Quaternion& rotation);
		void setScale(const Vector3& scale);
		void setScale(float x, float y, float z);

		const Matrix& getLocalMatrix();
		const Matrix& getParentMatrix();	
		const Matrix& getMatrix();
		const Matrix& getInverseMatrix();
		Matrix getMatrix(Transform* until);

		const Vector3 getWorldPosition();
		const Quaternion getWorldRotation();
		const Vector3 getUp();
		const Vector3 getRight();
		const Vector3 getForward();

	private:
		enum DirtyBit
		{
			Dirty_None = 0,
			Dirty_Translation = 1,
			Dirty_Rotation = 2,
			Dirty_Scale = 4,
			Dirty_LocalMatrix = Dirty_Translation | Dirty_Rotation | Dirty_Scale,
			Dirty_Parent = 8,
			Dirty_Matrix = Dirty_LocalMatrix | Dirty_Parent,
			Dirty_InverseMatrix = 16,
			Dirty_All = (Dirty_InverseMatrix * 2 - 1),
		};

		Transform* _parent;
		std::vector<Transform*> _children;

		Matrix _matrix;
		Matrix _localMatrix;
		Matrix _parentMatrix;
		Matrix _inverseMatrix;
		int _dirtyBits;

		Vector3 _position;
		Vector3 _scale;
		Quaternion _rotation;

		void dirty(int bit);
		void transformChanged();
	};
}

#endif