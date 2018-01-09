#pragma once
#ifndef OBB_H
#include <vector>
#include <glm/glm.hpp>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

namespace obbs {

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

	OBB moveOBB(const OBB &obb, const glm::mat4 &modelMat);

	//detect collision
	bool collides(const OBB& a, const OBB &b);

	//glm::vec3 getRV(const OBB &a, const OBB &b);
	////if collision happens, get overlap distance in the orientation of a or b
	//glm::vec3 getOverlap(const OBB& a, const OBB &b);
}

#endif // !OBB_H
