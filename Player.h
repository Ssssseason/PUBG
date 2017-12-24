#pragma once
#ifndef PLAYER_H
#include <vector>
#include <glm\glm.hpp>
#include "MGR.h"
#include "Camera.h"
class Player
{
public:
	Player();
	~Player();
	void Shoot();
	glm::mat4x4 GetView();
	void Update(std::vector<MGR> &managers);
private:
	Camera myCamera;
};

#endif // !PLAYER_H
