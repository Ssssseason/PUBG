#pragma once
#ifndef PLAYER_H
#include <vector>
#include <glm/glm.hpp>
#include "MGR.h"
#include "Camera.h"
class Player
{
public:
	Player(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, float sen);
	Player();
	~Player();
	glm::mat4x4 GetView();
	glm::mat4x4 GetProj();
	void Draw(Shader &shader);
	void updateOBB();
	void Move(Camera::MovementDirection dir);
	void Rotate(double xPos, double yPos);
	void Zoom(double yOff);
	bool checkCollision(OBJ &obj);
	void setSpeed(float newSpeed);
private:
	Camera myCamera;
	obbs::OBB obb;
	int score;
};

#endif // !PLAYER_H
