#ifndef __H_INSPECTOR__
#define __H_INSPECTOR__

#include <yellowEngine/Math/Vector2.hpp>
#include <yellowEngine/Math/Vector3.hpp>
#include <yellowEngine/Math/Vector4.hpp>
#include <yellowEngine/Math/Quaternion.hpp>
#include <yellowEngine/Rendering/Mesh.hpp>
#include <yellowEngine/Rendering/Material.hpp>
#include <yellowEngine/Component/Component.hpp>

namespace yellowEditor
{
	void InspectComponent(yellowEngine::Component* component);
}

#endif