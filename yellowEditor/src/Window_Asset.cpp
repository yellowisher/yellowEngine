#include "Window_Asset.hpp"
#include "EditorUtils.hpp"

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <imgui_user.h>
#include <Windows.h>
#include <cstring>

namespace yellowEditor
{
	static std::string workingDirectory;
	static std::vector<std::string> directories;
	static std::vector<std::string> files;

	static ImVec2 buttonSize(80, 80);
	static GLuint folderTexture;
	static GLuint fileTexture;

	void Init_AssetWindow()
	{
		int width, height, channels;

		//stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load("./res/Texture/folder.png", &width, &height, &channels, 0);
		glGenTextures(1, &folderTexture);
		glBindTexture(GL_TEXTURE_2D, folderTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		data = stbi_load("./res/Texture/file.png", &width, &height, &channels, 0);
		glGenTextures(1, &fileTexture);
		glBindTexture(GL_TEXTURE_2D, fileTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	void DrawWindow_Asset()
	{
		if (ImGui::Begin("Asset", nullptr, Editor::getBaseWindowFlag()))
		{
			if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
			{
				ImGui::OpenPopup("AssetPopup");
			}

			if (ImGui::BeginPopup("AssetPopup"))
			{
				ImGui::Text("Asset Menu");
				ImGui::Separator();
				if (ImGui::BeginMenu("Create"))
				{
					if (ImGui::MenuItem("Material"))
					{

					}
					if (ImGui::MenuItem("Folder"))
					{
						std::string newDirectory = workingDirectory + "/Mew Folder";
						createDirectory(newDirectory);
						directories.push_back("New Folder");
					}
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}

			ImGuiStyle& style = ImGui::GetStyle();
			int itemCount = directories.size() + files.size();
			float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

			for (int n = 0; n < itemCount; n++)
			{
				ImGui::PushID(n);

				std::string name;
				GLuint texture;
				if (n < directories.size())
				{
					name = directories[n];
					texture = folderTexture;
				}
				else
				{
					name = files[n - directories.size()];
					texture = fileTexture;
				}

				ImGui::BeginGroup();
				ImGui::ImageButtonCustom((ImTextureID)texture, buttonSize);

				// in case too long file name
				if (ImGui::CalcTextSize(name.c_str()).x > buttonSize.x)
				{
					float left = buttonSize.x - ImGui::CalcTextSize("...").x;
					for (int i = 1; i < name.length(); i++)
					{
						if (ImGui::CalcTextSize(name.c_str(), name.c_str() + i).x > left)
						{
							name = name.substr(0, i) + "...";
							break;
						}
					}
				}
				float cursor = ImGui::GetCursorPosX() + buttonSize.x / 2.0f - ImGui::CalcTextSize(name.c_str()).x / 2.0f;
				ImGui::SetCursorPosX(cursor);
				ImGui::Text(name.c_str());
				ImGui::EndGroup();

				float last_button_x2 = ImGui::GetItemRectMax().x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x + buttonSize.x;
				if (n + 1 < itemCount && next_button_x2 < window_visible_x2) ImGui::SameLine();
				ImGui::PopID();
			}
		}
		ImGui::End();
	}


	void LoadAsset()
	{
		directories.clear();
		files.clear();

		workingDirectory = Editor::getAssetPath();
		std::string path = workingDirectory + "/*.*";
		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile(path.c_str(), &ffd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
			return;
		}
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// ignore current directory
				if (strcmp(ffd.cFileName, ".") != 0)
				{
					directories.push_back(ffd.cFileName);
				}
			}
			else
			{
				files.push_back(ffd.cFileName);
			}
		} while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);
	}
}