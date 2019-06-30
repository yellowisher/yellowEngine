#ifndef __H_SCENEMANAGER__
#define __H_SCENEMANAGER__

namespace yellowEngine
{
	class SceneManager
	{
	public:
		static void loadScene(const char* path);
		static void saveScene(const char* path);
	};
}

#endif