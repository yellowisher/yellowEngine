#ifndef __H_CUBEMAP__
#define __H_CUBEMAP__

#include <vector>

#include "yellowEngine/Rendering/Texture.hpp"

namespace yellowEngine
{
	class CubeMap
	{
		friend class FrameBuffer;
	public:
		CubeMap(int internalFormat, int width, int format, int type);
		CubeMap(int internalFormat, int width, int format, int type, std::vector<Texture*> textures);
		~CubeMap();

		void bind();

	private:
		unsigned int _cubeMapHandle;
	};
}

#endif