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
		static Transform*& getSelectedTransform();
		static GLuint getSceneTexture();	
		static Camera* getEditorCamera();
		static void createEditorCamera();

		static const char* editorCameraName;

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
		Transform* _selectedTransform;
		Camera* _editorCamera;
	};
}

#endif