#include "stdafx.h"
#include "Camera.h"

Camera::Camera() {
	v = 0;
}

Camera::Camera(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, float sen)
	:location(loc), front(front), up(up), speed(speed), sensitiveness(sen), pitch(0), yaw(0), fov(45), isFirstMouseMove(true) {
	UpdateVector();
}

Camera::~Camera()
{
}

void Camera::Move(Camera::MovementDirection dir, bool updateOp) {
	if (updateOp)
		lastOp[dir] = true;
	switch (dir)
	{
	case Camera::FORWARD:
		location += front * speed;
		break;
	case Camera::BACKWARD:
		location -= front * speed;
		break;
	case Camera::LEFT:
		location -= right * speed;
		break;
	case Camera::RIGHT:
		location += right * speed;
		break;
	case Camera::UP:
		v = 9.8;
		break;
	}
	UpdateVector();
}

void Camera::Rotate(double xPos, double yPos) {
	double xOff, yOff;
	if (isFirstMouseMove) {
		xOff = 0;
		yOff = 0;
		isFirstMouseMove = !isFirstMouseMove;
	}
	else {
		xOff = xPos - xLastPos;
		yOff = yLastPos - yPos;
	}
	xLastPos = xPos;
	yLastPos = yPos;
	xOff *= sensitiveness;
	yOff *= sensitiveness;
	yaw += xOff;
	pitch += yOff;
	if (pitch > 89) {
		pitch = 89;
	}
	else if (pitch < -89) {
		pitch = -89;
	}
	front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front.y = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.z = sin(glm::radians(pitch));
	UpdateVector();
}

void Camera::Zoom(double yOff) {
	if (yOff > 0) {
		fov = 10;
	}
	else if (yOff < 0) {
		fov = 45;
	}
}

glm::mat4 Camera::GetView() {
	return glm::lookAt(location, location + front, up);
}

glm::mat4 Camera::GetProj() {
	return glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.f);
}

void Camera::UpdateVector() {
	front = glm::normalize(front);
	glm::vec3 zAxis = glm::vec3(0, 0, 1);
	right = glm::normalize(glm::cross(front, zAxis));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::updateScreenSize(int w, int h) {
	screenHeight = h;
	screenWidth = w;
}

void Camera::clearLastOps() {
	for (int i = 0; i < 4; i++) {
		lastOp[i] = false;
	}
}

