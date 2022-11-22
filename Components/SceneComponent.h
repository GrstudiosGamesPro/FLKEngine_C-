#ifndef SCENELEMENTS_H
#define SCENELEMENTS_H

#include <camera.h>
#include <glm/glm.hpp>
#include "../Engine/buffers.h"
#include <random>
#include "../Components/GameObject.h"
#include <list>
#include <array>
#include <vector>


struct SceneComponent {
	glm::vec3 lightPos, lightColor, lightDir, fogColor, seed;
	glm::mat4 projMatrix;
	Camera *CurrentCamera;
	FrameBufferObject *sceneFBO;
	bool wireframe = false;

	//std::list<GameObject> *Objetos = new std::list<GameObject>();
    std::vector<GameObject*> *Objetos = new std::vector<GameObject*>();
};
#endif