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
		static Texture* create(const char* path, bool noFlip);
		static Texture* create(const char* path, int wrap = GL_REPEAT, int filter = GL_NEAREST, bool noFlip = false);

		void bind();
		void unbind();
		const std::string& getName() { return _name; }

		int width;
		int height;

	private:
		static map<string, Texture*> __textureCache;

		Texture(const char* name, int internalFormat, int width, int height, int format, GLenum type,
				int wrap = GL_REPEAT, int filter = GL_NEAREST, bool generateMipMap = false, const void* data = nullptr);
		~Texture();
		
		unsigned int _id;
		std::string _name;
	};
}

#endif