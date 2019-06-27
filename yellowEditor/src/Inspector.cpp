#include <map>
#include <string>
#include <imgui.h>
#include <imgui_user.h>
#include "Inspector.hpp"


using namespace yellowEngine;
namespace yellowEditor
{
	enum PropertyType
	{
		PropertyType_Int,
		PropertyType_Float,
		PropertyType_Vector2,
		PropertyType_Vector3,
		PropertyType_Vector4,
		PropertyType_Quaternion,
		PropertyType_Mesh,
		PropertyType_Material
	};
	static const float SpacingForLabel = 5.0f;
	static std::map<std::string, int> propertyMap = {
		{"int", PropertyType_Int},
		{"float", PropertyType_Float},
		{"Vector2", PropertyType_Vector2},
		{"Vector3", PropertyType_Vector3},
		{"Vector4", PropertyType_Vector4},
		{"Quaternion", PropertyType_Quaternion},
		{"Mesh", PropertyType_Mesh},
		{"Material", PropertyType_Material},
	};

	void InspectComponent(yellowEngine::Component* component)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		ImGui::PushID(component);
		if (ImGui::CollapsingHeader(component->getTypeName(), flags))
		{
			const auto& properties = yellowEngine::Component::getProperties()[component->getTypeName()];
			for (auto prop : properties)
			{
				auto it = propertyMap.find(prop.type);
				if (it != propertyMap.end())
				{
					// if we know about the type
					ImGui::PushID(prop.name.c_str());
					ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.3f);

					ImGui::LabelText("", prop.name.c_str()); ImGui::SameLine(0, SpacingForLabel);

					//ImGui::BulletText(prop.name.c_str()); ImGui::SameLine(0, SpacingForLabel);


					bool valueChanged = false;
					switch (it->second)
					{
						case PropertyType_Int:
						case PropertyType_Float:
						case PropertyType_Vector2:
						case PropertyType_Vector3:
						{
							static const char* names[] = { "X","Y","Z" };
							valueChanged = ImGui::DragFloatNWithLabel(names, (float*)(((size_t)component) + prop.offset), 3, 0.05f);
							break;
						}
						case PropertyType_Vector4:
						case PropertyType_Quaternion:
						{
							static const char* names[] = { "X", "Y", "Z" };
							Quaternion* rotation = (Quaternion*)(((size_t)component) + prop.offset);
							Vector3 value = rotation->toEulerAngle();
							valueChanged = ImGui::DragFloatNWithLabel(names, (float*)&value, 3, 0.05f);
							if (valueChanged)*rotation = Quaternion(value);
							break;
						}

						case PropertyType_Mesh:
						case PropertyType_Material:
							break;
					}

					if (valueChanged) component->onValueChanged();
					ImGui::PopID();
				}
				else
				{
					// we don't know about the type; custom type or enum
				}
			}
		}
		ImGui::PopID();
	}
}