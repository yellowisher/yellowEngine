#include "yellowEngine/Utility/Utils.hpp"
#include "yellowEngine/Rendering/CubeMap.hpp"

namespace yellowEngine
{
	CubeMap::CubeMap(int internalFormat, int width, int format, int type)
	{
		Utils::printGLError("Cube map creation begin");

		glGenTextures(1, &_cubeMapHandle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapHandle);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, width, 0, format, type, NULL);
		}

		Utils::printGLError("Cube map creation end");
	}


	CubeMap::~CubeMap()
	{
		glDeleteTextures(1, &_cubeMapHandle);
	}


	void CubeMap::bind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapHandle);
	}
}