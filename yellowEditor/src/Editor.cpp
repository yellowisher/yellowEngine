#include <iostream>
#include <fstream>
#include <json/json.h>

#include "Window_Asset.hpp"
#include "Window_Hierarchy.hpp"
#include "Window_Inspector.hpp"
#include "Window_Scene.hpp"
#include "EditorUtils.hpp"
#include "Editor.hpp"


namespace yellowEditor
{
	extern float scrollY;

	const char* Editor::editorCameraName = "__EDITOR_CAMERA__";
	Editor* Editor::__instance = nullptr;

	static void DrawMainMenuBar();

	Editor::Editor(Window& editorWindow, Window& gameWindow, GLuint sceneTexture)
		:_editorWindow(editorWindow), _gameWindow(gameWindow), _sceneTexture(sceneTexture)
	{
		Init_AssetWindow();

		if (__instance == nullptr)
		{
			__instance = this;
		}
		_selectedTransform = nullptr;
		_selectedAsset = "";

		const char* glsl_version = "#version 330";

		// setting ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		// setting ImGui style
		ImGui::StyleColorsDark();
		ImGui::GetStyle().WindowRounding = 0.0f;
		//_baseWindowFlag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		_baseWindowFlag = 0;

		ImGui_ImplGlfw_InitForOpenGL(editorWindow.handle, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// init other
		glfwSetScrollCallback(editorWindow.handle, glfwScrollCallback);
		//Init_AssetWindow();
	}


	Editor::~Editor()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}


	void Editor::update()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}


	void Editor::render()
	{
		DrawMainMenuBar();
		DrawWindow_Hierarchy();
		DrawWindow_Inspector();
		DrawWindow_Scene();
		DrawWindow_Asset();

		ImGui::Render();
		glfwGetFramebufferSize(_editorWindow.handle, &_editorWindow.width, &_editorWindow.height);
		glViewport(0, 0, _editorWindow.width, _editorWindow.height);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		scrollY = 0;
	}


	ImGuiWindowFlags Editor::getBaseWindowFlag()
	{
		return __instance->_baseWindowFlag;
	}


	const Window& Editor::getEditorWindow()
	{
		return __instance->_editorWindow;
	}


	const Window& Editor::getGameWindow()
	{
		return __instance->_gameWindow;
	}


	Transform* Editor::getSelectedTransform()
	{
		return __instance->_selectedTransform;
	}


	std::string Editor::getSelectedAsset()
	{
		return __instance->_selectedAsset;
	}


	GLuint Editor::getSceneTexture()
	{
		return __instance->_sceneTexture;
	}


	Camera* Editor::getEditorCamera()
	{
		return __instance->_editorCamera;
	}


	void Editor::createEditorCamera()
	{
		GameObject* gameObject = new GameObject(editorCameraName);
		__instance->_editorCamera = gameObject->addComponent<Camera>();
		__instance->_editorCamera->transform->setPosition(0, 0, 5);
	}


	std::string& Editor::getProjectRoot()
	{
		return __instance->_projectRoot;
	}


	std::string& Editor::getAssetPath()
	{
		return __instance->_assetPath;
	}


	void Editor::selectHierarchyItem(Transform* item)
	{
		__instance->_selectedAsset.clear();
		__instance->_selectedTransform = item;
	}


	void Editor::selectAssetItem(std::string item)
	{
		__instance->_selectedTransform = nullptr;
		__instance->_selectedAsset = item;
	}


	void Editor::setGameContext()
	{
		glfwMakeContextCurrent(__instance->_gameWindow.handle);
	}


	void Editor::setEditorContext()
	{
		glfwMakeContextCurrent(__instance->_editorWindow.handle);
	}


	static void DrawMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					std::string path = fileDialog({ { "yep","Yellow Engine Project" } }, true);
					size_t separator = path.find_last_of("\\");

					Editor::getProjectRoot() = path.substr(0, separator);
					Editor::getAssetPath() = Editor::getProjectRoot() + "\\Asset";

					Json::Value root;
					root["name"] = path.substr(separator + 1, path.length() - 1 - separator);

					std::ofstream ofs(path + ".yep", std::ifstream::binary);
					auto writer = Json::StyledStreamWriter();
					writer.write(ofs, root);
					createDirectory(Editor::getProjectRoot() + "\\Asset");

					LoadAsset(true);
				}
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					std::string path = fileDialog({ { "yep","Yellow Engine Project" } }, false);
					Editor::getProjectRoot() = path.substr(0, path.find_last_of("\\"));
					Editor::getAssetPath() = Editor::getProjectRoot() + "\\Asset";
					
					LoadAsset(true);
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
				}
				if (ImGui::MenuItem("Save As..")) {}
				ImGui::Separator();

				if (ImGui::MenuItem("New Scene"))
				{
					std::string path = fileDialog({ { "yes","Yellow Engine Scene" } }, true);
					LoadAsset();
				}
				if (ImGui::MenuItem("Open Scene"))
				{
					std::string path = fileDialog({ { "yes","Yellow Engine Scene" } }, false);
					SceneManager::loadScene(path.c_str());
				}
				if (ImGui::MenuItem("Save Scene"))
				{
					std::string path = fileDialog({ { "yes","Yellow Engine Scene" } }, true);
					SceneManager::saveScene(path.append(".yes").c_str());
					LoadAsset();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}
}