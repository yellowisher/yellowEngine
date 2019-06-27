#include <imgui.h>
#include <imgui_user.h>
#include "Inspector.hpp"


namespace yellowEngine
{
	static const float SpacingForLabel = 5.0f;

	void Inspector(Component* component)
	{
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position"); ImGui::SameLine(0, spacing);
			const char* names[] = { "X","Y","Z" };
			ImGui::DragFloatNWithLabel(names, (float*)&selectedNode->position, 3, 0.05f);
			//ImGui::PushItemWidth(-1);
			//ImGui::DragFloat3("", (float*)&selectedNode->position, 0.05f);
			//ImGui::PopItemWidth();
		}
	}

	static void Property(int value);
	static void Property(float value);
	static void Property(Vector2 value);
	static void Property(Vector3 value);
	static void Property(Vector4 value);
	static void Property(Mesh* value);
	static void Property(Material* value);
}