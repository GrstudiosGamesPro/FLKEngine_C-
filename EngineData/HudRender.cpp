#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Engine/Window.h"
#include "../Engine/shader.h"
#include "../Engine/ScreenSpaceShader.h"
#include "../Engine/texture.h"

#include "../DrawableObjects/VolumetricClouds.h"
#include "../DrawableObjects/Terrain.h"
#include "../DrawableObjects/Skybox.h"
#include "../DrawableObjects/Water.h"
#include "../DrawableObjects/CloudsModel.h"

#include <camera.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include "../Engine/glError.h"

#include "../Components/SceneComponent.h"
#include "../DrawableObjects/drawableObject.h"
#include "../DrawableObjects/GUI.h"

#include <iostream>
#include <vector>
#include <functional>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../EngineData/HudRender.h"
#include "../Components/GameObject.h"
#include <ctime>
#include <cstdlib>
#include <list>
#include<cstring>
#include <iterator>
#include <sstream>



using namespace std;
using namespace glm;

using namespace std;

void HudRender::RenderHUD() 
{
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);



	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Save")) {

			}

			if (ImGui::MenuItem("Exit")) {

			}

			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu ("World Settings")) {

			if (ImGui::MenuItem("Water Settings")) {
				water->Editing = true;
			}

			if (ImGui::MenuItem("Sky Settings")) {
				sky->Editing = true;
			}

			if (ImGui::MenuItem("Clouds Settings")) {
				clouds->Editing = true;
			}

			if (ImGui::MenuItem("Terrain Settings")) {
				terrain->Editing = true;
			}

			if (ImGui::MenuItem("Scene Controls Settings")) {
				uiGUIControls->Editing = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu ("GameObject"))
		{
			if (ImGui::TreeNode ("Create Object"))
			{
				if (ImGui::Button ("Cubo"))
				{
					GameObject* obj = new GameObject();
					string oldName = "Objeto";
					string ObjectName = oldName.append (to_string (Scene.Objetos->size()));

					strcpy_s (obj->NameObject, ObjectName.c_str());		
					Scene.Objetos->push_back (obj);

					cout << obj->NameObject;

					obj->Start();
				}
				ImGui::TreePop();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu ("Package"))
		{
			if (ImGui::MenuItem ("Package Windows (FOUND)"))
			{

			}
			if (ImGui::MenuItem("Package Android (NOT FOUND)"))
			{

			}
			ImGui::Separator();
			ImGui::Separator();

			if (ImGui::MenuItem("Package Linux (NOT FOUND)"))
			{

			}

			if (ImGui::MenuItem("Package MacOS (NOT FOUND)"))
			{

			}
			ImGui::Separator();
			ImGui::Separator();

			if (ImGui::MenuItem("Package PS4 (NOT FOUND)"))
			{

			}

			if (ImGui::MenuItem("Package PS5 (NOT FOUND)"))
			{

			}
			ImGui::Separator();
			ImGui::Separator();

			if (ImGui::MenuItem("Package XBOX SERIES (NOT FOUND)"))
			{

			}

			if (ImGui::MenuItem("Package XBOX ONE (NOT FOUND)"))
			{

			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	if (ImGui::Begin ("Jerarquia"))
	{
		if (ImGui::TreeNode("ESPACIO DE TRABAJO"))
		{
			std::vector<GameObject*> ObjectsInHud = *Scene.Objetos;

			for (int i = 0; i < Scene.Objetos->size(); i++) {
				if (ImGui::Button(ObjectsInHud[i]->NameObject)) {
					CurrentObjectSelect = ObjectsInHud[i];
				}
			}
			
			ImGui::TreePop();
		}
		ImGui::End();
	}

	if (CurrentObjectSelect != nullptr) {
		if (ImGui::Begin("Inspector")) {
			GameObject* obj = CurrentObjectSelect;

			char name[128];
			strcpy_s(name, obj->NameObject);

			string GetIndex = "Index: ";

			ImGui::InputText("Name: ", name, ImGuiInputTextFlags_AutoSelectAll);
			strcpy_s(obj->NameObject, name);

			ImGui::Separator();

			for (int i = 0; i < Scene.Objetos->size(); i++) {
				if ((*Scene.Objetos)[i] == CurrentObjectSelect) {
					string NewIndex = GetIndex.append(to_string(i));
					ImGui::Text (NewIndex.c_str());
				}
			}
		}

		ImGui::End();
	}
}