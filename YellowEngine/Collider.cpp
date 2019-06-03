#include "ColliderManager.hpp"
#include "VertexLayout.hpp"
#include "Transform.hpp"
#include "Collider.hpp"


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
