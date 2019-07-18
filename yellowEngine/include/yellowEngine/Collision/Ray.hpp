#ifndef __H_RAY__
#define __H_RAY__

#include "yellowEngine/Math/Vector3.hpp"
#include "yellowEngine/Component/Camera.hpp"
#include "yellowEngine/Component/Collider.hpp"

namespace yellowEngine
{
	class Ray
	{
	public:
		struct CastResult
		{
			Vector3 point;
			Collider* collision;
		};

		static Ray screenPointToRay(Camera* camera, int sx, int sy);

		Ray(Vector3 origin, Vector3 direction);
		~Ray();

		Vector3 pointOnRay(int index, float tagetValue);
		bool cast(CastResult& result);
		bool cast(CastResult& result, Collider* target);

		Vector3 origin;
		Vector3 direction;
	};
}

#endif