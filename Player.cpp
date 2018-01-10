#include "stdafx.h"
#include <glm/glm.hpp>
#include "Player.h"

Player::Player(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, float sen)
:myCamera(loc, front, up, speed, sen), score(0), model("models/man/male02.obj")
{
	//trick
	////generate obb
	//std::vector<glm::vec3> vertices = {
	//	glm::vec3(0.5f, 0.5f, 0.5f),
	//	glm::vec3(0.5f, 0.5f, -0.5f),
	//	glm::vec3(0.5f, -0.5f, 0.5f),
	//	glm::vec3(0.5f, -0.5f, -0.5f),
	//	glm::vec3(-0.5f, 0.5f, 0.5f),
	//	glm::vec3(-0.5f, 0.5f, -0.5f),
	//	glm::vec3(-0.5f, -0.5f, 0.5f),
	//	glm::vec3(-0.5f, -0.5f, -0.5f)
	//};
	//obb = obbs::getOBB(vertices);

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

	updateOBB();
}
Player::Player():model("null") {

}

Player::~Player()
{
}

glm::mat4x4 Player::GetView() {
	return myCamera.GetView();
}

glm::mat4x4 Player::GetProj() {
	return myCamera.GetProj();
}

bool Player::checkCollision(OBJ &obj) {
	bool res = false;
	if (obbs::collides(currentObb, obj.currentObb)) {
		for (int i = 0; i < 4; i++) {
			if (myCamera.lastOp[i]) {
				res = true;
				switch (i)
				{
				case Camera::FORWARD:
					myCamera.Move(Camera::BACKWARD, false); break;
				case Camera::BACKWARD:
					myCamera.Move(Camera::FORWARD, false); break;
				case Camera::LEFT:
					myCamera.Move(Camera::RIGHT, false); break;
				case Camera::RIGHT:
					myCamera.Move(Camera::LEFT, false); break;
				default:
					break;
				}
			}
		}
		std::cout << "collision! " << myCamera.lastOp << "\n";
		//if the collision is caused by rotate view
		if (!res) {
			//myCamera.Move(Camera::BACKWARD, false);
			glm::vec3 rv = obbs::getRV(currentObb, obj.currentObb);
			myCamera.location -= myCamera.speed * rv;
			res = true;
		}
	}
	return res;
}

void Player::updateOBB() {
	//trick
	//obb.center = myCamera.location;

	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, myCamera.location);
	modelMat = glm::rotate(modelMat, glm::radians(90 - myCamera.yaw), glm::vec3(0, 0, 1));
	//modelMat = glm::rotate(modelMat, glm::radians(-myCamera.pitch), glm::vec3(0, 1, 0));

	modelMat = glm::translate(modelMat, glm::vec3(2, 0, -9.8));
	modelMat = glm::rotate(modelMat, (float)M_PI_2, glm::vec3(0, 0, 1));
	modelMat = glm::rotate(modelMat, (float)M_PI_2, glm::vec3(1, 0, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.05f, 0.05f, 0.05f));
	currentObb = obbs::moveOBB(obb, modelMat);
}

void Player::Move(Camera::MovementDirection dir) {
	myCamera.Move(dir);
}

void Player::Rotate(double xPos, double yPos) {
	myCamera.Rotate(xPos, yPos);
}
void Player::Zoom(double yOff) {
	myCamera.Zoom(yOff);
}

void Player::Draw(Shader& shader) {
	//draw model
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, myCamera.location);
	modelMat = glm::rotate(modelMat, glm::radians(90 - myCamera.yaw), glm::vec3(0, 0, 1));
	modelMat = glm::rotate(modelMat, glm::radians(-myCamera.pitch), glm::vec3(0, 1, 0));
	//modelMat = glm::translate(modelMat, glm::vec3( * (float)2 + glm::vec3(0,0,-9.5));
	modelMat = glm::translate(modelMat, glm::vec3(2, 0, -9.8));

	modelMat = glm::rotate(modelMat, (float)M_PI_2, glm::vec3(0, 0, 1));
	modelMat = glm::rotate(modelMat, (float)M_PI_2, glm::vec3(1, 0, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.05f, 0.05f, 0.05f));
	shader.setMat4("model", modelMat);
	model.Draw(shader);

	//draw white sight
	int Width = 1;
	int hLen = 15;
	glEnable(GL_SCISSOR_TEST);
	glScissor(myCamera.screenWidth / 2 - hLen, myCamera.screenHeight / 2 - Width, hLen - 4, Width);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glScissor(myCamera.screenWidth / 2 + 5, myCamera.screenHeight/2 - Width, hLen - 4, Width);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glScissor(myCamera.screenWidth / 2, myCamera.screenHeight / 2 + 4, Width, hLen -6);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT); 
	glScissor(myCamera.screenWidth / 2, myCamera.screenHeight / 2 - hLen, Width, hLen - 6);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}

void Player::setSpeed(float newSpeed) {
	myCamera.speed = newSpeed;
}

void Player::updateScreenSize(float width, float height) {
	myCamera.updateScreenSize(width, height);
}

//debug
void Player::showOBB() {
	std::cout << "camera center:" << currentObb.center[0] << "," << currentObb.center[1] << "," << currentObb.center[2] << "\t";
	std::cout << "half dimension:" << currentObb.halfDimension[0] << "," << currentObb.halfDimension[1] << "," << currentObb.halfDimension[2] << std::endl;
}

void Player::clearLastOps() {
	myCamera.clearLastOps();
}

glm::vec3 Player::getLoc() {
	return myCamera.getLoc();
}