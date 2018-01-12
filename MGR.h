#pragma once
#ifndef MGR_H

#include <vector>
#include "OBJ.h"
#include "shader.h"
#include "Player.h"

class MGR
{
public:
	MGR();
	virtual ~MGR();
	virtual void DrawAll(Shader &shader) = 0;
	virtual void DetectCollisionALL(MovableOBJ &mobj) = 0;
	virtual void DetectCollisionALL(Player &player) = 0;
	virtual void ShowInfo() = 0;
};

class SceneMRG : public MGR {
public:
	SceneMRG();
	void DrawAll(Shader &shader);
	void DetectCollisionALL(MovableOBJ &obj);
	void DetectCollisionALL(Player &player);
	void ShowInfo();
	std::vector<OBJ> objects;
private:
	std::vector<OBJ>::iterator it;
};

class NPCMGR : public MGR {
public:
	NPCMGR();
	void DrawAll(Shader &shader);
	void DetectCollisionALL(MovableOBJ &mobj);
	void DetectCollisionALL(Player &player);
	//todo: collide with itself
	void UpdateAll();
	void ShowInfo();
	std::vector<MovableOBJ> objects;
private:
	std::vector<MovableOBJ>::iterator it;
};

class BulletMGR : public MGR {
private:
	void DrawAll(Shader &shader);
	void CreateNewBullet(glm::vec3 &loc, glm::vec3 &front, float speed);
	void DetectCollisionALL(MovableOBJ &mobj);
	void DetectCollisionALL(Player &player);
	void UpdateAll();
	std::vector<MovableOBJ> objects;
private:
	std::vector<MovableOBJ>::iterator it;
};

#endif // !MGR_H
