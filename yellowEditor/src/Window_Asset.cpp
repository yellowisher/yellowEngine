#include "Window_Asset.hpp"


namespace yellowEditor
{
	void DrawWindow_Asset()
	{
		if (ImGui::Begin("Asset", nullptr, Editor::getBaseWindowFlag()))
		{
			if (ImGui::IsItemClicked(1))
			{
			}
		}
		ImGui::End();
	}
}