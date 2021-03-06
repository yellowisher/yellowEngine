#include "yellowEngine/Rendering/VertexLayout.hpp"

#include "yellowEngine/Collision/BroadPhase_NUL.hpp"
#include "yellowEngine/Collision/BroadPhase_SAP.hpp"
#include "yellowEngine/Collision/BroadPhase_BVH.hpp"
#include "yellowEngine/Collision/ColliderManager.hpp"

#include <iostream>
using namespace std;


namespace yellowEngine
{
	static const Vector3 _colliderColor = Vector3(0, 0.8f, 0);
	static const Vector3 _collidingColor = Vector3(0.8, 0, 0);
	static const Vector3 _boundingBoxColor = Vector3(0.8f, 0.8f, 0);

	ColliderManager* ColliderManager::_instance = nullptr;


	ColliderManager::ColliderManager(BroadPhaseType type)
	{
		_type = type;

		if (type == BroadPhaseType_NUL)_broadPhase = new BroadPhase_NUL();
		else if (type == BroadPhaseType_SAP)_broadPhase = new BroadPhase_SAP();
		else if (type == BroadPhaseType_BVH)_broadPhase = new BroadPhase_BVH();

		_wireFrameShader = Shader::create("./res/Shader/wireframe.vert", "./res/Shader/wireframe.frag");
		_colorUniform = _wireFrameShader->getUniform("u_LineColor");
		VertexLayout layout = VertexLayout({ Attr_Position });
		_renderer = Renderer(layout, _wireFrameShader);
	}


	ColliderManager::~ColliderManager()
	{
	}


	// should be called only once
	ColliderManager* ColliderManager::create(BroadPhaseType type)
	{
		if (_instance != nullptr) return nullptr;
		_instance = new ColliderManager(type);
		return _instance;
	}


	ColliderManager* ColliderManager::getInstance()
	{
		return _instance;
	}


	void ColliderManager::colliderCreated(Collider* collider)
	{
		_broadPhase->addObjcet(collider);
		_colliders.push_back(collider);
	}


	void ColliderManager::colliderDestroyed(Collider* collider)
	{
		_broadPhase->removeObject(collider);

		for (auto it = _colliders.begin(); it != _colliders.end(); ++it)
		{
			if (*it == collider)
			{
				_colliders.erase(it);
				break;
			}
		}

		for (auto it = _collidingPairs.begin(); it != _collidingPairs.end();)
		{
			if (it->first.ca == collider || it->first.cb == collider)
			{
				collisionExit(it->first);
				it = _collidingPairs.erase(it);
			}
			else
			{
				++it;
			}
		}
	}


	void ColliderManager::colliderUpdated(Collider* collider)
	{
		_broadPhase->updateObject(collider);
	}


	void ColliderManager::renderColliders(Camera* camera)
	{
		Camera::currentCamera = camera;

		// render colliders
		for (auto collider : _colliders)
		{
			_renderCollider(collider);
		}


		// render bounding boxies
		_wireFrameShader->setUniform(_colorUniform, _boundingBoxColor);
		for (auto collider : _colliders)
		{
			_renderer.setData(collider->getBoundingBox());
			_renderer.render();
		}

		// render other stuff
		_broadPhase->render(_renderer, _wireFrameShader, _colorUniform);
	}


	void ColliderManager::renderCollider(Camera* camera, Collider* collider)
	{
		Camera::currentCamera = camera;

		// render colliders
		_renderCollider(collider);
	}


	void ColliderManager::_renderCollider(Collider* collider)
	{
		if (collider->collidingCount > 0)
		{
			_wireFrameShader->setUniform(_colorUniform, _collidingColor);
		}
		else
		{
			_wireFrameShader->setUniform(_colorUniform, _colliderColor);
		}
		collider->fillRenderingPoints(_renderer.data);
		_renderer.bufferData();
		_renderer.render();
	}


	void ColliderManager::detect()
	{
		auto& potentialPairs = _broadPhase->getPotentialPairs();
		for (auto pair : potentialPairs)
		{
			if (pair.ca->isCollideWith(pair.cb))
			{
				auto it = _collidingPairs.find(pair);
				if (it != _collidingPairs.end())
				{
					// is collide but isn't new
					// OnColliderStay
					it->second = PairType_Continuous;
					collisionStay(pair);
				}
				else
				{
					// is collide and is new; add to collding list
					// OnColliderEnter
					_collidingPairs.insert({ pair, PairType_New });
					collisionEnter(pair);
				}
			}
		}

		for (auto it = _collidingPairs.begin(); it != _collidingPairs.end();)
		{
			if (it->second == PairType_Delete)
			{
				// the pair is not continuous or newly created
				// OnColliderExit
				collisionExit(it->first);
				it = _collidingPairs.erase(it);
			}
			else
			{
				it->second = PairType_Delete;
				++it;
			}
		}
	}


	void ColliderManager::collisionEnter(const ColliderPair& pair)
	{
		pair.ca->gameObject->onCollisionEnter(pair.cb);
		pair.cb->gameObject->onCollisionEnter(pair.ca);
		pair.ca->collidingCount++;
		pair.cb->collidingCount++;
	}


	void ColliderManager::collisionStay(const ColliderPair& pair)
	{
	}


	void ColliderManager::collisionExit(const ColliderPair& pair)
	{
		pair.ca->gameObject->onCollisionExit(pair.cb);
		pair.cb->gameObject->onCollisionExit(pair.ca);
		pair.ca->collidingCount--;
		pair.cb->collidingCount--;
	}
}