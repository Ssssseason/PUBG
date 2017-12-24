#include "stdafx.h"
#include "OBJ.h"


OBJ::OBJ(glm::vec4 &location, std::string &path, bool anim = false) : loc(location), isAnim(anim){
	//model
}

OBJ::~OBJ()
{	
	//todo model
}

void OBJ::Draw(Light &l, glm::mat4x4 &view) {

}

MovableOBJ::MovableOBJ(glm::vec4 &direction, float speed, glm::vec4 &location, std::string &path, bool anim = false): dir(direction), speed(speed), isLived(true), OBJ(location, path, anim) {
}

MovableOBJ::~MovableOBJ() {

}

void MovableOBJ::Translate(glm::mat4x4 &mat) {

}
void MovableOBJ::Scale(glm::mat4x4 &mat) {

}
void MovableOBJ::Rotate(glm::mat4x4 &mat) {

}
bool MovableOBJ::DetectCollision(OBJ &obj) {

}
//keep or remove it
void MovableOBJ::SetState() {

}
bool MovableOBJ::GetState() const {

}
// traverse all obj, detect collisions and update state and location
void MovableOBJ::Update(std::vector<MGR> &managers) {

}

void MovableOBJ::Draw(Light &l, glm::mat4x4 &view) {
	//
}

Bullet::Bullet(glm::vec4 &direction, float speed, glm::vec4 &location, std::string &path, bool anim = false): MovableOBJ(direction, speed, location, path, anim) {

}

NPC::NPC(glm::vec4 &direction, float speed, glm::vec4 &location, std::string &path, bool anim = true) : MovableOBJ(direction, speed, location, path, anim) {

}

