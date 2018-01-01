#pragma once
#ifndef CAMERA_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
class Camera
{
public:
	enum MovementDirection
	{
		FORWARD, BACKWARD, LEFT, RIGHT
	};
	Camera();
	Camera(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, float sen);
	~Camera();
	glm::vec3 location;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	float pitch;
	float yaw;
	float speed;
	float sensitiveness;
	float fov;
	int screenWidth;
	int screenHeight;
	void Move(MovementDirection dir);
	void Rotate(double xPos, double yPos);
	void Zoom(double yOff);
	glm::mat4 GetView();
	glm::mat4 GetProj();
	void updateScreenSize(int w, int h);
	void UpdateVector();
private:
	double xLastPos;
	double yLastPos;
	bool isFirstMouseMove;
};

#endif // !CAMERA_H
