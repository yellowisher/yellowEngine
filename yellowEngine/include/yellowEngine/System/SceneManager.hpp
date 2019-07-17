#ifndef __H_SCENEMANAGER__
#define __H_SCENEMANAGER__

namespace yellowEngine
{
	class Transform;
	class SceneManager
	{
	public:
		static void clearScene();
		static void loadScene(const char* path);
		static void saveScene(const char* path, Transform* ignore = nullptr);
	};
}

#endif