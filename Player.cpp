#include "stdafx.h"
#include <glm/glm.hpp>
#include "Player.h"

#define G 9.8;

Player::Player(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, float sen)
:myCamera(loc, front, up, speed, sen), score(0), model("models/player1/player1.obj"), model1("models/player1/player1.obj"), model2("models/player1/player2.obj")
{
	Model model2();

	//init original model matrix
	glm::mat4 modelMat = glm::mat4();
	modelMat = glm::translate(modelMat, glm::vec3(-0.5, 0.28, -6.0));
	modelMat = glm::rotate(modelMat, (float)M_PI_2, glm::vec3(0, 0, 1));
	modelMat = glm::rotate(modelMat, (float)M_PI_2, glm::vec3(1, 0, 0));
	modelMat = glm::scale(modelMat, glm::vec3(0.04f, 0.04f, 0.04f));
	oriModelMat = modelMat;

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
		//std::cout << "collision! " << myCamera.lastOp[0] << " " << myCamera.lastOp[1] << " " << myCamera.lastOp[2] << " " << myCamera.lastOp[3] << "\n";
		//if the collision is caused by rotate view
		if (!res) {
			//myCamera.Move(Camera::BACKWARD, false);
			glm::vec3 rv = obbs::getRV(currentObb, obj.currentObb);
			myCamera.location -= myCamera.speed * glm::normalize(rv);
			res = true;
		}
		updateOBB();
	}
	return res;
}

void Player::updateOBB() {
	glm::mat4 modelMat;
	//modelMat = glm::translate(modelMat, glm::vec3(0,0,40.f));

	modelMat = glm::translate(modelMat, myCamera.location);
	modelMat = glm::rotate(modelMat, glm::radians(90 - myCamera.yaw), glm::vec3(0, 0, 1));
	//modelMat = glm::rotate(modelMat, glm::radians(-myCamera.pitch), glm::vec3(0, 1, 0));
	modelMat = modelMat * oriModelMat;

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

	modelMat = modelMat * oriModelMat;

	shader.setMat4("model", modelMat);
	model.Draw(shader);

	//draw white sight
	int Width = 1;
	int hLen = 15;
	int yOff = 0, xOff = -2;
	glEnable(GL_SCISSOR_TEST);
	glScissor(myCamera.screenWidth / 2 - hLen + xOff, myCamera.screenHeight / 2 - Width + yOff, hLen - 4, Width);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glScissor(myCamera.screenWidth / 2 + 5 + xOff, myCamera.screenHeight/2 - Width + yOff, hLen - 4, Width);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glScissor(myCamera.screenWidth / 2 + xOff, myCamera.screenHeight / 2 + 4 + yOff, Width, hLen -6);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT); 
	glScissor(myCamera.screenWidth / 2 + xOff, myCamera.screenHeight / 2 - hLen + yOff, Width, hLen - 6);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	//clearLastOps();
}

void Player::setSpeed(float newSpeed) {
	myCamera.speed = newSpeed;
}

void Player::updateScreenSize(float width, float height) {
	myCamera.updateScreenSize(width, height);
}

//debug
void Player::showInfo() {
	std::cout << "CAMETA:\n";
	std::cout << "camera center:" << currentObb.center[0] << "," << currentObb.center[1] << "," << currentObb.center[2] << "\t";
	std::cout << "half dimension:" << currentObb.halfDimension[0] << "," << currentObb.halfDimension[1] << "," << currentObb.halfDimension[2] << std::endl;
	std::cout << "location:" << myCamera.location.x << " " << myCamera.location.y << " " << myCamera.location.z << "\n";
	std::cout << "yaw:" << myCamera.yaw << "\t";
	std::cout << "pitch:" << myCamera.pitch << "\n";
	std::cout << myCamera.front.x << " " << myCamera.front.y << " " << myCamera.front.z << "\t";
	std::cout << myCamera.right.x << " " << myCamera.right.y << " " << myCamera.right.z << "\t";
	std::cout << myCamera.up.x << " " << myCamera.up.y << " " << myCamera.up.z << std::endl;

}

void Player::clearLastOps() {
	myCamera.clearLastOps();
}

glm::vec3 Player::getLoc() {
	return myCamera.location;
}

glm::vec3 Player::getFront() {
	return myCamera.front;
}

glm::vec3 Player::getUp() {
	return myCamera.up;
}

float Player::getYaw() {
	return myCamera.yaw;
}

float Player::getPitch() {
	return myCamera.pitch;
}

void Player::updateVertical(float deltaTime)
{
	myCamera.v = myCamera.v - Hahaplus(deltaTime, 9.8);
	myCamera.location.z = myCamera.location.z + Hahaplus(deltaTime, myCamera.v) - Hahaplus(9.8, Hahaplus(deltaTime,deltaTime));
	//if (myCamera.location.z <= 0) { myCamera.location.z = 0; myCamera.v = 0; jump_alr = false; }
	if (myCamera.location.z <= -20) { myCamera.location.z = -20; myCamera.v = 0; jump_alr = false; }
}

