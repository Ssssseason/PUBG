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
	glm::vec3 getUp();
	float getYaw();
	float getPitch();
	void updateVertical(float deltaTime);
	bool jump_alr;
	glm::mat4 oriModelMat;
	Camera myCamera;
	Model model;
	Model model1;
	Model model2;
	obbs::OBB obb;
	obbs::OBB currentObb;
private:
	float Hahaplus(float a1, float a2)
	{
		return a1*a2;
	}
	int score;
};

#endif // !PLAYER_H
