//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/System/Game.hpp"
#include "yellowEngine/Rendering/Texture.hpp"


namespace yellowEngine
{
	map<string, Texture*> Texture::__textureCache;


	Texture::Texture(int internalFormat, int width, int height, int format, GLenum type,
					 int wrap, int filter, bool generateMipMap, const void* data)
	{
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);

		// TODO: should be parameters(customizable)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		Utils::printGLError("Texture.cpp", 27);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
		Utils::printGLError("Texture.cpp", 29);

		// lazy initialization?
		if (generateMipMap) glGenerateMipmap(GL_TEXTURE_2D);
	}


	Texture::~Texture()
	{
	}


	Texture* Texture::create(const char* path, bool absolute, int wrap, int filter)
	{
		std::string fullpath = path;
		if (!absolute) fullpath = Game::getAssetPath(path);

		auto it = __textureCache.find(fullpath);
		if (it != __textureCache.end())
		{
			return it->second;
		}

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load(fullpath.c_str(), &width, &height, &channels, 0);

		if (!data)
		{
			Utils::print(("Cannot read file " + fullpath).c_str());
			return nullptr;
		}

		GLenum format;
		if (channels == 1)format = GL_RED;
		else if (channels == 3)format = GL_RGB;
		else if (channels == 4)format = GL_RGBA;

		Texture* texture = new Texture(format, width, height, format, GL_UNSIGNED_BYTE, wrap, filter, true, data);
		stbi_image_free(data);

		__textureCache.insert({ fullpath, texture });
		return texture;
	}


	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
}