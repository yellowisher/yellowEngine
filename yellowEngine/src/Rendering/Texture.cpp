#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cstring>

#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/System/Game.hpp"
#include "yellowEngine/Rendering/Texture.hpp"


namespace yellowEngine
{
	map<string, Texture*> Texture::__textureCache;


	Texture::Texture(const char* name, int internalFormat, int width, int height, int format, GLenum type,
					 int wrap, int filter, bool generateMipMap, const void* data)
	{
		Utils::printGLError("Texture creating begin");
		_name = name;
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);

		this->width = width;
		this->height = height;

		// lazy initialization?
		if (generateMipMap) glGenerateMipmap(GL_TEXTURE_2D);
		Utils::printGLError("Texture creating end");
	}


	Texture::~Texture()
	{
		glDeleteTextures(1, &_id);
	}


	Texture* Texture::create(const char* path, bool noFlip)
	{
		return create(path, GL_REPEAT, GL_NEAREST, noFlip);
	}


	Texture* Texture::create(const char* path, int wrap, int filter, bool noFlip)
	{
		auto it = __textureCache.find(path);
		if (it != __textureCache.end())
		{
			return it->second;
		}

		int width, height, channels;
		const char* ext = path + strlen(path) - 3;

		if (noFlip || _strcmpi(ext, "tga") == 0)
		{
			stbi_set_flip_vertically_on_load(false);
		}
		else
		{
			stbi_set_flip_vertically_on_load(true);
		}

		unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

		if (!data)
		{
			Utils::print("Cannot read textrue file");
			return nullptr;
		}

		GLenum format;
		if (channels == 1)format = GL_RED;
		else if (channels == 3)format = GL_RGB;
		else if (channels == 4)format = GL_RGBA;

		Texture* texture = new Texture(
			path, format, width, height,
			format, GL_UNSIGNED_BYTE, wrap, filter, true, data);
		stbi_image_free(data);

		__textureCache.insert({ path, texture });
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