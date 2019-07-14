#include "Window_Hierarchy.hpp"


namespace yellowEditor
{
	static struct Shape
	{
		Mesh* mesh;
		std::string name;
	};

	static void HierarchyNode(Transform* target);

	void DrawWindow_Hierarchy()
	{
		static Shape shapes[] = {
			{Mesh::create("./res/Mesh/cube.obj"), "Cube"},
			{Mesh::create("./res/Mesh/quad.obj"), "Quad"},
			{Mesh::create("./res/Mesh/sphere.obj"), "Sphere"},
			{Mesh::create("./res/Mesh/cone.obj"), "Cone"}
		};

		if (ImGui::Begin("Hierarchy", nullptr, Editor::getBaseWindowFlag()))
		{
			for (auto child : Transform::Root->getChildren())
			{
				if (child == Editor::getEditorCamera()->transform) continue;
				HierarchyNode(child);
			}

			float leftHeight = ImGui::GetWindowHeight() - ImGui::GetCursorPosY() - 10;
			leftHeight = Utils::max(leftHeight, ImGui::GetWindowHeight() * 0.2f);

			ImVec2 dummySize = ImGui::GetWindowSize();
			dummySize.y = leftHeight;
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
					const char* ext = Utils::getExtension(path);
					if (
						_strcmpi(ext, "obj") == 0 ||
						_strcmpi(ext, "fbx") == 0 ||
						_strcmpi(ext, "blend") == 0
						)
					{
						Editor::setGameContext();
						Model* model = Model::create(path);
						if (model != nullptr)
						{
							model->instantiate("New Model");
						}
						Editor::setEditorContext();
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

				if (ImGui::Selectable("New GameObject"))
				{
					new GameObject();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::BeginMenu("3D Shape"))
				{
					for (auto shape : shapes)
					{
						if (ImGui::MenuItem(shape.name.c_str()))
						{
							auto go = new GameObject(shape.name.c_str());
							go->addComponent<MeshRenderer>()->set(shape.mesh);
							ImGui::CloseCurrentPopup();
						}
					}

					ImGui::EndMenu();
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