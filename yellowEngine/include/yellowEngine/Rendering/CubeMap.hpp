#ifndef __H_CUBEMAP__
#define __H_CUBEMAP__

namespace yellowEngine
{
	class CubeMap
	{
		friend class FrameBuffer;
	public:
		CubeMap(int internalFormat, int width, int format, int type);
		~CubeMap();

		void bind();

	private:
		unsigned int _cubeMapHandle;
	};
}

#endif