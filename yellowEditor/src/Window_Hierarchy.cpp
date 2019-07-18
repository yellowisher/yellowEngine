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
			{Mesh::create("./res/Mesh/cube.obj:cube"), "Cube"},
			{Mesh::create("./res/Mesh/quad.obj:Cube_Cube.001"), "Quad"},
			{Mesh::create("./res/Mesh/sphere.obj:Sphere"), "Sphere"},
			{Mesh::create("./res/Mesh/cone.obj:Cone"), "Cone"}
		};

		if (ImGui::Begin("Hierarchy", nullptr, Editor::getBaseWindowFlag()))
		{
			for (int i = 0; i < Transform::Root->getChildCount(); i++)
			{
				auto child = Transform::Root->getChild(i);
				if (child == Editor::getEditorCamera()->transform && !Editor::showEditorCamera) continue;
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
					GameObject* newObject = new GameObject();
					Editor::selectHierarchyItem(newObject->transform);
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

		auto nodeId = ImGui::GetID("##w");

		bool nodeOpend = ImGui::TreeNodeEx("##w", flags);

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

			if (ImGui::Selectable("Add Child"))
			{
				GameObject* child = new GameObject();
				target->addChild(child->transform);
				Editor::selectHierarchyItem(child->transform);
				nodeOpend = true;
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Selectable("Clone"))
			{
				Editor::setGameContext();
				GameObject* newObject = target->gameObject->clone();
				Editor::selectHierarchyItem(newObject->transform);
				Editor::setEditorContext();
				ImGui::CloseCurrentPopup();
			}

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
			ImGui::GetStateStorage()->SetInt(nodeId, 1);
			for (int i = 0; i < target->getChildCount(); i++)
			{
				HierarchyNode(target->getChild(i));
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
}