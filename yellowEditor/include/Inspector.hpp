#ifndef __H_INSPECTOR__
#define __H_INSPECTOR__

#include <yellowEngine/Math/Vector2.hpp>
#include <yellowEngine/Math/Vector3.hpp>
#include <yellowEngine/Math/Vector4.hpp>
#include <yellowEngine/Math/Quaternion.hpp>
#include <yellowEngine/Rendering/Mesh.hpp>
#include <yellowEngine/Rendering/Material.hpp>
#include <yellowEngine/Component/Component.hpp>

namespace yellowEngine
{
	void Inspector(Component* component);

	static void Property(int value);
	static void Property(float value);
	static void Property(Vector2 value);
	static void Property(Vector3 value);
	static void Property(Vector4 value);
	static void Property(Mesh* value);
	static void Property(Material* value);
}

#endif