#pragma once
#ifndef PLAYER_H
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Camera.h"
#include "shader.h"
#include "OBJ.h"
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
	void updateScreenSize(float width, float height);
	void showInfo();//debug
	void clearLastOps();
	glm::vec3 getLoc();
	glm::vec3 getFront();
private:
	glm::mat4 oriModelMat;
	Camera myCamera;
	Model model;
	obbs::OBB obb;
	obbs::OBB currentObb;
	int score;
};

#endif // !PLAYER_H
