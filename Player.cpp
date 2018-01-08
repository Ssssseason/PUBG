#include "stdafx.h"
#include <glm\glm.hpp>
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
	if (obbs::collides(obb, obj.currentObb)) {
		//double dis = obbs::getOverlap(obb, obj.currentObb);
		//myCamera.location += obbs::getOverlap(obb, obj.currentObb);
		myCamera.Move(Camera::BACKWARD);
		//std::cout << "collision!\n";
		return true;
	}
	return false;
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
