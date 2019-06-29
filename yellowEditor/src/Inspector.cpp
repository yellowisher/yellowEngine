#include <map>
#include <string>
#include <imgui.h>
#include <imgui_user.h>
#include "Inspector.hpp"


using namespace yellowEngine;
namespace yellowEditor
{
	static const float SpacingForLabel = 5.0f;
	static std::map<std::string, bool(*)(Component*, Component::Property)> handlers = {
		{"float", property_float},
		{"Vector2", property_Vector2 },
		{"Vector3", property_Vector3},
		{"Vector4", property_Vector4},
		{"Mesh", property_Mesh},
		{"Material", property_Material}
	};

	void InspectComponent(yellowEngine::Component* component)
	{
		const auto& properties = yellowEngine::Component::getProperties()[component->getTypeName()];
		for (auto prop : properties)
		{
			bool valueChanged = false;
			auto it = handlers.find(prop.type);
			if (it != handlers.end())
			{
				// we know about the type
				drawPropertyBase(prop);
				valueChanged = handlers[prop.type](component, prop);
				ImGui::PopID();
			}
			else
			{
				// we don't know about the type
				// check if matching enums exists
				const auto& enums = Component::getEnums()[component->getTypeName()];
				auto it = enums.find(prop.type);
				if (it != enums.end())
				{
					const auto& values = it->second;
					drawPropertyBase(prop);
					int* indexPtr = (int*)(((size_t)component) + prop.offset);

					if (ImGui::Button(values[*indexPtr].c_str())) ImGui::OpenPopup(prop.name.c_str());
					if (ImGui::BeginPopup(prop.name.c_str()))
					{
						for (int i = 0; i < values.size(); i++)
						{
							if (ImGui::Selectable(values[i].c_str()))
							{
								*indexPtr = i;
								valueChanged = true;
							}
						}
						ImGui::EndPopup();
					}
					ImGui::PopID();
				}
			}
			if (valueChanged) component->onValueChanged();
		}

	}

	static void drawPropertyBase(Component::Property prop)
	{
		ImGui::PushID(prop.name.c_str());
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.3f);
		ImGui::LabelText("", prop.name.c_str()); ImGui::SameLine(0, SpacingForLabel);
	}


	static bool property_float(Component* comp, Component::Property prop)
	{
		static const char* names[] = { "" };
		return ImGui::DragFloatNWithLabel(names, (float*)(((size_t)comp) + prop.offset), 1, 0.05f);
	}


	static bool property_Vector2(Component* comp, Component::Property prop)
	{
		static const char* names[] = { "X","Y" };
		return ImGui::DragFloatNWithLabel(names, (float*)(((size_t)comp) + prop.offset), 2, 0.05f);
	}


	static bool property_Vector3(Component* comp, Component::Property prop)
	{
		static const char* names[] = { "X","Y","Z" };
		return ImGui::DragFloatNWithLabel(names, (float*)(((size_t)comp) + prop.offset), 3, 0.05f);
	}


	static bool property_Vector4(Component* comp, Component::Property prop)
	{
		return false;
	}


	static bool property_Mesh(Component* comp, Component::Property prop)
	{
		return false;
	}


	static bool property_Material(Component* comp, Component::Property prop)
	{
		return false;
	}
}