#include "stdafx.h"
#include <glm/glm.hpp>
#include "Player.h"

Player::Player(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, float sen)
:myCamera(loc, front, up, speed, sen), score(0)
{
	//generate obb
	std::vector<glm::vec3> vertices = {
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, 0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, 0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f)
	};
	obb = obbs::getOBB(vertices);
	updateOBB();
}
Player::Player() {

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
	if (obbs::collides(obb, obj.currentObb)) {
		for (int i = 0; i < 4; i++) {
			if (myCamera.lastOp[i]) {
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
		//glm::vec3 rv = obbs::getRV(obb, obj.currentObb);
		//myCamera.Move(Camera::BACKWARD);
		//std::cout << rv[0] << " " << rv[1] << " " << rv[2] << std::endl;
		//myCamera.location -= glm::abs(glm::dot(myCamera.front * myCamera.speed, rv)) * rv;
		////double dis = obbs::getOverlap(obb, obj.currentObb);
		////myCamera.location += obbs::getOverlap(obb, obj.currentObb);
		std::cout << "collision! " << myCamera.lastOp << "\n";
		res = true;
	}
	return res;
}

void Player::updateOBB() {
	//trick
	obb.center = myCamera.location;
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
	
}

void Player::setSpeed(float newSpeed) {
	myCamera.speed = newSpeed;
}

void Player::updateScreenSize(float width, float height) {
	myCamera.updateScreenSize(width, height);
}

void Player::showOBB() {
	std::cout << "camera center:" << obb.center[0] << "," << obb.center[1] << "," << obb.center[2] << "\t";
	std::cout << "half dimension:" << obb.halfDimension[0] << "," << obb.halfDimension[1] << "," << obb.halfDimension[2] << std::endl;
}

void Player::clearLastOps() {
	myCamera.clearLastOps();
}