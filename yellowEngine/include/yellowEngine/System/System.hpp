#ifndef __H_SYSTEM__
#define __H_SYSTEM__

#include <string>

namespace yellowEngine
{
	class System
	{
	public:
		static System* getInstance();

		float getWidth();
		float getHeight();
		void setWidth(float width);
		void setHeight(float height);
		float getAspectRatio();

		void setResourcePath(const char* path);
		const std::string getResourcePath(const char* fileName);

	private:
		static System* _instance;

		std::string _resourcePath;
		float _width;
		float _height;

		System();
		System(const System& copy);
		~System();
	};
}

#endif