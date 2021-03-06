#include "yellowEngine/Collision/ColliderManager.hpp"
#include "yellowEngine/Rendering/VertexLayout.hpp"
#include "yellowEngine/Component/MeshRenderer.hpp"
#include "yellowEngine/Component/SkinnedMeshRenderer.hpp"
#include "yellowEngine/Component/Transform.hpp"
#include "yellowEngine/Component/Collider.hpp"


namespace yellowEngine
{
	Collider::Collider(GameObject* gameObject) :Component(gameObject)
	{
		_transformChangeListener.setParent(this);
		transform->transformChangeNotifier.addListener(&_transformChangeListener);
		collidingCount = 0;
	}


	Collider::~Collider()
	{
	}


	void Collider::onCreate()
	{
		ColliderManager::getInstance()->colliderCreated(this);

		auto meshRenderer = gameObject->getComponent<MeshRenderer>();
		Mesh* mesh;
		if (meshRenderer && (mesh = meshRenderer->getMesh()))
		{
			initSize(mesh->getBounds());
		}
		else
		{
			auto skinnedRenderer = gameObject->getComponent<SkinnedMeshRenderer>();
			if (skinnedRenderer && (mesh = skinnedRenderer->getMesh()))
			{
				initSize(mesh->getBounds());
			}
			else
			{
				initSize(AABB(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));
			}
		}
	}


	void Collider::onDestroy()
	{
		ColliderManager::getInstance()->colliderDestroyed(this);
	}


	void Collider::onActive(bool active)
	{
		if (active) ColliderManager::getInstance()->colliderCreated(this);
		else ColliderManager::getInstance()->colliderDestroyed(this);
	}


	void Collider::notify(Event event, void* sender)
	{
		switch (event)
		{
		case Event_TransformChanged:
			onTransformChange();
			if (getActive()) ColliderManager::getInstance()->colliderUpdated(this);
			break;
		}
	}
}