#include <limits>

namespace Utils
{
	const float pi = 3.1415926535f;
	const float epsilon = 0.00001f;
	const float inf = std::numeric_limits<float>::infinity();

	constexpr float deg2rad(float deg)
	{
		return deg * 0.0174532925f;
	}

	constexpr float rad2deg(float rad)
	{
		return rad * 57.295779513f;
	}

	constexpr float max(float a, float b)
	{
		return a > b ? a : b;
	}

	constexpr float min(float a, float b)
	{
		return a > b ? b : a;
	}
}