#include "yellowEngine/Collision/ColliderManager.hpp"
#include "yellowEngine/Rendering/VertexLayout.hpp"
#include "yellowEngine/Component/Transform.hpp"
#include "yellowEngine/Component/Collider.hpp"


namespace yellowEngine
{
	Collider::Collider(GameObject* gameObject) :Component(gameObject)
	{
	}


	Collider::~Collider()
	{
	}


	void Collider::onCreate()
	{
		_transformChangeListener.setParent(this);
		transform->transformChangeNotifier.addListener(&_transformChangeListener);

		ColliderManager::getInstance()->colliderCreated(this);
	}


	void Collider::onDestroy()
	{
		ColliderManager::getInstance()->colliderDestroyed(this);
	}


	void Collider::notify(Event event, void* sender)
	{
		switch (event)
		{
		case Event_TransformChanged:
			onTransformChange();
			ColliderManager::getInstance()->colliderUpdated(this);
			break;
		}
	}
}