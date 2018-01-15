#include "stdafx.h"
#include "OBJ.h"


OBJ::OBJ(glm::mat4 &modelMat, Model mod, bool anim):
oriModelMat(modelMat), model(mod)
{
	//generate obb
	initOBB(model);
	updateCurrenOBB(modelMat);
}

OBJ::~OBJ()
{	
	//todo: destruct model?
}

void OBJ::Draw(Shader &shader) {
	shader.setMat4("model", oriModelMat);
	model.Draw(shader);
}

void OBJ::setOriModelMat(glm::mat4 &newModelMat) {
	oriModelMat = newModelMat;
	updateCurrenOBB(newModelMat);
}

void OBJ::showInfo() {
	std::cout << "current obb center:" << currentObb.center[0] << "," << currentObb.center[1] << "," << currentObb.center[2] << "\t";
	std::cout << "half dimension:" << currentObb.halfDimension[0] << "," << currentObb.halfDimension[1] << "," << currentObb.halfDimension[2] << std::endl;
	std::cout << "ori obb center:" << obb.center[0] << "," << obb.center[1] << "," << obb.center[2] << "\t";
	std::cout << "half dimension:" << obb.halfDimension[0] << "," << obb.halfDimension[1] << "," << obb.halfDimension[2] << std::endl;
}

void OBJ::initOBB(Model &model) {
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

void OBJ::updateCurrenOBB(glm::mat4 &modelMat) {
	currentObb = obbs::moveOBB(obb, modelMat);
}

MovableOBJ::MovableOBJ(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed,  glm::mat4 &modelMat, Model mod, double yaw, double pitch, bool anim)
	: location(loc), front(front), up(up), speed(speed), pitch(pitch), yaw(yaw), v(0), isLive(true), jumpAlr(false), OBJ(modelMat, mod, anim)
{

	update();
}

MovableOBJ::~MovableOBJ() {

}

void MovableOBJ::showInfo() {
	std::cout << "location:" << location[0] << " " << location[1] << " " << location[2] << "\t";
	std::cout << "current obb center:" << currentObb.center[0] << "," << currentObb.center[1] << "," << currentObb.center[2] << "\t";
	std::cout << "half dimension:" << currentObb.halfDimension[0] << "," << currentObb.halfDimension[1] << "," << currentObb.halfDimension[2] << std::endl;
	std::cout << "ori obb center:" << obb.center[0] << "," << obb.center[1] << "," << obb.center[2] << "\t";
	std::cout << "half dimension:" << obb.halfDimension[0] << "," << obb.halfDimension[1] << "," << obb.halfDimension[2] << std::endl;

}

void MovableOBJ::move(MovableOBJ::MovementDirection dir, bool updateOp) {
	if (updateOp)
		lastOp[dir] = true;
	switch (dir)
	{
	case MovableOBJ::FORWARD:
		location += front * speed;
		break;
	case MovableOBJ::BACKWARD:
		location -= front * speed;
		break;
	case MovableOBJ::LEFT:
		location -= right * speed;
		break;
	case MovableOBJ::RIGHT:
		location += right * speed;
		break;
	case MovableOBJ::UP:
		v = rand() % 9;
		break;
	}
	//update();
}

void MovableOBJ::setYaw(double yaw) {
	this->yaw = yaw;
	//update();
}

void MovableOBJ::setPitch(float pitch) {
	if (pitch > 89) {
		pitch = 89;
	}
	else if (pitch < -89) {
		pitch = -89;
	}
	this->pitch = pitch;
	//update();
}


void MovableOBJ::update() {
	front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front.y = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.z = sin(glm::radians(pitch));
	updateVector();
	updateObb();
}

void MovableOBJ::updateVertical(float deltaTime)
{
	v -= deltaTime * 9.8;
	location.z += deltaTime * v - 9.8 * deltaTime * deltaTime;
	if (location.z <= 0) { 
		location.z = 0; 
		v = 0; 
		jumpAlr = false; 
	}
}
void MovableOBJ::updateVector() {
	front = glm::normalize(front);
	glm::vec3 zAxis = glm::vec3(0, 0, 1);
	right = glm::normalize(glm::cross(front, zAxis));
	up = glm::normalize(glm::cross(right, front));
}

void MovableOBJ::updateObb() {
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, location);
	modelMat = glm::rotate(modelMat, glm::radians(90 - yaw), glm::vec3(0, 0, 1));
	modelMat = glm::rotate(modelMat, glm::radians(-pitch), glm::vec3(0, 1, 0));
	modelMat = modelMat * oriModelMat;

	currentObb = obbs::moveOBB(obb, modelMat);
}

void MovableOBJ::Draw(Shader &shader)
{
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, location);
	modelMat = glm::rotate(modelMat, glm::radians(90 - yaw), glm::vec3(0, 0, 1));
	modelMat = glm::rotate(modelMat, glm::radians(-pitch), glm::vec3(0, 1, 0));
	modelMat = modelMat * oriModelMat;
	shader.setMat4("model", modelMat);
	model.Draw(shader);
	clearLastOps();
}

bool MovableOBJ::detectCollision(OBJ &obj) {
	//only detect, but do nothing
	return obbs::collides(currentObb, obj.currentObb);
}

void MovableOBJ::clearLastOps() {
	for (int i = 0; i < 4; i++) {
		lastOp[i] = false;
	}
}


NPC::NPC(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, glm::mat4 &modelMat, Model mod, double yaw, double pitch, bool anim)
: turnDegree(0), MovableOBJ(loc, front, up, speed, modelMat, mod,yaw,pitch, anim)
{

}

bool NPC::detectCollision(OBJ &obj) {
	bool res = false;
	if (obbs::collides(currentObb, obj.currentObb)) {
		for (int i = 0; i < 4; i++) {
			if (lastOp[i]) {
				res = true;
				switch (i)
				{
				case MovableOBJ::FORWARD:
					move(MovableOBJ::BACKWARD, false); break;
				case MovableOBJ::BACKWARD:
					move(MovableOBJ::FORWARD, false); break;
				case MovableOBJ::LEFT:
					move(MovableOBJ::RIGHT, false); break;
				case MovableOBJ::RIGHT:
					move(MovableOBJ::LEFT, false); break;
				default:
					break;
				}
			}
		}
		move(MovableOBJ::BACKWARD, false);
		std::cout << "collision! " << lastOp[0] << " " << lastOp[1] << " " << lastOp[2] << " " << lastOp[3] << "\n";
		//if the collision is caused by rotate view
		if (!res) {
			glm::vec3 rv = obbs::getRV(currentObb, obj.currentObb);
			location -= speed * glm::normalize(rv);
			res = true;
		}
		update();
	}
	return res;
}

Bullet::Bullet(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, glm::mat4 &modelMat, Model mod, double yaw, double pitch, bool anim)
	: MovableOBJ(loc, front, up, speed, modelMat, mod, yaw, pitch, anim)
{
	//shooted = false;
}

void Bullet::Draw(Shader &shader)
{
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, location);
	modelMat = glm::rotate(modelMat, glm::radians(90 - yaw), glm::vec3(0, 0, 1));
	modelMat = glm::rotate(modelMat, glm::radians(-pitch), glm::vec3(0, 1, 0));
	modelMat = modelMat * oriModelMat;
	shader.setMat4("model", modelMat);
	model.Draw(shader);
	clearLastOps();
}