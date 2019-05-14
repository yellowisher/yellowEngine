#include <iostream>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.hpp"

map<string, Texture*> Texture::textures;

Texture::Texture()
{
}


Texture::~Texture()
{

}


Texture* Texture::create(const char* path)
{
	auto it = textures.find(path);
	if (it != textures.end())
	{
		return it->second;
	}

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

	if (!data)
	{
		cout << "Cannot read file " << path << endl;
		return nullptr;
	}

	Texture* texture = new Texture();
	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	// TODO: should be parameters(customizable)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	// lazy initialization?
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	return texture;
}


void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, id);
}