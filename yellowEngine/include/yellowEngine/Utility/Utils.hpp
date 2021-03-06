#ifndef __H_UTILS__
#define __H_UTILS__

#include <limits>
#include <string>
#include <cstring>
#include <iostream>

// for debugging code
#include <cassert>

#include <glad/glad.h>

namespace yellowEngine
{
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

		constexpr float max(float a, float b, float c)
		{
			return max(max(a, b), c);
		}

		constexpr int max(int a, int b)
		{
			return a > b ? a : b;
		}

		constexpr float min(float a, float b)
		{
			return a > b ? b : a;
		}

		constexpr int min(int a, int b)
		{
			return a > b ? b : a;
		}

		constexpr float abs(float n)
		{
			return n > 0 ? n : -n;
		}

		constexpr float lerp(float a, float b, float f)
		{
			return (1.0f - f) * a + f * b;
		}

		inline void print(const char* message)
		{
			std::cout << message << std::endl;
		}

		void printGLError(const char* point);
		const char* getExtension(const char* path);
	}
}

#endif