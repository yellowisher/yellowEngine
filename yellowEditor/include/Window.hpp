#ifndef __H_WINDOW__
#define __H_WINDOW__

struct GLFWwindow;

namespace yellowEditor
{
	struct Window
	{
		Window(int width, int height, const char* name);

		GLFWwindow* handle;
		int width;
		int height;
	};
}

#endif