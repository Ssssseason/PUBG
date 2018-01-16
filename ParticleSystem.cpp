//
//  ParticleSystem.cpp
//  pubg
//
//  Created by Candy on 15/01/2018.
//  Copyright © 2018 Candy. All rights reserved.
//

#include "stdafx.h"
#include "ParticleSystem.h"

double Random(){
    return (double)rand()/ (double)RAND_MAX;
}

void RandomDirection(double vox, double voy, mVector3d *direction){
    direction->x = (double) (sin(voy) * sin(vox));
    direction->y = (double) cos(voy);
    direction->z = (double) (sin(voy) * cos(vox));
}

tEmitter::tEmitter(){
}
tEmitter::~tEmitter(){
}

void tParticle::draw(GLuint VAO, mVector3d location, Shader& bloodShader){
    glm::mat4 modelMat = glm::mat4();
    glm::vec3 loc = glm::vec3(location.x, location.y, location.z);
    glm::vec3 scale = glm::vec3(0.005,0.005,0.005);
    
    modelMat = glm::translate(modelMat, loc);
    modelMat = glm::scale(modelMat, scale);
    bloodShader.use();
    bloodShader.setMat4("model", modelMat);
    glBindVertexArray(VAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //clear
    glBindVertexArray(0);
}


//粒子添加器
void tEmitter::addParticle(){
    tParticle *particle;
    double voy, vox, speed;
    if (ptcPool != NULL && curPtc < totalPtc){
        particle = ptcPool;
        ptcPool = ptcPool->next;
        if (ptc != NULL)
            ptc->prev = particle;
        particle->next = ptc;
        particle->prev = NULL;
        ptc = particle;
        
        double degreeY = DEGTORAD(120.0 + 60.0 * Random());
        double degreeXZ = DEGTORAD(360.0 * Random());
        particle->pos = this->pos + mVector3d(0.01 * sin(degreeY) * cos(degreeXZ), 0.01 * (1 - cos(degreeY)), 0.01 * sin(degreeY) * sin(degreeXZ));

        voy = (voy_max - voy_min) * Random();
        vox =  (vox_max - vox_min) * Random();;
        RandomDirection(vox,voy,&particle->direction);
        particle->direction = this->direction + particle->direction;
        speed = speed_min + (speed_max - speed_min) * Random();
        particle->direction = particle->direction * speed;
        particle->restLife = life_min + (life_max - life_min) * Random();
        curPtc++;
    }
}

//粒子更新器
void tEmitter::updateParticle(tParticle *particle){
    if (particle != NULL && particle->restLife > 0){
        particle->pos = particle->pos - particle->direction;
        particle->direction = particle->direction - force;
        particle->restLife--;
    }else if (particle != NULL && particle->restLife == 0){
        // 修改当前粒子的前后粒子的指针
        if (particle->prev != NULL)
            particle->prev->next = particle->next;
        else
            ptc = particle->next;
        if (particle->next != NULL)
            particle->next->prev = particle->prev;
        //回收至粒子库
        particle->next = ptcPool;
        ptcPool = particle;
        curPtc--;
    }
}
//添加发射器
void ParticleSystem::addEmitter(mVector3d pos, mVector3d direction){
    srand((unsigned)time( NULL ));
    tEmitter emitter = *new tEmitter();
    emitter.totalPtc = 1000;
    emitter.ptcPool = (tParticle *)malloc(emitter.totalPtc * sizeof(tParticle));
    for (int i = 0; i < emitter.totalPtc - 1; i++){
        emitter.ptcPool[i].next = &emitter.ptcPool[i + 1];
    }
    emitter.ptcPool[emitter.totalPtc - 1].next = NULL;
    //可改为外部设置
    emitter.pos = pos;
    emitter.direction = direction;
    emitter.voy_max =    DEGTORAD(180.0f);
    emitter.voy_min =    DEGTORAD(0.0f);
    emitter.vox_max =    DEGTORAD(360.0f);
    emitter.vox_min =  DEGTORAD(0.0f);
    emitter.speed_max =  0.02f;
    emitter.speed_min =  0.002f;
    emitter.curPtc    = -1;
    emitter.pfPtc_max    = 400;
    emitter.pfPtc_min    = 300;
    emitter.life_min = 40;
    emitter.life_max = 85;
    emitter.force = mVector3d(0.0f, 0.00f, -0.0005f);
    emitter.ptc     = NULL;                    // NULL TERMINATED LINKED LIST
    emitters.push_back(emitter);
}
// 更新发射器
void ParticleSystem::updateEmitter(tEmitter * emitter){
    tParticle *particle, *next;
    if (emitter->ptc != NULL){
        particle = emitter->ptc;
        while (particle){
            next = particle->next;
            emitter->updateParticle(particle);
            particle = next;
        }
    }else{
        std::cout << "new particles!" << std::endl;
        //随机生成不同个数的粒子
        emitter->curPtc = 0;
        int emits = emitter->pfPtc_min + (emitter->pfPtc_max - emitter->pfPtc_min) * Random();
        for (int i = 0; i < emits; i++)
            emitter->addParticle();
        std::cout << "add particles done" << emitter->curPtc << std::endl;
    }
}

void ParticleSystem::renderEmitter(Shader &bloodShader){
    std::vector<tEmitter>::iterator emitteri;
    int size = emitters.size();
    int index = 0;
	for (emitteri = emitters.begin(); emitteri != emitters.end(); ) {
		tEmitter *emitter = &(*emitteri);
		updateEmitter(emitter);
		if(emitter->curPtc != 0){
			tParticle *particle = emitter->ptc;
			glPointSize(2.0);
			while(particle){
				particle->draw(VAO, particle->pos, bloodShader);
				particle = particle->next;
			}
			++emitteri;
		}
		else
		{
			emitteri = emitters.erase(emitteri);
		}
	}
}

void ParticleSystem::load(){
    std::cout << "<ParticleSystem>load" << std::endl;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void TransAndRotate(mVector3d pos, mVector3d destiny){
    glTranslatef(pos.x, pos.y, pos.z);
    double axis[3];
    axis[0] = destiny.z;
    axis[1] = 0;
    axis[2] = -destiny.x;
    double degree = RADTODEG(acos(destiny.y/sqrt(destiny.x*destiny.x + destiny.y*destiny.y + destiny.z*destiny.z)));
    glRotated(degree, axis[0], axis[1], axis[2]);
}




