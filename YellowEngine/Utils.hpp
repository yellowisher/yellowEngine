namespace Utils
{
	const float pi = 3.1415926535f;
	const float epsilon = 0.00001f;

	constexpr float deg2rad(float deg)
	{
		return deg * 0.0174532925f;
	}

	constexpr float rad2deg(float rad)
	{
		return rad * 57.295779513f;
	}
}