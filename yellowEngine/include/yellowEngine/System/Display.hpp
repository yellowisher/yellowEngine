#ifndef __H_DISPLAY__
#define __H_DISPLAY__

namespace yellowEngine
{
	class Display
	{
		friend class Game;

	public:
		static const int& width;
		static const int& height;
		static const float& aspectRatio;

	private:
		static int _width;
		static int _height;
		static float _aspectRatio;
	};
}

#endif