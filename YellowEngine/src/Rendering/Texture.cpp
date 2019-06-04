#include <iostream>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "yellowEngine/Utility/stb_image.h"
#include "yellowEngine/System/System.hpp"
#include "yellowEngine/Rendering/Texture.hpp"

map<string, Texture*> Texture::__textureCache;

Texture::Texture()
{
}


Texture::~Texture()
{

}


Texture* Texture::create(const char* path)
{
	auto it = __textureCache.find(path);
	if (it != __textureCache.end())
	{
		return it->second;
	}

	std::string pathString = System::getInstance()->getResourcePath(path);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(pathString.c_str(), &width, &height, &channels, 0);

	if (!data)
	{
		cout << "Cannot read file " << pathString << endl;
		return nullptr;
	}

	GLenum format;
	if (channels == 1)format = GL_RED;
	else if (channels == 3)format = GL_RGB;
	else if (channels == 4)format = GL_RGBA;

	Texture* texture = new Texture();
	glGenTextures(1, &texture->_id);
	glBindTexture(GL_TEXTURE_2D, texture->_id);

	// TODO: should be parameters(customizable)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	// lazy initialization?
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	__textureCache.insert({ path, texture });
	return texture;
}


void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, _id);
}