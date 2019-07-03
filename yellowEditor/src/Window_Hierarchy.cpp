#include "Window_Hierarchy.hpp"


namespace yellowEditor
{
	static void HierarchyNode(Transform* target);


	void DrawWindow_Hierarchy()
	{
		if (ImGui::Begin("Hierarchy", nullptr, Editor::getBaseWindowFlag()))
		{
			for (auto child : Transform::Root->getChildren())
			{
				if (child == Editor::getEditorCamera()->transform)continue;
				HierarchyNode(child);
			}

			ImVec2 dummySize = ImGui::GetWindowSize();
			dummySize.y *= 0.3f;
			ImGui::Dummy(dummySize);
			if (ImGui::BeginDragDropTarget())
			{
				auto payload = ImGui::AcceptDragDropPayload("GameObject");
				{
					if (payload != nullptr)
					{
						Transform* ptr = *((Transform**)payload->Data);
						Transform::Root->addChild(ptr);
					}
				}

				payload = ImGui::AcceptDragDropPayload("Asset");
				if (payload != nullptr)
				{
					char* path = (char*)payload->Data;
					if(
						strstr(path, ".fbx") != nullptr ||
						strstr(path, ".blend") != nullptr ||
						strstr(path, ".obj") != nullptr)
					{
						Model* model = Model::create(path);
						if (model != nullptr)
						{
							model->instantiate("New Model");
						}
					}
				}

				ImGui::EndDragDropTarget();
			}

			if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
			{
				ImGui::OpenPopup("Edit Menu");
			}

			if (ImGui::BeginPopup("Edit Menu"))
			{
				ImGui::Text("Edit Menu");
				ImGui::Separator();

				if (ImGui::Selectable("Create new GameObject"))
				{
					new GameObject();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}


	static void HierarchyNode(Transform* target)
	{
		ImGuiTreeNodeFlags flags = 0;
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_AllowItemOverlap;
		if (target->getChildCount() == 0)flags |= ImGuiTreeNodeFlags_Leaf;

		ImGui::PushID((size_t)target);

		bool nodeOpend = ImGui::TreeNodeEx("", flags);

		bool selected = target == Editor::getSelectedTransform();
		ImGui::SameLine();
		if (ImGui::Selectable(target->gameObject->getName().c_str(), &selected))
		{
			Editor::selectHierarchyItem(target);
		}

		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("GameObject Edit");
			Editor::selectHierarchyItem(target);
		}

		if (ImGui::BeginPopup("GameObject Edit"))
		{
			ImGui::Text("Edit Menu");
			ImGui::Separator();
			if (ImGui::Selectable("Delete"))
			{
				if (target == Editor::getSelectedTransform()) Editor::selectHierarchyItem(nullptr);
				delete(target->gameObject);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptPeekOnly))
		{
			ImGui::SetDragDropPayload("GameObject", &target, sizeof(target));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			auto payload = ImGui::AcceptDragDropPayload("GameObject");
			{
				if (payload != nullptr)
				{
					Transform* ptr = *((Transform**)payload->Data);
					target->addChild(ptr);
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (nodeOpend)
		{
			for (auto child : target->getChildren())
			{
				HierarchyNode(child);
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}