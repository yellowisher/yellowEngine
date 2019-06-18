#ifndef __H_FRAMEBUFFER__
#define __H_FRAMEBUFFER__

#include <vector>

#include "yellowEngine/Rendering/Texture.hpp"

namespace yellowEngine
{
	class Framebuffer
	{
	public:
		Framebuffer(int textureCount, int width, int height, int format, GLenum type, int internalForamt, int addition = NULL);
		~Framebuffer();

		void bindForWriting();
		void bindForReading();
		void unbind();
		void setBufferToRead(int i);

	protected:
		unsigned int _framebufferHandle;
		std::vector<Texture*> _textures;
	};
}

#endif