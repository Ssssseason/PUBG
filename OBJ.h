#pragma once
#ifndef OBJ_H

#include <vector>
#include <glm/glm.hpp>
#include "Light.h"
#include "obb.h"
#include "model.h"

class OBJ{
public:
	//path for loading model
	OBJ(glm::mat4 &modelMat, const std::string &modelPath, bool anim = false);
	virtual ~OBJ();
	virtual void Draw(Shader &shader);
	void setOriModelMat(glm::mat4 &newModelMat);
	obbs::OBB currentObb;
	//debugs
	void showInfo();
protected:
	//x,y,z,w
	glm::mat4 oriModelMat;
	Model model;
	obbs::OBB obb;
	bool isAnim;
	void initOBB(Model &model);
	void updateCurrenOBB(glm::mat4 &modelMat);
};

class MovableOBJ: public OBJ {
public:
	enum MovementDirection
	{
		FORWARD, BACKWARD, LEFT, RIGHT
	};
	MovableOBJ(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, glm::mat4 &modelMat, const std::string &path, bool anim = false);
	virtual ~MovableOBJ();
	virtual bool detectCollision(OBJ &obj);
	//virtual bool detectCollision(MovableOBJ &mobj);
	////keep or remove it
	//void SetState();	
	//bool GetState() const;
	void Draw(Shader &shader);
	void setYaw(double yaw);
	void setPitch(double pitch);
	double getYaw();
	double getPitch();
	void move(MovementDirection dir, bool updateOp = true);
	void update();
	void clearLastOps();
protected:
	bool lastOp[4];
	bool isLive;	//should be kept or removed from its manager
	glm::vec3 location;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	float pitch;
	float yaw;
	float speed;
	void updateVector();
	void updateObb();
};

class NPC : public MovableOBJ {
public:
	NPC(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, glm::mat4 &modelMat, const std::string &path, bool anim = false);
	bool detectCollision(OBJ &obj);
	//bool detectCollision(MovableOBJ &mobj);
};

class Bullet : public MovableOBJ {
public:
	Bullet(glm::vec3 &loc, glm::vec3 &front, glm::vec3 &up, float speed, glm::mat4 &modelMat, const std::string &path, bool anim = false);
	//bool detectCollision(OBJ &obj);
	//bool detectCollision(MovableOBJ &mobj);
	glm::vec3 getLoc();
	glm::vec3 getFront();
};


#endif // !OBJ_H



