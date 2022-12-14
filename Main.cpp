#ifndef GLAD_H
#define GLAD_H
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include "Engine/Window.h"
#include "Engine/shader.h"
#include "Engine/ScreenSpaceShader.h"
#include "Engine/texture.h"

#include "DrawableObjects/VolumetricClouds.h"
#include "DrawableObjects/Terrain.h"
#include "DrawableObjects/Skybox.h"
#include "DrawableObjects/Water.h"
#include "DrawableObjects/CloudsModel.h"

#include <camera.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include "Engine/glError.h"

#include "Components/SceneComponent.h"
#include "DrawableObjects/drawableObject.h"
#include "DrawableObjects/GUI.h"

#include <iostream>
#include <vector>
#include <functional>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "EngineData/HudRender.h"
#include "Components/GameObject.h"
#include "Include/IrrKlang/irrKlang.h"
#include <list>

using namespace irrklang;
using namespace std;
using namespace glm;

HudRender* hud = new HudRender();

Camera camera = Camera();
Camera camera2 = Camera();

SceneComponent scene;
ISoundEngine* SoundEngine = createIrrKlangDevice();

int main()
{
	vec3 startPosition(0.0f, 800.0f, 0.0f);
	camera.Position = startPosition;

	int success;
	Window window(success, 1920, 1080);
	if (!success) return -1;

	window.camera = &camera;

	GUI gui(window);

	vec3 fogColor(0.5,0.6,0.7);
	vec3 lightColor(255, 255, 230);
	lightColor /= 255.0;

	FrameBufferObject SceneFBO(Window::SCR_WIDTH, Window::SCR_HEIGHT);
	vec3 lightPosition, seed;
	mat4 proj = perspective(radians(camera.Zoom), (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.f, 10000000.0f);
	vec3 lightDir = vec3(-.5, 45, 1.0);

	scene.lightPos = lightPosition;
	scene.lightColor = lightColor;
	scene.fogColor = fogColor;
	scene.seed = seed;
	scene.projMatrix = proj;
	scene.CurrentCamera = &camera;
	scene.sceneFBO = &SceneFBO;
	scene.lightDir = lightDir;
	

	drawableObject::scene = &scene;

	int gridLength = 120;
	Terrain terrain(gridLength);

	float waterHeight = 120.;
	Water water(vec2(0.0, 0.0), gridLength, waterHeight);
	terrain.waterPtr = &water;

	Skybox skybox;
	CloudsModel cloudsModel(&scene, &skybox);
	
	VolumetricClouds volumetricClouds(Window::SCR_WIDTH, Window::SCR_HEIGHT, &cloudsModel);
	VolumetricClouds reflectionVolumetricClouds(1280, 720, &cloudsModel);
	
	gui.subscribe(&terrain)
		.subscribe(&skybox)
		.subscribe(&cloudsModel)
		.subscribe(&water);

	ScreenSpaceShader PostProcessing("shaders/post_processing.frag");
	ScreenSpaceShader fboVisualizer("shaders/visualizeFbo.frag"); 

	hud->Scene = scene;
	SoundEngine->play2D("F:/GameEngines VS/FLKEngine_C-/Music/Stellar.mp3", true);
	SoundEngine->setSoundVolume (0.1f);

	while (window.continueLoop())
	{
		scene.lightDir = normalize(scene.lightDir);
		scene.lightPos = scene.lightDir * 1e6f + camera.Position;
		scene.lightPos = scene.lightDir*1e6f + camera.Position;

		float frametime = 1 / ImGui::GetIO().Framerate;
		window.processInput(frametime);

		terrain.updateTilesPositions();
		cloudsModel.update();
		gui.update();
		skybox.update();

		SceneFBO.bind();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		hud->water = &water;
		hud->sky = &skybox;
		hud->clouds = &cloudsModel;
		hud->terrain = &terrain;
		hud->uiGUIControls = &gui;

		hud->RenderHUD();


		if (scene.wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		mat4 view = scene.CurrentCamera->GetViewMatrix();
		scene.projMatrix = perspective(radians(camera.Zoom), (float)Window::SCR_WIDTH / (float)Window::SCR_HEIGHT, 5.f,10000000.0f);

		
		//draw to water reflection buffer object
		water.bindReflectionFBO();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.CurrentCamera->invertPitch();
		scene.CurrentCamera->Position.y -= 2 * (scene.CurrentCamera->Position.y - water.getHeight());
		
		terrain.up = 1.0;
		terrain.draw();
		FrameBufferObject const& reflFBO = water.getReflectionFBO();
		
		ScreenSpaceShader::disableTests();

		reflectionVolumetricClouds.draw();
		water.bindReflectionFBO(); //rebind refl buffer; reflVolumetricClouds unbound it

		
		Shader& post = PostProcessing.getShader();
		post.use();
		post.setVec2("resolution", vec2(1280, 720));
		post.setSampler2D("screenTexture", reflFBO.tex, 0);
		post.setSampler2D("depthTex", reflFBO.depthTex, 2);
		post.setSampler2D("cloudTEX", reflectionVolumetricClouds.getCloudsRawTexture(), 1);
		PostProcessing.draw();

		ScreenSpaceShader::enableTests();
		
		scene.CurrentCamera->invertPitch();
		scene.CurrentCamera->Position.y += 2 * abs(scene.CurrentCamera->Position.y - water.getHeight());
		
		//draw to water refraction buffer object
		water.bindRefractionFBO();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		terrain.up = -1.0;
		terrain.draw();

		// draw terrain and water
		scene.sceneFBO->bind();
		terrain.draw();
		water.draw();

		//disable test for quad rendering
		ScreenSpaceShader::disableTests();

		volumetricClouds.draw();
		skybox.draw();

		// blend volumetric clouds rendering with terrain and apply some post process
		unbindCurrentFrameBuffer(); // on-screen drawing
		//Shader& post = PostProcessing.getShader();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		post.use();
		post.setVec2("resolution", vec2(Window::SCR_WIDTH, Window::SCR_HEIGHT));
		post.setVec3("cameraPosition", scene.CurrentCamera->Position);
		post.setSampler2D("screenTexture", SceneFBO.tex, 0);
		post.setSampler2D("cloudTEX", volumetricClouds.getCloudsTexture(), 1);
		post.setSampler2D("depthTex", SceneFBO.depthTex, 2);
		post.setSampler2D("cloudDistance", volumetricClouds.getCloudsTexture(VolumetricClouds::cloudDistance), 3);

		post.setBool("wireframe", scene.wireframe);

		post.setMat4("VP", scene.projMatrix * view);
		PostProcessing.draw();



		// Texture visualizer
		Shader& fboVisualizerShader = fboVisualizer.getShader();
		fboVisualizerShader.use();
		fboVisualizerShader.setSampler2D("fboTex", volumetricClouds.getCloudsTexture(), 0);
		//fboVisualizer.draw(); //for debugging purposes

		gui.draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		window.swapBuffersAndPollEvents();
	}
}