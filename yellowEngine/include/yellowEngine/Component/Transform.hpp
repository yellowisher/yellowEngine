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
	public:
		const Vector3& position;
		const Vector3& scale;
		const Quaternion& rotation;

		EventNotifier transformChangeNotifier;

		Transform(GameObject* gameObject);
		virtual ~Transform();

		void onCreate() override;
		void onDestroy() override;

		Transform* getParent();
		void addChild(Transform* child);
		void removeChild(Transform* child);
		Transform* findChild(const std::string& name);
		Transform* getChild(int index);
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

		const Matrix& getTRMatrix(Transform* until = nullptr);
		const Matrix& getInverseTRMatrix(Transform* until = nullptr);
		const Matrix& getSMatrix(Transform* until = nullptr);
		const Matrix& getMatrix(Transform* until = nullptr);
		Matrix getLocalMatrix();

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
			Dirty_Inverse_Translation_Rotation = 4,
			Dirty_Scale = 8,
			Dirty_Matrix = 16,
			Dirty_Translation_Rotation = Dirty_Translation | Dirty_Rotation,
			Dirty_All = Dirty_Translation | Dirty_Rotation | Dirty_Inverse_Translation_Rotation | Dirty_Scale | Dirty_Matrix
		};

		Transform* _parent;
		std::vector<Transform*> _children;

		Matrix _matrix;
		Matrix _trMatrix;
		Matrix _itrMatrix;
		Matrix _sMatrix;
		char _dirtyBits;

		Vector3 _position;
		Vector3 _scale;
		Quaternion _rotation;

		void dirty(char bit);
		void transformChanged();
	};
}

#endif