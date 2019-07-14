#include "yellowEngine/Utility/Utils.hpp"

namespace yellowEngine
{
	namespace Utils
	{
		void printGLError(const char* point)
		{
			GLenum glError = glGetError();
			while (glError != GL_NO_ERROR)
			{
				std::string errorString;
				switch (glError)
				{
					case GL_INVALID_ENUM:
						errorString = "GL_INVALID_ENUM";
						break;
					case GL_INVALID_VALUE:
						errorString = "GL_INVALID_VALUE";
						break;
					case GL_INVALID_OPERATION:
						errorString = "GL_INVALID_OPERATION";
						break;
					case GL_INVALID_FRAMEBUFFER_OPERATION:
						errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
						break;
					case GL_OUT_OF_MEMORY:
						errorString = "GL_OUT_OF_MEMORY";
						break;
					case GL_STACK_UNDERFLOW:
						errorString = "GL_STACK_UNDERFLOW";
						break;
					case GL_STACK_OVERFLOW:
						errorString = "GL_STACK_OVERFLOW";
						break;
				}

				std::cout << point << " " << errorString << std::endl;
				glError = glGetError();
			}
		}

		const char* getExtension(const char* path)
		{
			int cursor = strlen(path) - 1;

			while (path[cursor] != '.')
			{
				cursor--;
			}

			return path + cursor + 1;
		}
	}
}