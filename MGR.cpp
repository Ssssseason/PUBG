#include "stdafx.h"
#include "MGR.h"


MGR::MGR()
{
}

MGR::~MGR()
{
}

SceneMRG::SceneMRG() {
	glm::mat4 model;
	//test: load models
	//OBJ ourTreeOBJ(model, "models/tree/tree_low.obj");
	//handle all other objects' behavior
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 10.75f, 0.0f));
	model = glm::rotate(model, (float)M_PI_2, glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	OBJ ourFemaleOBJ(model, "models/female/female02.obj");
	//ourFemaleOBJ.setOriModelMat(model);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 20.f, -5.0f));
	model = glm::rotate(model, (float)M_PI_2, glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	OBJ ourMaleOBJ(model, "models/man/male02.obj");
	//ourMaleOBJ.setOriModelMat(model);
	//ourTreeOBJ.setModelMat(model);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-50.0f, 100.75f, -20.0f));
	model = glm::rotate(model, (float)M_PI_2, glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	OBJ ourSceneOBJ(model, "models/terrain/mountains_4.obj");
	//ourSceneOBJ.setOriModelMat(model);

	objects.push_back(ourFemaleOBJ);
	objects.push_back(ourMaleOBJ);
	objects.push_back(ourSceneOBJ);
}

void SceneMRG::DrawAll(Shader &shader) {
	for (it = objects.begin(); it != objects.end(); ++it) {
		it->Draw(shader);
	}
}

void SceneMRG::DetectCollisionALL(MovableOBJ &mobj) {
	for (it = objects.begin(); it != objects.end(); ++it) {
		mobj.detectCollision(*it);
	}
}

void SceneMRG::DetectCollisionALL(Player &player) {
	for (it = objects.begin(); it != objects.end(); ++it) {
		player.checkCollision(*it);
	}
}

void SceneMRG::ShowInfo() {
	std::cout << "Scene:\n";
	for (it = objects.begin(); it != objects.end(); ++it) {
		it->showInfo();
	}
}

NPCMGR::NPCMGR() {
	glm::vec3 loc = glm::vec3(0, -3, 0);
	glm::vec3 front = glm::vec3(0, 1, 0);
	glm::vec3 up = glm::vec3(0, 0, 1);
	glm::mat4 model;
	model = glm::rotate(model, (float)M_PI_2, glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	NPC ourFemaleOBJ(loc, front, up, 0.1f, model, "models/female/female02.obj");
	objects.push_back(ourFemaleOBJ);
}

void NPCMGR::DrawAll(Shader &shader) {
	for (it = objects.begin(); it != objects.end(); ++it) {
		it->Draw(shader);
	}
}

void NPCMGR::DetectCollisionALL(MovableOBJ &mobj) {
	//todo: collide 这个函数应该是没有什么卵用的
	//for (it = objects.begin(); it != objects.end(); ++it) {
	//	mobj.detectCollision(*it);
	//}
}

void NPCMGR::DetectCollisionALL(Player &player) {
	for (it = objects.begin(); it != objects.end(); ++it) {
		player.checkCollision(*it);
	}
}

void NPCMGR::UpdateAll() {
	//todo: control behavior
	for (it = objects.begin(); it != objects.end(); ++it) {
		it->setYaw(it->getYaw() + 0.5);
		//it->move(MovableOBJ::FORWARD);
	}
}

//debug
void NPCMGR::ShowInfo() {
	std::cout << "NPC:\n";
	for (it = objects.begin(); it != objects.end(); ++it) {
		it->showInfo();
	}
}