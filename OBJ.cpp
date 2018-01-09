#include "stdafx.h"
#include "OBJ.h"


OBJ::OBJ(glm::mat4 &modelMat, const std::string &modelPath, bool anim):
modelMat(modelMat), model(modelPath)
{
	//generate obb
	std::vector<glm::vec3> vertices;
	std::vector<Vertex>::iterator vtxIter;
	std::vector<Mesh>::iterator meshIter;
	for (meshIter = model.meshes.begin(); meshIter != model.meshes.end(); ++meshIter) {
		for (vtxIter = meshIter->vertices.begin(); vtxIter != meshIter->vertices.end(); ++vtxIter) {
			vertices.push_back(vtxIter->Position);
		}
	}
	obb = obbs::getOBB(vertices);
	currentObb = obb;
}


OBJ::~OBJ()
{	
	//todo model
}

void OBJ::Draw(Shader &shader) {
	shader.setMat4("model", modelMat);
	model.Draw(shader);
}

glm::mat4 OBJ::getModelMat() {
	return modelMat;
}

void OBJ::setModelMat(glm::mat4 &newModelMat) {
	modelMat = newModelMat;
	currentObb = obbs::moveOBB(obb, newModelMat);
}

void OBJ::showOBB() {
	std::cout << "current obb center:" << currentObb.center[0] << "," << currentObb.center[1] << "," << currentObb.center[2] << "\t";
	std::cout << "half dimension:" << currentObb.halfDimension[0] << "," << currentObb.halfDimension[1] << "," << currentObb.halfDimension[2] << std::endl;
	std::cout << "org obb center:" << obb.center[0] << "," << obb.center[1] << "," << obb.center[2] << "\t";
	std::cout << "half dimension:" << obb.halfDimension[0] << "," << obb.halfDimension[1] << "," << obb.halfDimension[2] << std::endl;
}

MovableOBJ::MovableOBJ(glm::vec4 &direction, float speed, glm::mat4 &modelMat, std::string &path, bool anim)
	: dir(direction), speed(speed), isLived(true), OBJ(modelMat, path, anim) {
}

MovableOBJ::~MovableOBJ() {

}

void MovableOBJ::Translate(glm::mat4x4 &mat) {

}
void MovableOBJ::Scale(glm::mat4x4 &mat) {

}
void MovableOBJ::Rotate(glm::mat4x4 &mat) {

}
bool MovableOBJ::DetectCollision(OBJ &obj) {
	return true;
}
//keep or remove it
void MovableOBJ::SetState() {

}
bool MovableOBJ::GetState() const {
	return true;
}
// traverse all obj, detect collisions and update state and location
void MovableOBJ::Update() {

}

void MovableOBJ::Draw(Shader &shader) {
	//
}

Bullet::Bullet(glm::vec4 &direction, float speed, glm::mat4 &modelMat, std::string &path, bool anim): 
	MovableOBJ(direction, speed, modelMat, path, anim) {

}

Bullet::~Bullet() {

}

NPC::NPC(glm::vec4 &direction, float speed, glm::mat4 &modelMat, std::string &path, bool anim) : 
	MovableOBJ(direction, speed, modelMat, path, anim) {

}

NPC::~NPC(){

}

void NPC::Run() {

}

void NPC::Stop() {

}

void NPC::SetDirection() {

}

