#ifndef __H_TEXTURE__
#define __H_TEXTURE__

using namespace std;

#include <string>
#include <map>
#include <glad/glad.h>

namespace yellowEngine
{
	class Texture
	{
		friend class FrameBuffer;
	public:
		static Texture* create(const char* path, bool absolute = false, int wrap = GL_REPEAT, int filter = GL_NEAREST);

		void bind();
		void unbind();

	private:
		static map<string, Texture*> __textureCache;

		Texture(int width, int height, int format,  GLenum type, int internalFromat,
				int wrap = GL_REPEAT, int filter = GL_NEAREST, bool generateMipMap = false, const void* data = nullptr);
		~Texture();
		
		unsigned int _id;
	};
}

#endif