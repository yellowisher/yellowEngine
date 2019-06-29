#ifndef __H_INSPECTOR__
#define __H_INSPECTOR__

#include <yellowEngine/Math/Vector2.hpp>
#include <yellowEngine/Math/Vector3.hpp>
#include <yellowEngine/Math/Vector4.hpp>
#include <yellowEngine/Math/Quaternion.hpp>
#include <yellowEngine/Rendering/Mesh.hpp>
#include <yellowEngine/Rendering/Material.hpp>
#include <yellowEngine/Component/Component.hpp>

using namespace yellowEngine;
namespace yellowEditor
{
	void InspectComponent(yellowEngine::Component* component);

	static void drawPropertyBase(Component::Property prop);
	static bool	property_float(Component* comp, Component::Property prop);
	static bool	property_Vector2(Component* comp, Component::Property prop);
	static bool	property_Vector3(Component* comp, Component::Property prop);
	static bool	property_Vector4(Component* comp, Component::Property prop);
	static bool	property_Mesh(Component* comp, Component::Property prop);
	static bool	property_Material(Component* comp, Component::Property prop);
}

#endif