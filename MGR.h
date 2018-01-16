#pragma once
#include <vector>
#include "OBJ.h"
#include "shader.h"
#include "Player.h"
#include "ParticleSystem.h"
class MGR
{
public:
	MGR() {};
	virtual ~MGR() {};
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
	void DetectCollisionBullet(Bullet &bullet);
	void DetectCollisionALL(Player &player);
	void DetectCollisionIn();
	//todo: collide with itself
	void UpdateAll(float deltaTime);
	void ShowInfo();
	std::vector<NPC> objects;
	void setBlood(ParticleSystem* Blood);
private:
	std::vector<NPC>::iterator it;
	Model patrick[9];
	int frames;
	int expNum;
	ParticleSystem* Blood;
};

class BulletMGR : public MGR {
public:
	BulletMGR();
	void DrawAll(Shader &shader);
	void CreateNewBullet(glm::vec3 &loc, glm::vec3 &front, float speed, glm::vec3 &up, double yaw, double pitch);
	void DetectCollisionALL(NPCMGR &npc);
	void DetectCollisionALL(MovableOBJ &mobj) {};
	void DetectCollisionALL(Player &player) {};
	void UpdateAll();
	std::vector<Bullet> objects;
	Model bulletModel;
	void ShowInfo();
	void setBlood(ParticleSystem* Blood);
private:
	ParticleSystem* Blood;
	std::vector<Bullet>::iterator it;
};





//class MGR
//{
//public:
//	MGR() {};
//	virtual ~MGR() {};
//	virtual void DrawAll(Shader &shader) = 0;
//	virtual void DetectCollisionALL(MovableOBJ &mobj) = 0;
//	virtual void DetectCollisionALL(Player &player) = 0;
//	virtual void ShowInfo() = 0;
//};
//
//class SceneMRG : public MGR {
//public:
//	SceneMRG() {};
//	void DrawAll(Shader &shader);
//	void DetectCollisionALL(MovableOBJ &obj);
//	void DetectCollisionALL(Player &player);
//	void ShowInfo();
//	std::vector<OBJ> objects;
//private:
//	std::vector<OBJ>::iterator it;
//};
//
//class NPCMGR : public MGR {
//public:
//	NPCMGR();
//	void DrawAll(Shader &shader);
//	void DetectCollisionALL(MovableOBJ &mobj);
//	void DetectCollisionBullet(Bullet &bullet);
//	void DetectCollisionALL(Player &player);
//	void DetectCollisionIn();
//	//todo: collide with itself
//	void UpdateAll(float deltaTime);
//	void ShowInfo();
//	std::vector<NPC> objects;
//private:
//	std::vector<NPC>::iterator it;
//	Model patrick[9];
//	int frames;
//	int expNum;
//};
//
//class BulletMGR : public MGR {
//public:
//	BulletMGR();
//	void DrawAll(Shader &shader);
//	void CreateNewBullet(glm::vec3 &loc, glm::vec3 &front, float speed, glm::vec3 &up, double yaw, double pitch);
//	void DetectCollisionALL(NPCMGR &npc);
//	void DetectCollisionALL(MovableOBJ &mobj) {};
//	void DetectCollisionALL(Player &player) {};
//	void UpdateAll();
//	std::vector<Bullet> objects;
//	Model bulletModel;
//	void ShowInfo();
//private:
//	std::vector<Bullet>::iterator it;
//};