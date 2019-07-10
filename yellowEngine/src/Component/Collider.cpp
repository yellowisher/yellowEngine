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

		ColliderManager::getInstance()->colliderCreated(this);
	}


	Collider::~Collider()
	{
		ColliderManager::getInstance()->colliderDestroyed(this);
	}


	void Collider::onCreate()
	{
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