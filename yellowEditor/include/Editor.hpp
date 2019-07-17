#ifndef __H_EDITOR__
#define __H_EDITOR__

#include <imgui.h>
#include <imgui_user.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <yellowEngine/yellowEngine.hpp>

#include "Window.hpp"

int main();

using namespace yellowEngine;
namespace yellowEditor
{
	class Editor
	{
		friend int ::main();

	public:
		static ImGuiWindowFlags getBaseWindowFlag();
		static const Window& getEditorWindow();
		static const Window& getGameWindow();
		static Transform* getSelectedTransform();
		static std::string getSelectedAsset();
		static GLuint getSceneTexture();	
		static Camera* getEditorCamera();
		static void createEditorCamera();
		static const char* editorCameraName;

		static std::string& getProjectRoot();
		static std::string& getAssetPath();

		static void selectHierarchyItem(Transform* item);
		static void selectAssetItem(std::string item);

		static void setGameContext();
		static void setEditorContext();
		static void setWelcomeScene();

		static bool drawGizmo;
		static bool drawAllColliders;
		static bool drawGBuffer;

	private:
		static Editor* __instance;

		Editor(Window& editorWindow, Window& gameWindow, GLuint sceneTexture);
		~Editor();
		void update();
		void render();

		ImGuiWindowFlags _baseWindowFlag;
		Window& _editorWindow;
		Window& _gameWindow;
		GLuint _sceneTexture;
		Camera* _editorCamera;
		
		Transform* _selectedTransform;
		std::string _selectedAsset;

		std::string _projectRoot;
		std::string _assetPath;
	};
}

#endif