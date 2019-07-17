#ifndef __H_COLLIDER__
#define __H_COLLIDER__

#include <vector>

#include "yellowEngine/Component/Component.hpp"
#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/System/Event.hpp"
#include "yellowEngine/System/EventListener.hpp"
#include "yellowEngine/Rendering/Mesh.hpp"
#include "yellowEngine/Math/AABB.hpp"

namespace yellowEngine
{
	class ColliderManager;

	class Collider : public Component, public INotifiable
	{
	public:
		enum Type
		{
			Type_None = -1,
			Type_Box,
			Type_Sphere
		};

		Collider(GameObject* gameObject);
		virtual ~Collider();

		virtual bool isCollideWith(Collider* other) = 0;
		virtual Type getType() = 0;
		virtual AABB getBoundingBox() = 0;

		// for passing rendering data
		virtual void fillRenderingPoints(std::vector<Vector3>& lines) = 0;

		virtual void onCreate() override;
		virtual void onDestroy() override;
		virtual void onActive(bool active) override;

		int collidingCount;

	protected:
		EventListener _transformChangeListener;

		virtual void onTransformChange() = 0;
		virtual void initSize(const AABB& aabb) = 0;

	private:
		void notify(Event event, void* sender) override;
	};
}

#endif