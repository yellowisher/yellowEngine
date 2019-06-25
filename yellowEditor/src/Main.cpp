#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <Windows.h>
#include <Commdlg.h>
#include <yellowEngine/yellowEngine.hpp>
#include <iostream>

using namespace yellowEngine;

class CameraScript : IUpdatable
{
public:
	void update() override
	{
		static const float moveSpeed = 0.06f;
		static const float rotateSpeedX = 0.016f;
		static const float rotateSpeedY = 0.008f;
		static Vector3 rotation = Vector3(0, 0, 0);

		// translate
		Vector3 move = Vector3::zero;
		if (InputManager::getKey(GLFW_KEY_A)) move.x -= 1.0f;
		if (InputManager::getKey(GLFW_KEY_D)) move.x += 1.0f;
		if (InputManager::getKey(GLFW_KEY_W)) move.z += 1.0f;
		if (InputManager::getKey(GLFW_KEY_S)) move.z -= 1.0f;
		if (InputManager::getKey(GLFW_KEY_SPACE)) move.y += 1.0f;
		if (InputManager::getKey(GLFW_KEY_LEFT_CONTROL)) move.y -= 1.0f;

		Transform* transform = Camera::getMainCamera()->transform;
		Vector3 forward = transform->getForward();
		forward.y = 0; forward.normalize();

		Vector3 right = transform->getRight();
		right.y = 0; right.normalize();

		Vector3 up = Vector3::up;

		Vector3 movement = Vector3::zero;
		movement += forward * move.z;
		movement += right * move.x;
		movement += up * move.y;

		transform->translate(movement * moveSpeed);

		// rotate
		Vector2 rotate = InputManager::getDeltaMousePosition();
		rotation.x += -rotate.y * rotateSpeedX;
		rotation.y += -rotate.x *rotateSpeedY;

		transform->setRotation(rotation);
	}
};

class LightScript : public IUpdatable, public Component
{
public:
	LightScript(GameObject* gameObject) : Component(gameObject) {}

	void update() override
	{
		Vector3 move = Vector3::zero;
		if (InputManager::getKey(GLFW_KEY_L)) move.x -= 1.0f;
		if (InputManager::getKey(GLFW_KEY_APOSTROPHE)) move.x += 1.0f;
		if (InputManager::getKey(GLFW_KEY_P)) move.z -= 1.0f;
		if (InputManager::getKey(GLFW_KEY_SEMICOLON)) move.z += 1.0f;
		if (InputManager::getKey(GLFW_KEY_I)) move.y += 1.0f;
		if (InputManager::getKey(GLFW_KEY_K)) move.y -= 1.0f;

		transform->translate(move * 0.06f);
	}
};

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


std::string fileDialog(const std::vector<std::pair<std::string, std::string>> &filetypes, bool save);

void mainMenuBar();

int main()
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) return 1;

	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* mainWindow = glfwCreateWindow(1440, 1080, "yellowEditor", NULL, NULL);
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
	//ImGui::StyleColorsClassic();

	ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Our state
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		mainMenuBar();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(mainWindow, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(mainWindow);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(mainWindow);
	glfwTerminate();

	return 0;
}

std::string fileDialog(const std::vector<std::pair<std::string, std::string>> &filetypes, bool save)
{
	static const int FILE_DIALOG_MAX_BUFFER = 16384;

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	wchar_t tmp[FILE_DIALOG_MAX_BUFFER];
	ofn.lpstrFile = tmp;
	ZeroMemory(tmp, sizeof(tmp));
	ofn.nMaxFile = FILE_DIALOG_MAX_BUFFER;
	ofn.nFilterIndex = 1;

	std::string filter;

	if (!save && filetypes.size() > 1)
	{
		filter.append("Supported file types (");
		for (size_t i = 0; i < filetypes.size(); ++i)
		{
			filter.append("*.");
			filter.append(filetypes[i].first);
			if (i + 1 < filetypes.size())
				filter.append(";");
		}
		filter.append(")");
		filter.push_back('\0');
		for (size_t i = 0; i < filetypes.size(); ++i)
		{
			filter.append("*.");
			filter.append(filetypes[i].first);
			if (i + 1 < filetypes.size())
				filter.append(";");
		}
		filter.push_back('\0');
	}
	for (auto pair : filetypes)
	{
		filter.append(pair.second);
		filter.append(" (*.");
		filter.append(pair.first);
		filter.append(")");
		filter.push_back('\0');
		filter.append("*.");
		filter.append(pair.first);
		filter.push_back('\0');
	}
	filter.push_back('\0');

	int size = MultiByteToWideChar(CP_UTF8, 0, &filter[0], (int)filter.size(), NULL, 0);
	std::wstring wfilter(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, &filter[0], (int)filter.size(), &wfilter[0], size);

	ofn.lpstrFilter = wfilter.data();

	if (save)
	{
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		if (GetSaveFileNameW(&ofn) == FALSE)
			return {};
	}
	else
	{
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		/*if (multiple)
			ofn.Flags |= OFN_ALLOWMULTISELECT;*/
		if (GetOpenFileNameW(&ofn) == FALSE)
			return {};
	}

	size_t i = 0;
	std::vector<std::string> result;
	while (tmp[i] != '\0')
	{
		std::string filename;
		int tmpSize = (int)wcslen(&tmp[i]);
		if (tmpSize > 0)
		{
			int filenameSize = WideCharToMultiByte(CP_UTF8, 0, &tmp[i], tmpSize, NULL, 0, NULL, NULL);
			filename.resize(filenameSize, 0);
			WideCharToMultiByte(CP_UTF8, 0, &tmp[i], tmpSize, &filename[0], filenameSize, NULL, NULL);
		}

		result.emplace_back(filename);
		i += tmpSize + 1;
	}

	if (result.size() > 1)
	{
		for (i = 1; i < result.size(); ++i)
		{
			result[i] = result[0] + "\\" + result[i];
		}
		result.erase(begin(result));
	}

	return result.empty() ? "" : result.front();
}


void mainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) { std::cout << fileDialog({ {"yes", "Yellow Editor Scene"} }, false); }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { std::cout << fileDialog({ {"yes", "Yellow Editor Scene"} }, true); }
			if (ImGui::MenuItem("Save As..")) {}
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
