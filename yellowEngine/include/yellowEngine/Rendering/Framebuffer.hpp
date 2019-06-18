#ifndef __H_FRAMEBUFFER__
#define __H_FRAMEBUFFER__

#include <vector>
#include <glad/glad.h>

#include "yellowEngine/Rendering/Texture.hpp"

namespace yellowEngine
{
	class FrameBuffer
	{
	public:
		FrameBuffer(int textureCount, int width, int height, int format, GLenum type, int internalForamt, int addition = NULL);
		~FrameBuffer();

		void bindForWriting();
		void bindForReading();

	protected:
		unsigned int _frameBufferHandle;
		std::vector<Texture*> _textures;
	};
}

#endif