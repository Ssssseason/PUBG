#pragma once
#ifndef OBB_H
#include <vector>
#include <glm\glm.hpp>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

struct OBB {
	std::vector<glm::vec3> axis = { glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) };
	glm::vec3 center, halfDimension;
};

void computeCovarianceMat(std::vector<glm::vec3> &vertices, glm::mat3 &covariance);

void jacobiSolver(glm::mat3 &covariance, std::vector<double> &eValue, std::vector<glm::vec3> &eVec, double precision = 0.00001, double iteration = 10000);

void schmidtOrthogonal(glm::vec3 &u, glm::vec3 &v, glm::vec3 &w);

void computerLen(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &axis, glm::vec3 &center, glm::vec3 &halfDimension);

//generate OBB
OBB getOBB(std::vector<glm::vec3> &vertices);

//detect collision
bool collides(const OBB& a, const OBB &b);

//if collision happens, get overlap distance in the orientation of a or b
double getOBBOverlap(const OBB& a, const OBB &b, glm::vec3 &orientation);


//class OBB {
//private:
//	std::vector<glm::vec3> axis = {glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) };
//	glm::vec3 center, halfDimension;
//
//public:
//	OBB();
//	void initOBB(std::vector<glm::vec3> &vertices);
//	bool collidesWith(OBB &obb) const;
//	double getOverlap(OBB &obb, glm::vec3 &orientation);
//};

#endif // !OBB_H
