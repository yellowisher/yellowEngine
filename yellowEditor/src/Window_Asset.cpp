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

	static std::string draggingPath = "";
	static bool changingName = false;
	static const int buffSize = 4;
	static char buff[buffSize] = "";

	using PropertyType = yellowEngine::AnimationClip::PropertyType;
	static AnimationClip* clip = nullptr;
	static Animator* animator = nullptr;
	static int frameCount = 60;
	static int _frame = 0;
	static float animationLeftWidth = 0.3f;
	static bool isPlaying = false;
	static bool isDragging = false;
	static std::pair<AnimationClip::Key, int> draggingNode;

	static std::map<std::string, AnimationClip::PropertyType> stringToProperty = {
		{"Position", AnimationClip::Property_Position},
		{"Rotation", AnimationClip::Property_EulerRotation},
		{"Scale", AnimationClip::Property_Scale}
	};

	static std::map<AnimationClip::PropertyType, std::string> propertyToString = {
		{AnimationClip::Property_Position, "Position"},
		{AnimationClip::Property_EulerRotation, "Rotation"},
		{AnimationClip::Property_Scale, "Scale"}
	};

	// really should separate docks and windows...
	static void DrawTab_Asset();
	static void DrawTab_Animation();
	static void AddProperty_Transform(Transform* target);
	static void AddProperty_Child(Transform* target);
	static void AddKeyFrame(Transform* target, float frame, 
							AnimationClip::Key key, bool init = false);
	static void AddKeyFrame(std::vector< AnimationClip::KeyFrame>& keyFrames, AnimationClip::KeyFrame keyFrame);
	static float getSliderWidth(bool slider = true);
	static void CreateClip();

	void Init_AssetWindow()
	{
		int width, height, channels;

		//stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load(".\\res\\Texture\\folder.png", &width, &height, &channels, 0);
		glGenTextures(1, &folderTexture);
		glBindTexture(GL_TEXTURE_2D, folderTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		data = stbi_load(".\\res\\Texture\\file.png", &width, &height, &channels, 0);
		glGenTextures(1, &fileTexture);
		glBindTexture(GL_TEXTURE_2D, fileTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	void DrawWindow_Asset()
	{
		auto flag = Editor::getBaseWindowFlag() | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;
		if (ImGui::Begin("Asset", nullptr, flag))
		{
			if(ImGui::BeginTabBar("Tab"))
			{
				if (ImGui::BeginTabItem("Asset"))
				{
					DrawTab_Asset();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Animation"))
				{
					DrawTab_Animation();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();
 	}


	static void DrawTab_Asset()
	{
		if (!ImGui::IsWindowFocused()) { changingName = false; }
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
					std::string newMaterial = workingDirectory + "\\New Material.yem";
					Material::saveAsFile(nullptr, newMaterial.c_str());
					files.push_back("New Material.yem");
				}
				if (ImGui::MenuItem("Folder"))
				{
					std::string newDirectory = workingDirectory + "\\New Folder";
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

			bool isFolder = false;
			std::string name;
			std::string path;
			if (n < directories.size())
			{
				name = path = directories[n];
				isFolder = true;
			}
			else
			{
				name = path = files[n - directories.size()];
			}

			ImGui::BeginGroup();

			bool selected = Editor::getSelectedAsset() == workingDirectory + "\\" + path;
			ImGui::ImageButtonCustom((ImTextureID)(isFolder ? folderTexture : fileTexture), buttonSize, selected);
			if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered() && isFolder)
			{
				// just appending \\.. might lead to crash
				if (path == "..")
				{
					workingDirectory = workingDirectory.substr(0, workingDirectory.find_last_of("\\"));
				}
				else
				{
					workingDirectory += "\\" + path;
				}
				LoadAsset();
				ImGui::EndGroup();
				ImGui::PopID();
				break;
			}
			else if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered() && !isFolder)
			{
				Editor::selectAssetItem(workingDirectory + "\\" + path);
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptPeekOnly))
			{
				draggingPath = workingDirectory + "\\" + path;
				ImGui::SetDragDropPayload("Asset", draggingPath.c_str(), draggingPath.length() + 1);
				ImGui::EndDragDropSource();
			}

			if (selected && ImGui::IsKeyDown(GLFW_KEY_F2))
			{
				changingName = true;
				memset(buff, 0, buffSize);
				memcpy(buff, path.c_str(), path.size());
			}

			if (selected && changingName)
			{
				float cursor = ImGui::GetCursorPosX() + buttonSize.x / 2.0f - ImGui::CalcTextSize(name.c_str()).x / 2.0f;
				ImGui::SetCursorPosX(cursor);
				ImGui::SetNextItemWidth(ImGui::CalcTextSize(name.c_str()).x);
				if (ImGui::InputText("", buff, buffSize, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
				{
					std::string oldName = workingDirectory + "\\" + path;
					std::string newName = workingDirectory + "\\" + std::string(buff);
					rename(oldName.c_str(), newName.c_str());
					changingName = false;
					LoadAsset();
				}
			}
			else
			{
				// in case too long file name
				if (ImGui::CalcTextSize(name.c_str()).x > buttonSize.x)
				{
					float left = buttonSize.x - ImGui::CalcTextSize("..").x;
					for (int i = 1; i < name.length(); i++)
					{
						if (ImGui::CalcTextSize(name.c_str(), name.c_str() + i).x > left)
						{
							name = name.substr(0, i + 1) + "..";
							break;
						}
					}
				}
				float cursor = ImGui::GetCursorPosX() + buttonSize.x / 2.0f - ImGui::CalcTextSize(name.c_str()).x / 2.0f;
				ImGui::SetCursorPosX(cursor);
				ImGui::Text(name.c_str());
			}
			ImGui::EndGroup();

			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + buttonSize.x;
			if (n + 1 < itemCount && next_button_x2 < window_visible_x2) ImGui::SameLine();
			ImGui::PopID();
		}
	}


	static void DrawTab_Animation()
	{
		static const int frameBufferSize = 64;
		static char frameBuffer[frameBufferSize] = "60";
		static int frameCount = 60;

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * animationLeftWidth);
		ImGui::Text("Frame");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::CalcTextSize("00000000").x);
		if (ImGui::InputText("##frame", frameBuffer, frameBufferSize, ImGuiInputTextFlags_CharsDecimal))
		{
			frameCount = atoi(frameBuffer);
			for (auto channel : clip->_channels)
			{
				(channel.second.end() - 1)->frame = frameCount;
			}
			
		}
		ImGui::SameLine();

		if (!isPlaying)
		{
			if (ImGui::Button("Play"))
			{
				isPlaying = true;
			}
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonHovered));
			if (ImGui::Button("Pause"))
			{
				isPlaying = false;
			}
			ImGui::PopStyleColor();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			isPlaying = false;
			_frame = 0;
			if (animator)
			{
				animator->gotoFrame(0);
			}
		}
		ImGui::SameLine(ImGui::GetColumnWidth() - ImGui::CalcTextSize("Save").x - 20);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7, 0.4, 0.1, 1.0));
		if (ImGui::Button("Save"))
		{
			std::string path = fileDialog({ { "yea","Yellow Engine Animation" } }, true) + ".yea";
			AnimationClip::saveClip(path.c_str(), clip);
		}
		ImGui::PopStyleColor();

		Transform* transform = Editor::getSelectedTransform();
		if (transform != nullptr)
		{
			Animator* newAnimator = transform->gameObject->getComponent<Animator>();
			if (newAnimator != nullptr)
			{
				if (newAnimator != animator)
				{
					if (clip)
					{
						delete(clip);
						clip = nullptr;
					}
					animator = newAnimator;
					CreateClip();
					animator->setClip(clip);
				}

				if (isPlaying)
				{
					if (++_frame == frameCount)
					{
						_frame = 0;
					}
					animator->gotoFrame(_frame);
				}

				// show all selected properties
				ImGui::NextColumn();
				ImGui::SetNextItemWidth(getSliderWidth(true));
				if (ImGui::SliderInt("##frame_slider", &_frame, 0, frameCount - 1))
				{
					animator->gotoFrame(_frame);
				}

				ImGui::NextColumn();
				for (auto keyChannelPair : clip->_channels)
				{
					auto& key = keyChannelPair.first;
					auto& channel = keyChannelPair.second;

					Transform* keyTransform = animator->getTransform(key.transformPath);
					std::string propName = propertyToString[key.prop];
					std::string name = keyTransform->gameObject->getName() + ": " + propName;
					
					//ImGui::Separator();
					if (ImGui::TreeNodeEx(name.c_str()))
					{
						//ImGui::Separator();
						static const char* labels[3] = { "X", "Y", "Z" };

						auto transformProp = keyTransform->getProperty(propName);
						float* basePtr = (float*)((size_t)keyTransform + transformProp.offset);

						for (int i = 0; i < 3; i++)
						{
							ImGui::Separator();
							if (ImGui::TreeNodeEx(labels[i], ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Leaf))
							{
								ImGui::SameLine();
								ImGui::SetNextItemWidth(60);
								ImGui::SetCursorPosX(ImGui::GetColumnWidth() - 80);
								if (ImGui::DragFloat("" + i, basePtr + i, 0.05))
								{
									keyTransform->onValueChanged();
									AddKeyFrame(keyTransform, _frame, key);
								}
								ImGui::TreePop();
							}

							ImGui::NextColumn();

							auto& keyFrames = clip->_channels[key];
							for (int k = 0; k < keyFrames.size(); k++)
							{
								float ratio = (float)keyFrames[k].frame / (frameCount);
								ImGui::SetCursorPosX(ImGui::GetColumnWidth(0) + ratio * (getSliderWidth()));

								bool selected = (isDragging) && (draggingNode.first == key) && (draggingNode.second == k);

								if (selected)
								{
									ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_ButtonActive));
									ImGui::Bullet();
									ImGui::PopStyleColor();
								}
								else
								{
									ImGui::Bullet();
								}

								static float keyFrameStride = 0;
								static float handledDelta = 0;
								if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered())
								{
									isDragging = true;
									draggingNode.first = key;
									draggingNode.second = k;
									keyFrameStride = (getSliderWidth()) / (frameCount);
									handledDelta = 0;
								}

								if (selected && i == 2 && ImGui::IsMouseDragging(0))
								{
									float moveDelta = ImGui::GetMouseDragDelta(0).x - handledDelta;
									int move = moveDelta / keyFrameStride;
									if (move != 0)
									{
										handledDelta += move * keyFrameStride;
										keyFrames[k].frame += move;
										//animator->gotoFrame(_frame);
									}
								}

								if (selected && isDragging && ImGui::IsMouseReleased(0))
								{
									int move = ImGui::GetMouseDragDelta(0).x / keyFrameStride;
									if (move != 0)
									{
										auto kf = keyFrames[k];
										keyFrames.erase(keyFrames.begin() + k);
										AddKeyFrame(keyFrames, kf);
									}

									animator->gotoFrame(_frame);
								}

								ImGui::SameLine();
							}
							ImGui::NextColumn();
						}
						ImGui::Separator();

						ImGui::TreePop();
					}
				}

				ImGui::Dummy(ImVec2(1, 10));
				float width = ImGui::CalcTextSize("Add Property").x + 20;
				ImGui::SetCursorPosX(ImGui::GetColumnWidth() * 0.5f - width * 0.5f);
				ImGui::SetNextItemWidth(width);
				if (ImGui::Button("Add Property"))
				{
					ImGui::OpenPopup("add_property");
				}

				if (ImGui::BeginPopup("add_property"))
				{
					AddProperty_Transform(transform);
					AddProperty_Child(transform);
					ImGui::EndPopup();
				}
			}
		}
		if (ImGui::IsMouseReleased(0))
		{
			isDragging = false;
		}

		ImGui::Columns(1);
	}

	static void AddProperty_Transform(Transform* target)
	{
		// let all the components can be property?
		if (ImGui::TreeNodeEx("Transform"))
		{
			const auto& properties = yellowEngine::Component::getProperties()[target->getTypeName()];
			for (auto prop : properties)
			{
				if (ImGui::TreeNodeEx(prop.name.c_str(), ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Leaf))
				{
					if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
					{
						std::string path = "";
						Transform* cursor = target;
						while (cursor != Editor::getSelectedTransform())
						{
							path = cursor->gameObject->getName() + '/' + path;
							cursor = cursor->getParent();
						}

						auto animProp = stringToProperty[prop.name];
						AnimationClip::Key key = { path,  animProp };

						clip->_channels[key] = {};
						AddKeyFrame(target, 0, key, true);
						AddKeyFrame(target, frameCount - 1, key, true);

						ImGui::CloseCurrentPopup();
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}


	static void AddProperty_Child(Transform* target)
	{
		for (auto child : target->getChildren())
		{
			if(ImGui::TreeNodeEx(child->gameObject->getName().c_str()))
			{
				AddProperty_Transform(child);
				AddProperty_Child(child);
				ImGui::TreePop();
			}
		}
	}


	static void AddKeyFrame(Transform* target, float frame, 
							AnimationClip::Key key, bool init)
	{
		auto& keyFrames = clip->_channels[key];

		auto transformProp = target->getProperty(propertyToString[key.prop]);
		float* basePtr = (float*)((size_t)target + transformProp.offset);

		if (init)
		{
			keyFrames.push_back(AnimationClip::KeyFrame(frame, Vector3(basePtr[0], basePtr[1], basePtr[2])));
			return;
		}

		AddKeyFrame(keyFrames, AnimationClip::KeyFrame(frame, Vector3(basePtr[0], basePtr[1], basePtr[2])));
	}


	static void AddKeyFrame(std::vector< AnimationClip::KeyFrame>& keyFrames, AnimationClip::KeyFrame keyFrame)
	{
		auto it = keyFrames.begin();
		for (; it != keyFrames.end(); ++it)
		{
			if (it->frame == keyFrame.frame)
			{
				it->value.vector3 = keyFrame.value.vector3;
				return;
			}
			if (it->frame > keyFrame.frame)
			{
				break;
			}
		}
		keyFrames.insert(it, keyFrame);
	}



	static float getSliderWidth(bool slider)
	{
		int offset = 0;
		if (slider) ImGui::GetColumnWidth() - offset;
		return ImGui::GetColumnWidth() - offset - ImGui::getBulletWidth();
	}


	static void CreateClip()
	{
		clip = new AnimationClip();
		clip->_frameCount = frameCount;
		clip->_isLooping = true;
		_frame = 0;
		isPlaying = false;
	}


	void LoadAsset(bool init)
	{
		directories.clear();
		files.clear();

		if (init) workingDirectory = Editor::getAssetPath();
		std::string path = workingDirectory + "\\*.*";
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