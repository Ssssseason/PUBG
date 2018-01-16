#include "stdafx.h"
#include "MGR.h"

SceneMRG::SceneMRG() {
	//ground
	Model scene("models/terrain/mountains_4.obj");
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-50.0f, 100.75f, -20.0f));
	model = glm::rotate(model, (float)M_PI_2, glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	OBJ ourSceneOBJ(model, scene);
	objects.push_back(ourSceneOBJ);
}

void SceneMRG::DrawAll(Shader &shader) {
	for (it = objects.begin(); it != objects.end(); ++it) {
		it->Draw(shader);
	}
}

void SceneMRG::DetectCollisionALL(MovableOBJ &mobj) {
	for (it = objects.begin(); it != objects.end(); ++it) {
		mobj.detectCollision(*it);
	}
}

void SceneMRG::DetectCollisionALL(Player &player) {
	for (it = objects.begin(); it != objects.end(); ++it) {
		player.checkCollision(*it);
	}
}

void SceneMRG::ShowInfo() {
	std::cout << "Scene:\n";
	for (it = objects.begin(); it != objects.end(); ++it) {
		it->showInfo();
	}
}

NPCMGR::NPCMGR() {
	frames = 0;
	expNum = 20;
	patrick[0] = Model("models/patrick/patrick1.obj");
	patrick[1] = Model("models/patrick/patrick2.obj");
	patrick[2] = Model("models/patrick/patrick3.obj");
	patrick[3] = Model("models/patrick/patrick4.obj");
	patrick[4] = Model("models/patrick/patrick5.obj");
	patrick[5] = Model("models/patrick/patrick6.obj");
	patrick[6] = Model("models/patrick/patrick7.obj");
	patrick[7] = Model("models/patrick/patrick8.obj");
	patrick[8] = Model("models/patrick/patrick9.obj");
}

void NPCMGR::DrawAll(Shader &shader)
{
	for (it = objects.begin(); it != objects.end(); ++it)
	{
		it->Draw(shader);
	}
}

void NPCMGR::DetectCollisionBullet(Bullet &bullet)
{
	for (it = objects.begin(); it != objects.end(); ++it) {
		if ((*it).isLive && (*it).detectCollision(bullet))
		{
			(*it).isLive = false;
			it->turnDegree = 50;
			bullet.isLive = false;
			//todo:lizi
			std::cout << "count" << std::endl;
			glm::vec3 loc = bullet.location + bullet.front * 2.f;
			std::cout << "blood" << loc.x << "," << loc.y << "," << loc.z << std::endl;
			glm::vec3 front = bullet.front;
			std::cout << "%" << front.x << "," << bullet.front.x << std::endl;
			Blood->addEmitter(*new mVector3d(loc.x, loc.y, loc.z), *new mVector3d(front.x, front.y, front.z));
			std::cout << loc.x << "," << loc.y << "," << loc.z;
			break;
		}

	}
}

void NPCMGR::DetectCollisionALL(MovableOBJ &mobj)
{
	for (it = objects.begin(); it != objects.end(); ++it) {
		mobj.detectCollision(*it);	
	}
}

void NPCMGR::DetectCollisionALL(Player &player) {
	for (it = objects.begin(); it != objects.end(); ++it) {
		player.checkCollision(*it);
	}
}

void NPCMGR::DetectCollisionIn() {
	for (int i = 0; i < objects.size(); ++i) {
		for (int j = i + 1; j < objects.size(); ++j) {
			if (objects[i].isLive && objects[j].isLive) {
				objects[j].detectCollision(objects[i]);

			}
		}
	}
}

void NPCMGR::UpdateAll(float deltaTime)
{
	++frames;
	for (it = objects.begin(); it != objects.end(); /*++it*/)
	{
		if (it->isLive)
		{
			////turn
			//if (it->turnDegree == 0) {
			//	if (rand() % 32 == 0) {
			//		it->turnDegree = rand() % 40 * 4 - 80;
			//	}
			//}
			//else if(it->turnDegree > 0){
			//	it->setYaw(it->getYaw() + 4);
			//	it->turnDegree -= 4;
			//}
			//else if (it->turnDegree < 0) {
			//	it->setYaw(it->getYaw() - 4);
			//	it->turnDegree += 4;
			//}
			////move forward
			//if(rand() % 4 == 0)
			//	it->move(MovableOBJ::FORWARD);
			////jump
			//if (rand() % 32 == 0 && !it->jumpAlr)
			//{
			//	it->move(MovableOBJ::UP);
			//	it->jumpAlr = true;
			//}
			//if (frames % 2) {
			//	it->model = patrick[(frames/2) % 9];
			//}
			//it->updateVertical(deltaTime);
			//it->update();
			++it;
		}
		else {
			if (it->turnDegree) {
				if (it->turnDegree % 2) {
					it->setYaw(it->getYaw() + 10);
					it->setPitch(it->getPitch() + 8);
				}
				else {
					it->setYaw(it->getYaw() - 10);
					it->setPitch(it->getPitch() - 8);
				}
				it->turnDegree--;
				++it;
			}
			else {
				it = objects.erase(it);
			}
		}
	}
	if (frames % 100 == 0 && objects.size() < expNum)			//add more npcs every a period of time
	{
		glm::mat4 model = glm::mat4();
		model = glm::rotate(model, (float)M_PI_2, glm::vec3(0, 0, 1));
		model = glm::rotate(model, (float)M_PI_2, glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(2.f, 2.f, 2.f));
		NPC aa(glm::vec3((rand() / double(RAND_MAX) - 0.5) * 40, (rand() / double(RAND_MAX) - 0.5) * 40, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), 0.1f, model, patrick[0], 0, 0);

		objects.push_back(aa);
	}
}

void NPCMGR::setBlood(ParticleSystem* Blood) {
	this->Blood = Blood;
}

//debug
void NPCMGR::ShowInfo() {
	std::cout << "NPC:\n";
	for (it = objects.begin(); it != objects.end(); ++it) {
		it->showInfo();
	}
}

BulletMGR::BulletMGR() :bulletModel("null") {
}

void BulletMGR::DrawAll(Shader &shader)
{
	for (it = objects.begin(); it != objects.end(); ++it) {
		it->Draw(shader);
	}
}

void BulletMGR::CreateNewBullet(glm::vec3 &loc, glm::vec3 &front, float speed, glm::vec3 &up,double yaw, double pitch )
{
	std::cout << "newBullet:\n";
	if (bulletModel.meshes.size() == 0) {
		bulletModel = Model("models/bullet/bullet2.obj");
	}

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.25f, 0.5f, -1.94f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	Bullet ourBulletOBJ(loc, front, up, speed, model, this->bulletModel, yaw, pitch);

	std::cout << ourBulletOBJ.front.x << " " << ourBulletOBJ.front.y << " " << ourBulletOBJ.front.z << endl;
	ourBulletOBJ.showInfo();
	objects.push_back(ourBulletOBJ);
}

void BulletMGR::DetectCollisionALL(NPCMGR &npc)
{
	for (it = objects.begin(); it != objects.end(); ++it) {
		assert(it->isLive);
		npc.DetectCollisionBullet(*it);
	}
		//if(it->isLive)
		//	npc.DetectCollisionBullet(*it);
}

void BulletMGR::UpdateAll()
{
	for (it = objects.begin(); it != objects.end(); /*++it*/) {
		if (it->isLive) {
			it->move(it->FORWARD, false);
			it->update();
			++it;
		}
		else {
			it = objects.erase(it);
		}
		//std::cout << it->location.x << " " << it->location.y << " " << it->location.z << " " << it->front.x << " " << it->front.y << " " << it->front.z<<endl;
	}
}

void BulletMGR::setBlood(ParticleSystem* Blood) {
	this->Blood = Blood;
}

void BulletMGR::ShowInfo()
{
	std::cout << "BULLET:\n" << "vector size:" << objects.size() << "\n";
	for (it = objects.begin(); it != objects.end(); ++it) {
		it->showInfo();
	}
}