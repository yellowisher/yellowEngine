#include <iostream>
#include <fstream>
#include <json/json.h>

#include "Window_Asset.hpp"
#include "Window_Hierarchy.hpp"
#include "Window_Inspector.hpp"
#include "Window_Scene.hpp"
#include "EditorUtils.hpp"
#include "Editor.hpp"
#include "yellowEngine/Rendering/Technique/Technique_Deferred.hpp"


namespace yellowEditor
{
	extern float scrollY;

	bool Editor::drawGizmo = true;
	bool Editor::drawAllColliders = false;
	bool Editor::drawGBuffer = false;
	bool Editor::showEditorCamera = false;

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
		//_baseWindowFlag = ImGuiWindowFlags_NoMove;
		_baseWindowFlag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

		ImGui_ImplGlfw_InitForOpenGL(editorWindow.handle, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// init other
		glfwSetScrollCallback(editorWindow.handle, glfwScrollCallback);
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
		GameObject* gameObject = GameObject::find(editorCameraName);
		if (gameObject == nullptr)
		{
			gameObject = new GameObject(editorCameraName);
			__instance->_editorCamera = gameObject->addComponent<Camera>();
			__instance->_editorCamera->transform->setPosition(0, 2, 5);
		}
		else
		{
			__instance->_editorCamera = gameObject->getComponent<Camera>();
		}
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
			if (ImGui::BeginMenu("Project", "Ctrl+P"))
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
				if (ImGui::MenuItem("Open", "Ctrl+E"))
				{
					std::string path = fileDialog({ { "yep","Yellow Engine Project" } }, false);
					Editor::getProjectRoot() = path.substr(0, path.find_last_of("\\"));
					Editor::getAssetPath() = Editor::getProjectRoot() + "\\Asset";
					
					LoadAsset(true);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					Editor::setGameContext();

					Editor::selectHierarchyItem(nullptr);
					SceneManager::clearScene();
					Editor::createEditorCamera();
					Editor::setWelcomeScene();

					Editor::setEditorContext();

				}
				if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
				{
					std::string path = fileDialog({ { "yes","Yellow Engine Scene" } }, false);
					Editor::setGameContext();

					SceneManager::loadScene(path.c_str());
					Editor::createEditorCamera();

					Editor::setEditorContext();
				}
				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					std::string path = fileDialog({ { "yes","Yellow Engine Scene" } }, true);
					if (_strcmpi(path.c_str() + path.size() - 3, "yes") != 0)
					{
						SceneManager::saveScene(path.append(".yes").c_str(), Editor::getEditorCamera()->transform);
					}
					LoadAsset();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Display"))
			{
				if (ImGui::Checkbox("Draw Gizmo", &Editor::drawGizmo))
				{
				}
				if (ImGui::Checkbox("Draw Colliders", &Editor::drawAllColliders))
				{
				}
				if (ImGui::Checkbox("Draw G Buffer", &Technique_Deferred::drawGBuffer))
				{
				}
				if (ImGui::Checkbox("Show editor Camera", &Editor::showEditorCamera))
				{
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Rendering"))
			{
				if (ImGui::Checkbox("Apply Bloom", &Technique_Deferred::applyBloom))
				{
				}

				if (ImGui::BeginMenu("Tone mapping"))
				{
					if (ImGui::DragFloat("Exposure", &Technique_Deferred::exposure, 0.05f));
					ImGui::EndMenu();
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


	void Editor::setWelcomeScene()
	{
		GameObject* light = new GameObject("Light");
		light->addComponent<Light>();
		light->transform->setPosition(0, 10, 3);
		light->transform->setRotation(-45, -45, 0);

		GameObject* cube = new GameObject("Cube");
		cube->addComponent<MeshRenderer>()->set(Mesh::create("./res/Mesh/cube.obj:cube"));
		cube->transform->setScale(100.0f, 1.0f, 100.0f);
	}
}