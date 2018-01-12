#include "stdafx.h"
#include "OBJ.h"


OBJ::OBJ(glm::mat4 &modelMat, const std::string &modelPath, bool anim):
oriModelMat(modelMat), model(modelPath)
{
	//generate obb
	initOBB(model);
	updateCurrenOBB(modelMat);
	//currentObb = obbs::moveOBB(obb, oriModelMat);
	//std::vector<glm::vec3> vertices;
	//std::vector<Vertex>::iterator vtxIter;
	//std::vector<Mesh>::iterator meshIter;
	//for (meshIter = model.meshes.begin(); meshIter != model.meshes.end(); ++meshIter) {
	//	for (vtxIter = meshIter->vertices.begin(); vtxIter != meshIter->vertices.end(); ++vtxIter) {
	//		vertices.push_back(vtxIter->Position);
	//	}
	//}
	//obb = obbs::getOBB(vertices);
	//currentObb = obb;
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
	//currentObb = obbs::moveOBB(obb, newModelMat);
}

void OBJ::showInfo() {
	std::cout << "current obb center:" << currentObb.center[0] << "," << currentObb.center[1] << "," << currentObb.center[2] << "\t";
	std::cout << "half dimension:" << currentObb.halfDimension[0] << "," << currentObb.halfDimension[1] << "," << currentObb.halfDimension[2] << std::endl;
	//std::cout << "org obb center:" << obb.center[0] << "," << obb.center[1] << "," << obb.center[2] << "\t";
	//std::cout << "half dimension:" << obb.halfDimension[0] << "," << obb.halfDimension[1] << "," << obb.halfDimension[2] << std::endl;
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

MovableOBJ::MovableOBJ(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, glm::mat4 &modelMat, const std::string &path, bool anim)
	: location(loc), front(front), up(up), speed(speed), pitch(0), yaw(0), isLive(true), OBJ(modelMat, path, anim)
{
	//generate obb
	initOBB(model);
	update();
}

MovableOBJ::~MovableOBJ() {

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
	}
	update();
	//updateVector();
}

void MovableOBJ::setYaw(double yaw) {
	this->yaw = yaw;
	update();
}

void MovableOBJ::setPitch(double pitch) {
	if (pitch > 89) {
		pitch = 89;
	}
	else if (pitch < -89) {
		pitch = -89;
	}
	this->pitch = pitch;
	update();
}
double MovableOBJ::getYaw() {
	return yaw;
}
double MovableOBJ::getPitch() {
	return pitch;
}

void MovableOBJ::update() {
	front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front.y = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.z = sin(glm::radians(pitch));
	updateVector();
	updateObb();
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

void MovableOBJ::Draw(Shader &shader) {
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

//bool MovableOBJ::detectCollision(MovableOBJ &mobj) {
//	//only detect, but do nothing
//	return obbs::collides(currentObb, mobj.currentObb);
//}

void MovableOBJ::clearLastOps() {
	for (int i = 0; i < 4; i++) {
		lastOp[i] = false;
	}
}

////keep or remove it
//void MovableOBJ::SetState() {
//
//}
//bool MovableOBJ::GetState() const {
//	return true;
//}

NPC::NPC(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, glm::mat4 &modelMat, const std::string &path, bool anim) 
: MovableOBJ(loc, front, up, speed, modelMat, path, anim)
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

//bool NPC::detectCollision(MovableOBJ &mobj) {
//	return false;
//}

Bullet::Bullet(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, glm::mat4 &modelMat, const std::string &path, bool anim)
	: MovableOBJ(loc, front, up, speed, modelMat, path, anim)
{

}

glm::vec3 Bullet::getLoc() {
	return location;
}

glm::vec3 Bullet::getFront() {
	return front;
}

