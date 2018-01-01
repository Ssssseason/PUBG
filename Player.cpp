#include "stdafx.h"
#include <glm\glm.hpp>
#include "Player.h"

Player::Player(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, float sen)
:myCamera(loc, front, up, speed, sen), score(0)
{
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

void Player::Draw() {
	
}

