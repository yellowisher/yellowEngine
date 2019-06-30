#include <imgui.h>

#include "Inspector.hpp"
#include "Window_Inspector.hpp"

namespace yellowEditor
{
	void DrawWindow_Inspector()
	{
		static GameObject* nameChangingObject = nullptr;
		static const int buffSize = 64;
		static char buff[buffSize] = "";
		static char compBuff[buffSize] = "";
		static std::string searchWord = "";
		static bool focus = false;

		if (ImGui::Begin("Inspector", nullptr, Editor::getBaseWindowFlag()))
		{
			if (Editor::getSelectedTransform() != nullptr)
			{
				// name editing
				std::string objectName = Editor::getSelectedTransform()->gameObject->getName();
				if (nameChangingObject)
				{
					if (nameChangingObject == Editor::getSelectedTransform()->gameObject)
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
						nameChangingObject = Editor::getSelectedTransform()->gameObject;
						memset(buff, 0, buffSize);
						memcpy(buff, objectName.c_str(), objectName.size());
					}
				}

				// components
				Component* removeComponent = nullptr;
				ImGui::Separator();
				for (auto component : Editor::getSelectedTransform()->gameObject->getComponents())
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
					ImGui::PushID(component);
					if (ImGui::CollapsingHeader(component->getTypeName(), flags))
					{
						yellowEditor::InspectComponent(component);
						ImGui::Dummy(ImVec2(10, 10));
						ImGui::SameLine(ImGui::GetWindowWidth() - 30);
						if (ImGui::Button("X", ImVec2(20, 20)))
						{
							removeComponent = component;
						}
					}
					ImGui::PopID();
				}
				if (removeComponent) Editor::getSelectedTransform()->gameObject->removeComponent(removeComponent);

				// add component
				ImGui::Separator();
				if (ImGui::Button("Add Component"))
				{
					ImGui::OpenPopup("Components");
					focus = true;
					searchWord = "";
				}

				if (ImGui::BeginPopup("Components", Editor::getBaseWindowFlag()))
				{
					if (focus)
					{
						focus = false;
						ImGui::SetKeyboardFocusHere();
					}
					if (ImGui::InputText("##Search", compBuff, buffSize))
					{
						searchWord = compBuff;
						std::transform(searchWord.begin(), searchWord.end(), searchWord.begin(), ::tolower);
					}

					for (auto componentName : Component::getComponents())
					{
						if (componentName == "Transform")continue;

						std::string lowerName = componentName;
						std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
						if (lowerName.find(searchWord) != -1)
						{
							if (ImGui::Button(componentName.c_str()))
							{
								Editor::getSelectedTransform()->gameObject->addComponent(componentName);
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
}