#ifndef __H_COLLIDERMANAGER__
#define __H_COLLIDERMANAGER__

#include <vector>
#include <unordered_map>

#include "yellowEngine/Component/Camera.hpp"
#include "yellowEngine/Component/Collider.hpp"
#include "yellowEngine/Rendering/Renderer.hpp"
#include "yellowEngine/Collision/BroadPhase.hpp"

namespace yellowEngine
{
	class ColliderManager
	{
		friend class BroadPhase_NUL;

	public:
		enum BroadPhaseType
		{
			BroadPhaseType_NUL,
			BroadPhaseType_SAP,
			BroadPhaseType_BVH
		};

		static ColliderManager* create(BroadPhaseType type);
		static ColliderManager* getInstance();

		void detect();
		void colliderCreated(Collider* collider);
		void colliderDestroyed(Collider* collider);
		void colliderUpdated(Collider* collider);
		void renderColliders(Camera* camera);
		void renderCollider(Camera* camera, Collider* collider);

	private:
		enum PairType
		{
			PairType_New,
			PairType_Continuous,
			PairType_Delete
		};

		ColliderManager(BroadPhaseType type);
		~ColliderManager();

		void collisionEnter(const ColliderPair& pair);
		void collisionStay(const ColliderPair& pair);
		void collisionExit(const ColliderPair& pair);

		static ColliderManager* _instance;

		BroadPhaseType _type;
		BroadPhase* _broadPhase;
		std::unordered_map<ColliderPair, PairType> _collidingPairs;
		std::vector<Collider*> _colliders;

		// for rendering colliders
		Shader* _wireFrameShader;
		const Uniform* _colorUniform;
		Renderer _renderer;

		const Vector3 _colliderColor = Vector3(0, 0.8f, 0);
		const Vector3 _boundingBoxColor = Vector3(0.8f, 0.8f, 0);
	};
}
#endif