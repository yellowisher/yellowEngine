#ifndef __H_YELLOWENGINE__
#define __H_YELLOWENGINE__

#include "Animation/AnimationClip.hpp"
#include "Animation/Animator.hpp"

#include "Collision/BroadPhase.hpp"
#include "Collision/BroadPhase_BVH.hpp"
#include "Collision/BroadPhase_NUL.hpp"
#include "Collision/BroadPhase_SAP.hpp"
#include "Collision/ColliderManager.hpp"

#include "Component/BoxCollider.hpp"
#include "Component/Camera.hpp"
#include "Component/Collider.hpp"
#include "Component/Component.hpp"
#include "Component/Light.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/ObjectRenderer.hpp"
#include "Component/SphereCollider.hpp"
#include "Component/Transform.hpp"

#include "Math/AABB.hpp"
#include "Math/Matrix.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"

#include "Rendering/Mesh.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/ShaderProgram.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/UniformUpdater.hpp"
#include "Rendering/VertexLayout.hpp"
#include "Rendering/VertexLayoutBinding.hpp"

#include "System/Event.hpp"
#include "System/EventListener.hpp"
#include "System/EventNotifier.hpp"
#include "System/GameObject.hpp"
#include "System/System.hpp"

#include "Utility/ObjectPool.hpp"
#include "Utility/Utils.hpp"

#endif