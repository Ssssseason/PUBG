#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Light.h"
#include "obb.h"
#include "model.h"

class OBJ{
public:
	//path for loading model
	OBJ(glm::mat4 &modelMat, Model mod, bool anim = false);
	virtual ~OBJ();
	virtual void Draw(Shader &shader);
	void setOriModelMat(glm::mat4 &newModelMat);
	obbs::OBB currentObb;
	//debugs
	virtual void showInfo();
	Model model;
protected:
	//x,y,z,w
	glm::mat4 oriModelMat;
	obbs::OBB obb;
	bool isAnim;
	void initOBB(Model &model);
	void updateCurrenOBB(glm::mat4 &modelMat);
};

class MovableOBJ: public OBJ {
public:
	enum MovementDirection
	{
		FORWARD, BACKWARD, LEFT, RIGHT, UP
	};
	MovableOBJ(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed,  glm::mat4 &modelMat, Model mod,double yaw=0, double pitch=0, bool anim = false);
	virtual ~MovableOBJ();
	virtual bool detectCollision(OBJ &obj);
	//virtual bool detectCollision(MovableOBJ &mobj);
	void showInfo();
	void Draw(Shader &shader);

	void setYaw(double yaw);
	double getYaw() { return yaw; }

	void setPitch(float pitch);
	float getPitch() { return pitch; }

	void setLoc(glm::vec3 loc) { location = loc; }
	glm::vec3 getLoc() { return location; }
	
	void setFront(glm::vec3 fron) { front = fron; } //update();
	glm::vec3 getFront() { return front; }

	void move(MovementDirection dir, bool updateOp = true);
	void updateVertical(float deltaTime);
	void update();
	void clearLastOps();
//protected:
	bool lastOp[4];
	bool isLive;	//should be kept or removed from its manager
	glm::vec3 location;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	float pitch;
	float yaw;
	float speed;
	float v;
	bool jumpAlr;
	void updateVector();
	void updateObb();
};

class NPC : public MovableOBJ {
public:
	NPC(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, glm::mat4 &modelMat, Model mod, double yaw, double pitch, bool anim = false);
	bool detectCollision(OBJ &obj);
	//bool detectCollision(MovableOBJ &mobj);
	int turnDegree;
};

class Bullet : public MovableOBJ {
public:
	Bullet(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, glm::mat4 &modelMat, Model mod, double yaw, double pitch, bool anim = false);
	//bool detectCollision(OBJ &obj);
	//bool detectCollision(MovableOBJ &mobj);
	void Draw(Shader &shader);
};




