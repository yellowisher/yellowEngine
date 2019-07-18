#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Component/BoxCollider.hpp"
#include "yellowEngine/Component/SphereCollider.hpp"
#include "yellowEngine/System/Display.hpp"
#include "yellowEngine/Math/Vector4.hpp"
#include "yellowEngine/Collision/ColliderManager.hpp"
#include "yellowEngine/Collision/Ray.hpp"


namespace yellowEngine
{
	Ray Ray::screenPointToRay(Camera* camera, int sx, int sy)
	{
		Vector4 ndc = Vector4(
			2.0f * (float)sx / (float)Display::width - 1.0f,
			2.0f * (float)sy / (float)Display::height - 1.0f,
			-1.0f,
			1.0f
		);

		Vector4 view = ~camera->getPMatrix() * ndc;
		view.z = -1.0f;
		view.w = 0.0f;

		Vector3 dir = ~camera->getVMatrix() * view;
		dir.normalize();

		return Ray(camera->transform->getWorldPosition(), dir);
	}


	Ray::Ray(Vector3 origin, Vector3 direction) :origin(origin), direction(direction)
	{
	}


	Ray::~Ray()
	{
	}


	Vector3 Ray::pointOnRay(int index, float targetValue)
	{
		float t = (targetValue - origin.v[index]) / direction.v[index];
		return origin + direction * t;
	}


	bool Ray::cast(CastResult& result)
	{
		for (auto collider : ColliderManager::getInstance()->getColliders())
		{
			if (cast(result, collider))
			{
				std::cout << collider->gameObject->getName() << std::endl;
				return true;
			}
		}
		return false;
	}

 
	bool Ray::cast(CastResult& result, Collider* target)
	{
		if (target->getType() == Collider::Type_Sphere)
		{
			SphereCollider* targetSphere = (SphereCollider*)target;
			//float tp = targetSphere->getWorldRadius() + begin - targetSphere->getWorldCenter();
		}
		else if (target->getType() == Collider::Type_Box)
		{
			/*BoxCollider* targetBox = (BoxCollider*)target;

			float tMin = 0.0f;
			float tMax = 100000.0f;

			AABB bb = targetBox->getBoundingBox();
			Vector3 delta = targetBox->transform->getWorldPosition() - begin;
			Matrix model = targetBox->transform->getMatrix();

			for (int v = 0; v < 3; v++)
			{
				Vector3 axis = Vector3(model[0 + v * 4], model[1 + v * 4], model[2 + v * 4]);
				float e = axis * delta;
				float f = direction * axis;

				if (fabsf(f) > 0.001f)
				{
					float t1 = (e + bb.min.v[v]) / f;
					float t2 = (e + bb.max.v[v]) / f;

					if (t1 > t2)
					{
						float w = t1;
						t1 = t2;
						t2 = w;
					}

					if (t2 < tMax) tMax = t2;
					if (t1 > tMin) tMin = t1;

					if (tMax < tMin) return false;
				}
				else
				{
					if (-e + bb.min.v[v] > 0 || -e + bb.max.v[v] < 0) return false;
				}
			}*/

			return true;
		}
	}
}