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
		{"bool", property_bool},
		{"int", property_int},
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
				ImGui::PushID(prop.name.c_str());
				drawPropertyBase(prop.name.c_str());
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
					ImGui::PushID(prop.name.c_str());
					drawPropertyBase(prop.name.c_str());
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

	void InspectMaterial(Material* material)
	{
		std::string mat = "u_Material.";
		bool valueChanged = false;
		for (auto namePropPair : material->getProperties())
		{
			std::string prettyName = namePropPair.first.substr(mat.length());
			ImGui::PushID(namePropPair.first.c_str());
			drawPropertyBase(prettyName.c_str());
			switch (Material::stringToPrimitive[namePropPair.second.type])
			{
				case Material::Primitive_Float:
				{
					static const char* names[] = { "" };
					float value = namePropPair.second.floatValue;
					if (ImGui::DragFloatNWithLabel(names, &value, 1, 0.05f))
					{
						material->setProperty(namePropPair.first.c_str(), value);
						valueChanged = true;
					}
					break;
				}
				case Material::Primitive_Color:
				{
					Vector3 color = namePropPair.second.colorValue;
					if (ImGui::ColorEdit3("", color.v))
					{
						material->setProperty(namePropPair.first.c_str(), color);
						valueChanged = true;
					}
					break;
				}
			}
			ImGui::PopID();
		}

		for (auto nameTexturePair : material->getTextures())
		{
			std::string prettyName = nameTexturePair.first.substr(mat.length());
			ImGui::PushID(nameTexturePair.first.c_str());
			drawPropertyBase(prettyName.c_str());
			ImGui::Button(nameTexturePair.second->getName().c_str());
			ImGui::PopID();
		}

		if (valueChanged)
		{
			Material::saveAsFile(material, material->getPath().c_str());
		}
	}

	static void drawPropertyBase(const char* name, float width)
	{
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * width);
		ImGui::LabelText("", name); ImGui::SameLine(0, SpacingForLabel);
	}


	static bool property_bool(Component* comp, Component::Property prop)
	{
		return ImGui::Checkbox("", (bool*)(((size_t)comp) + prop.offset));
	}


	bool property_int(Component* comp, Component::Property prop)
	{
		return ImGui::DragInt("", (int*)(((size_t)comp) + prop.offset));
	}


	static bool property_float(Component* comp, Component::Property prop)
	{
		return ImGui::DragFloat("", (float*)(((size_t)comp) + prop.offset), 1, 0.05f);
	}


	static bool property_Vector2(Component* comp, Component::Property prop)
	{
		static const char* names[] = { "X","Y" };
		return ImGui::DragFloatNWithLabel(names, (float*)(((size_t)comp) + prop.offset), 2, 0.05f);
	}


	static bool property_Vector3(Component* comp, Component::Property prop)
	{
		if (prop.name.find("Color") != -1)
		{
			return ImGui::ColorEdit3("", (float*)(((size_t)comp) + prop.offset));
		}
		else
		{
			static const char* names[] = { "X","Y","Z" };
			return ImGui::DragFloatNWithLabel(names, (float*)(((size_t)comp) + prop.offset), 3, 0.05f);
		}
	}


	static bool property_Vector4(Component* comp, Component::Property prop)
	{
		return false;
	}


	static bool property_Mesh(Component* comp, Component::Property prop)
	{
		std::string* meshPath = (std::string*)((size_t)comp + prop.offset);
		ImGui::Button(meshPath->c_str());
		bool changed = false;

		if (ImGui::BeginDragDropTarget())
		{
			auto payload = ImGui::AcceptDragDropPayload("Asset");
			{
				if (payload != nullptr)
				{
					char* path = (char*)payload->Data;
					if (strstr(path, ".obj") != nullptr)
					{
						if (Mesh::create(path) != nullptr)
						{
							*meshPath = path;
								changed = true;
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
		return changed;
	}


	static bool property_Material(Component* comp, Component::Property prop)
	{
		std::string* materialPath = (std::string*)((size_t)comp + prop.offset);
		ImGui::Button(materialPath->c_str());
		bool changed = false;

		if (ImGui::BeginDragDropTarget())
		{
			auto payload = ImGui::AcceptDragDropPayload("Asset");
			{
				if (payload != nullptr)
				{
					char* path = (char*)payload->Data;
					if (strstr(path, ".yem") != nullptr)
					{
						if (Material::create(path) != nullptr)
						{
							*materialPath = path;
							changed = true;
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
		return changed;
	}
}