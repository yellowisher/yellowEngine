#ifndef __H_AABB__
#define __H_AABB__

#include "yellowEngine/Math/Vector3.hpp"

namespace yellowEngine
{
	class AABB
	{
		friend class Collider;
	public:
		static AABB combine(const AABB& a, const AABB& b);

		AABB();
		AABB(Vector3 min, Vector3 max);
		AABB(const AABB& copy);
		~AABB();

		AABB& operator=(const AABB& other);

		bool isCollideWith(const AABB& other);
		bool contains(const AABB& other);

		float perimeter();
		float volume();
		AABB& expand(float factor);

		Vector3 min;
		Vector3 max;
		const Vector3& operator[](int i) const
		{
			if (i == 0)return min;
			return max;
		}

	private:
	};
}

#endif