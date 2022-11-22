#pragma once
#include "../Engine/Window.h"
#include "../imgui/imgui.h"
#include "../DrawableObjects/VolumetricClouds.h"
#include "../DrawableObjects/Terrain.h"
#include "../DrawableObjects/Skybox.h"
#include "../DrawableObjects/Water.h"
#include "../DrawableObjects/CloudsModel.h"


class HudRender
{
public:
	Water* water;
	Skybox* sky;
	CloudsModel* clouds;
	Terrain* terrain;
	drawableObject* uiGUIControls;
	SceneComponent Scene;
	GameObject* CurrentObjectSelect;
	bool CreatingNewOBJ;

public:
	virtual void RenderHUD();
};