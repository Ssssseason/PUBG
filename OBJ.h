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
	glm::mat4 getModelMat();
	void setModelMat(glm::mat4 &newModelMat);
	obbs::OBB currentObb;
	//debug
	void showOBB();
protected:
	//x,y,z,w
	glm::mat4 modelMat;
	//std::vector<Model> models;
	Model model;
	obbs::OBB obb;
	bool isAnim;
};

class MovableOBJ: public OBJ {
public:
	MovableOBJ(glm::vec4 &direction, float speed, glm::mat4 &modelMat, std::string &path, bool anim);
	virtual ~MovableOBJ();
	void Translate(glm::mat4x4 &mat);
	void Scale(glm::mat4x4 &mat);
	void Rotate(glm::mat4x4 &mat);
	bool DetectCollision(OBJ &obj);
	//keep or remove it
	void SetState();	
	bool GetState() const;
	// traverse all obj, detect collisions and update state and location
	virtual void Update();
	virtual void Draw(Shader &shader);

protected:
	//x,y,z,0
	glm::vec4 dir;
	float speed;
	//should be kept or removed from its manager
	bool isLived;	
};

class Bullet : public MovableOBJ {
public:
	Bullet(glm::vec4 &direction, float speed, glm::mat4 &modelMat, std::string &path, bool anim);
	~Bullet();
};

class NPC : public MovableOBJ {
public:
	NPC(glm::vec4 &direction, float speed, glm::mat4 &modelMat, std::string &path, bool anim);
	~NPC();
	void Run();
	void Stop();
	void SetDirection();
};

#endif // !OBJ_H



