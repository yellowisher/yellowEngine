#include "Window_Scene.hpp"


namespace yellowEditor
{
	static Vector3 rotation;
	static float moveSpeed = 0.02f;
	static float moveSpeedZ = 6.0f;
	static float rotateSpeedX = 0.035f;
	static float rotateSpeedY = 0.0175f;
	float scrollY = 0;

	void glfwScrollCallback(GLFWwindow* window, double x, double y)
	{
		scrollY = (float)y;
	}


	void DrawWindow_Scene()
	{
		static bool controllingCamera = false;
		static Vector2 prevMousePosition;
		double x, y;

		ImGuiWindowFlags flags = Editor::getBaseWindowFlag();
		flags |= ImGuiWindowFlags_NoCollapse;

		if (ImGui::Begin("Scene", nullptr, flags))
		{
			ImGui::Image((void*)Editor::getSceneTexture(), ImVec2(Editor::getGameWindow().width, Editor::getGameWindow().height));

			if (ImGui::IsItemClicked(1) || ImGui::IsItemClicked(2))
			{
				glfwSetInputMode(Editor::getEditorWindow().handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				controllingCamera = true;
				glfwGetCursorPos(Editor::getEditorWindow().handle, &x, &y);
				prevMousePosition = Vector2((float)x, (float)y);
			}

			// handle mouse
			if (controllingCamera || ImGui::IsItemHovered())
			{
				glfwGetCursorPos(Editor::getEditorWindow().handle, &x, &y);
				Vector2 newMousePosition = Vector2((float)x, (float)y);
				Vector2 delta = newMousePosition - prevMousePosition;
				Vector3 move;

				if (glfwGetMouseButton(Editor::getEditorWindow().handle, GLFW_MOUSE_BUTTON_RIGHT))
				{
					rotation.x -= delta.y;
					rotation.y -= delta.x;
				}
				if (glfwGetMouseButton(Editor::getEditorWindow().handle, GLFW_MOUSE_BUTTON_MIDDLE))
				{
					move.x = -delta.x;
					move.y = delta.y;
				}
				move.z = (float)scrollY * moveSpeedZ;

				Editor::getEditorCamera()->transform->setRotation(rotation * rotateSpeedX);

				move = move * moveSpeed;
				Vector3 movement = Editor::getEditorCamera()->transform->getForward() * move.z;
				movement += Editor::getEditorCamera()->transform->getRight() * move.x;
				movement += Editor::getEditorCamera()->transform->getUp() * move.y;
				Editor::getEditorCamera()->transform->translate(movement);

				prevMousePosition = newMousePosition;
			}


			if (ImGui::IsMouseReleased(1) || ImGui::IsMouseReleased(2))
			{
				glfwSetInputMode(Editor::getEditorWindow().handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				controllingCamera = false;
			}
		}
		ImGui::End();
	}
}