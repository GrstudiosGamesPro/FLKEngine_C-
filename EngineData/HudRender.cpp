#include "HudRender.h"
#include "../Engine/Window.h"
#include "../imgui/imgui.h"
#include <iostream>


using namespace std;

void HudRender::RenderHUD() 
{
	if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Save")) {

			}

			if (ImGui::MenuItem("Exit")) {

			}

			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("World Settings")) {

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
		ImGui::EndMainMenuBar();
	}
}