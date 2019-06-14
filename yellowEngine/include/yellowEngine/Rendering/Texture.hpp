#ifndef __H_TEXTURE__
#define __H_TEXTURE__

using namespace std;

#include <string>
#include <map>

namespace yellowEngine
{
	class Texture
	{
	public:
		static Texture* create(const char* path, bool absolute = false);

		void bind();
		void unbind();

		unsigned int _id;

	private:
		static map<string, Texture*> __textureCache;

		Texture();
		~Texture();
	};
}

#endif