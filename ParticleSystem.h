//
//  ParticleSystem.hpp
//  pubg
//
//  Created by Candy on 15/01/2018.
//  Copyright © 2018 Candy. All rights reserved.
//

#ifndef ParticleSystem_h
#define ParticleSystem_h

#define M_PI        3.14159265358979323846
#define DEGTORAD(d)    ((d * (double)M_PI) / 180.0f);
#define RADTODEG(r)    ((r * 180.0f) /(double)M_PI);
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <ctime>
#include <iostream>
#include <GL/glew.h>
#include "shader.h"
//#include "model.h"
//#include <stdlib.h>
//#include <glut/glut.h>
//#include <time.h>

class mVector3d{
public:
    mVector3d():x(0), y(0), z(0){}
    mVector3d(double x1, double y1, double z1):x(x1), y(y1), z(z1){}
    mVector3d(const mVector3d &v){ x = v.x;  y = v.y;  z = v.z; }
    ~mVector3d(){}
    void operator=(const mVector3d &v){  x = v.x;  y = v.y;  z = v.z; }
    mVector3d operator+(const mVector3d &v){  return mVector3d(x+v.x, y+v.y, z+v.z);  }
    mVector3d operator-(const mVector3d &v){  return mVector3d(x-v.x, y-v.y, z-v.z);  }
    mVector3d operator+(double d){  return mVector3d(x+d, y+d, z+d);  }
    mVector3d operator-(double d){  return mVector3d(x-d, y-d, z-d);  }
    mVector3d operator*(double d){  return mVector3d(x*d, y*d, z*d);  }
public:
    double x, y, z;
};

struct tParticle{
    tParticle   *prev,*next;
    mVector3d    pos;
    mVector3d    direction;
    int          restLife;
    void draw(GLuint VAO, mVector3d location, Shader& bloodShader);
};

class tEmitter
{
public:
    tEmitter();
    ~tEmitter();
    void addParticle();
    void updateParticle(tParticle *particle);
public:
    mVector3d    pos;   //发射器位置
    mVector3d    direction; //发射器喷射主方向
    // 通过球坐标系随机产生指定方向范围内的速度分量
    double       voy_max, voy_min;
    double       vox_max, vox_min;
    double       speed_max, speed_min;
    //粒子设置信息
    tParticle    *ptcPool;
    tParticle    *ptc;  //指向当前活跃粒子链表
    int          totalPtc; // 粒子库粒子总数
    int          curPtc; // 当前粒子数
    int          pfPtc_max, pfPtc_min; // 每一帧添加粒子数变化范围
    int          life_max, life_min; // 粒子初始生命值变化范围
    mVector3d    force; // 三坐标轴速度变化率
};

class ParticleSystem
{
public:
    void addEmitter(mVector3d pos, mVector3d direction);
    void updateEmitter(tEmitter * emitter);
    void renderEmitter(Shader &bloodShader);
    void load();
public:
    std::vector<tEmitter> emitters;
    GLuint VAO, VBO;
    GLfloat vertices[108] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    
};

void TransAndRotate(mVector3d pos, mVector3d destiny);

#endif /* ParticleSystem_h */
