#ifndef __H_TRANSFORM__
#define __H_TRANSFORM__

#include <list>

#include "Component.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"
#include "Event.hpp"
#include "EventNotifier.hpp"

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

	void addChild(Transform* child);
	void removeChild(Transform* child);

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

	const Matrix& getTRMatrix();
	const Matrix& getInverseTRMatrix();
	const Matrix& getSMatrix();
	const Matrix& getMatrix();

	const Vector3 getWorldPosition();
	const Vector3 getWorldRotation();
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
	std::list<Transform*> _children;

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

#endif