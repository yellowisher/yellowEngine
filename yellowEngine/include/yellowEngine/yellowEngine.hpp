#ifndef __H_YELLOWENGINE__
#define __H_YELLOWENGINE__

// animation
#include "Animation/AnimationClip.hpp"
#include "Animation/Animator.hpp"

// collision
#include "Collision/BroadPhase.hpp"
#include "Collision/BroadPhase_BVH.hpp"
#include "Collision/BroadPhase_NUL.hpp"
#include "Collision/BroadPhase_SAP.hpp"
#include "Collision/ColliderManager.hpp"

// component
#include "Component/BoxCollider.hpp"
#include "Component/Camera.hpp"
#include "Component/Collider.hpp"
#include "Component/Component.hpp"
#include "Component/GameObject.hpp"
#include "Component/Light.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/GameObject.hpp"
#include "Component/SkinnedMeshRenderer.hpp"
#include "Component/SphereCollider.hpp"
#include "Component/Transform.hpp"

// math
#include "Math/AABB.hpp"
#include "Math/Matrix.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"

// rendering
#include "Rendering/Framebuffer.hpp"
#include "Rendering/Material.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Model.hpp"
#include "Rendering/Renderbuffer.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/UniformUpdater.hpp"
#include "Rendering/VertexLayout.hpp"
#include "Rendering/VertexLayoutBinding.hpp"

// system
#include "System/Event.hpp"
#include "System/EventListener.hpp"
#include "System/EventNotifier.hpp"
#include "System/SceneManager.hpp"
#include "System/InputManager.hpp"
#include "System/Game.hpp"
#include "System/IUpdatable.hpp"

// utility
#include "Utility/ObjectPool.hpp"
#include "Utility/Definitions.hpp"
#include "Utility/Utils.hpp"

#endif