#pragma once
#ifndef PLAYER_H
#include <vector>
#include <glm\glm.hpp>
#include "MGR.h"
#include "Camera.h"
class Player
{
public:
	Player(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, float sen);
	~Player();
	glm::mat4x4 GetView();
	glm::mat4x4 GetProj();
	void Draw();
private:
	Camera myCamera;
	int score;
};

#endif // !PLAYER_H
