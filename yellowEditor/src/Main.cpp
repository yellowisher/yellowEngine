#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_user.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <yellowEngine/yellowEngine.hpp>

#include "EditorUtils.hpp"
#include "Inspector.hpp"


using namespace yellowEngine;

void MainMenuBar();
void SceneWindow();
void HierarchyWindow();
void HierarchyNode(Transform* target);
void AssetWindow();
void InsepctorWindow();

#pragma region Editor Properties

int mainWindowWidth = 1440;
int mainWindowHeight = 900;

const int sceneWindowWidth = 800;
const int sceneWindowHeight = 600;

GLuint sceneTexture;
GLuint sceneData[sceneWindowWidth * sceneWindowHeight * 3];
GLuint sceneDataFlip[sceneWindowWidth * sceneWindowHeight * 3];

#pragma endregion

ImGuiWindowFlags baseFlag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

int main()
{
	if (!glfwInit()) return 1;

	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* mainWindow = glfwCreateWindow(mainWindowWidth, mainWindowHeight, "yellowEditor", NULL, NULL);
	glfwMakeContextCurrent(mainWindow);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD initialization failed!" << std::endl;
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic;

	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// main window initialize
	glClearColor(255.0f / 255.0f, 225.0f / 255.0f, 144.0f / 255.0f, 1.0f);
	glGenTextures(1, &sceneTexture);
	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sceneWindowWidth, sceneWindowHeight, 0, GL_RGB, GL_UNSIGNED_INT, nullptr);

	// scene window initialize
	GLFWwindow* sceneWindow = glfwCreateWindow(sceneWindowWidth, sceneWindowHeight, "sceneWindow", NULL, NULL);
	glfwMakeContextCurrent(sceneWindow);
	glfwHideWindow(sceneWindow);

	Game* game = new Game(sceneWindowWidth, sceneWindowHeight);
	game->init();

	// scene loading code should be here
	Model* model = Model::create("Mesh/nanosuit/nanosuit.obj");

	GameObject* nano = model->instantiate("Model");
	GameObject* spotLightGo = new GameObject();
	Light* sl = spotLightGo->addComponent<Light>()->setType(Light::LightType_Spot);
	sl->diffuseIntensity = 1.0f;
	sl->linear = 0.07f;
	sl->quadratic = 0.002f;
	spotLightGo->transform->setPosition(0, 10, 5);

	auto parent1 = new GameObject("Parent1");
	auto parent2 = new GameObject("Parent2");

	auto child1 = new GameObject("Child1");
	auto child2 = new GameObject("Child2");
	auto child3 = new GameObject("Child1");

	parent1->transform->addChild(child1->transform);
	parent1->transform->addChild(child2->transform);
	parent1->transform->addChild(child3->transform);


	GameObject* dirLight = new GameObject();
	dirLight->addComponent<Light>()->setType(Light::LightType_Dir)->diffuseIntensity = 0.8f;
	dirLight->transform->setRotation(45, 0, 0);

	GameObject* cameraGo = new GameObject();
	Camera* camera = cameraGo->addComponent<Camera>();
	camera->setPerspective(60.0f, 0.01f, 1000.0f);
	camera->transform->setPosition(0, 10, 5);
	//

	// ImGui style
	ImGui::GetStyle().WindowRounding = 0.0f;
	//

	while (!glfwWindowShouldClose(mainWindow))
	{
		// update scene window first
		glfwMakeContextCurrent(sceneWindow);
		{
			game->update();
			game->render(Camera::getMainCamera());

			glfwSwapBuffers(sceneWindow);
			glReadPixels(0, 0, sceneWindowWidth, sceneWindowHeight, GL_RGB, GL_UNSIGNED_INT, sceneData);
		}

		// flip y axis of scene data
		for (int y = 0; y < sceneWindowHeight; y++)
		{
			memcpy(
				sceneDataFlip + (y * sceneWindowWidth * 3), 
				sceneData	  + ((sceneWindowHeight - 1 - y) * sceneWindowWidth * 3), 
				sceneWindowWidth * 3 * sizeof(GLuint));
		}

		// then update main window
		glfwMakeContextCurrent(mainWindow);
		{
			glfwPollEvents();
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			MainMenuBar();
			SceneWindow();
			HierarchyWindow();
			InsepctorWindow();
			AssetWindow();

			// Rendering
			ImGui::Render();
			glfwGetFramebufferSize(mainWindow, &mainWindowWidth, &mainWindowHeight);
			glViewport(0, 0, mainWindowWidth, mainWindowHeight);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(mainWindow);
		}
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(mainWindow);
	glfwTerminate();

	return 0;
}


void MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) { }
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}
			ImGui::Separator();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) { std::cout << "!"; }
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


void SceneWindow()
{
	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sceneWindowWidth, sceneWindowHeight, 0, GL_RGB, GL_UNSIGNED_INT, sceneDataFlip);

	ImGuiWindowFlags flags = baseFlag;
	flags |= ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("Scene", nullptr, flags))
	{
		ImGui::Image((void*)sceneTexture, ImVec2(sceneWindowWidth, sceneWindowHeight));
	}
	ImGui::End();
}


void HierarchyWindow()
{
	if (ImGui::Begin("Hierarchy", nullptr, baseFlag))
	{
		for (auto child : Transform::Root->getChildren())
		{
			HierarchyNode(child);
		}
	}
	ImGui::End();
}


static Transform* selectedNode = nullptr;
void HierarchyNode(Transform* target)
{
	ImGuiTreeNodeFlags flags = 0;
	flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	if (target->getChildCount() == 0)flags |= ImGuiTreeNodeFlags_Leaf;
	if (target == selectedNode) flags |= ImGuiTreeNodeFlags_Selected;
	flags |= ImGuiTreeNodeFlags_AllowItemOverlap;

	std::string id = target->gameObject->getName() + "##" + std::to_string((size_t)target);
	bool nodeOpend = ImGui::TreeNodeEx(id.c_str(), flags);

	if (ImGui::IsItemClicked())
	{
		selectedNode = target;
	}

	if (nodeOpend)
	{
		for (auto child : target->getChildren())
		{
			HierarchyNode(child);
		}
		ImGui::TreePop();
	}
}


void InsepctorWindow()
{
	static GameObject* nameChangingObject = nullptr;
	static const int buffSize = 64;
	static char buff[buffSize] = "";
	static char compBuff[buffSize] = "";
	static std::string searchWord = "";

	if (ImGui::Begin("Inspector", nullptr, baseFlag))
	{
		if (selectedNode != nullptr)
		{
			// name editing
			std::string objectName = selectedNode->gameObject->getName();
			if (nameChangingObject)
			{
				if (nameChangingObject == selectedNode->gameObject)
				{
					if (ImGui::InputText("", buff, buffSize, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					{
						nameChangingObject->setName(buff);
						nameChangingObject = nullptr;
					}

					if (!ImGui::IsItemActive() && (ImGui::IsMouseClicked(0) || ImGui::IsMouseDragging()))
					{
						nameChangingObject = nullptr;
					}
				}
				else
				{
					ImGui::LabelText("", objectName.c_str());
					nameChangingObject = nullptr;
				}
			}
			else
			{
				ImGui::LabelText("", objectName.c_str());
				if (ImGui::IsItemClicked())
				{
					nameChangingObject = selectedNode->gameObject;
					memset(buff, 0, buffSize);
					memcpy(buff, objectName.c_str(), objectName.size());
				}
			}

			// components
			ImGui::Separator();
			for (auto component : selectedNode->gameObject->getComponents())
			{
				yellowEditor::InspectComponent(component);
			}

			// add component
			ImGui::Separator();
			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Components");
				searchWord = "";
			}
			if (ImGui::BeginPopup("Components"))
			{
				if (ImGui::InputText("##Search", compBuff, buffSize))
				{
					searchWord = compBuff;
				}

				for (auto componentName : Component::getComponents())
				{
					if (componentName == "Transform")continue;

					if (componentName.find(searchWord) != -1)
					{
						if (ImGui::Button(componentName.c_str()))
						{
							selectedNode->gameObject->addComponent(componentName);
							ImGui::CloseCurrentPopup();
							break;
						}
					}
				}
				ImGui::EndPopup();
			}
		}
	}
	ImGui::End();
}


void AssetWindow()
{
	if (ImGui::Begin("Asset", nullptr, baseFlag))
	{

	}
	ImGui::End();
}