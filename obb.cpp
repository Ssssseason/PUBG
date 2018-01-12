#include "stdafx.h"
#include "obb.h"

//input vertices, output covariance
void obbs::computeCovarianceMat(std::vector<glm::vec3> &vertices, glm::mat3 &covariance) {

	//average
	glm::vec3 avgPos(0, 0, 0);
	std::vector<glm::vec3>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); ++it) {
		avgPos += (*it);
	}
	avgPos /= vertices.size();

	//sub avg
	for (it = vertices.begin(); it != vertices.end(); ++it) {
		(*it) -= avgPos;
	}

	//covariance
	double v;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			v = 0;
			for (it = vertices.begin(); it != vertices.end(); ++it) {
				v += (*it)[i] * (*it)[j];
			}
			covariance[i][j] = covariance[j][i] = v / vertices.size();
		}
	}
}

//input covariance(changed), precision, iteration, output eValue, eVec(t), 
void obbs::jacobiSolver(glm::mat3 &covariance, std::vector<double> &eValue, std::vector<glm::vec3> &eVec, double precision, double iteration) {
	assert(eValue.size() == 3);
	assert(eVec.size() == 3);
	double max;
	int row, col, i, j, itCnt = 0;
	double theta, sinTheta, cosTheta, sin2Theta, cos2Theta, covRow, covCol, covRC, covRI, covCI, eVecRI, eVecCI;
	while (1) {
		//get max value
		max = fabs(covariance[0][1]);
		row = 0;
		col = 1;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if (i != j && fabs(covariance[i][j]) > max) {
					max = fabs(covariance[i][j]);
					row = i;
					col = j;
				}
			}
		}
		//check whether to stop
		if (itCnt > iteration || max < precision) {
			break;
		}
		//jacobi
		if (covariance[row][row] == covariance[col][col]) {
			theta = M_PI / 4;
		}
		else {
			theta = atan(2 * covariance[row][col] / (covariance[row][row] - covariance[col][col])) / 2;
		}
		covRow = covariance[row][row];
		covCol = covariance[col][col];
		covRC = covariance[row][col];
		sinTheta = sin(theta);
		cosTheta = cos(theta);
		sin2Theta = sin(2 * theta);
		cos2Theta = cos(2 * theta);
		covariance[row][row] = covRow * cosTheta * cosTheta + covCol * sinTheta * sinTheta + covRC * sin2Theta;
		covariance[col][col] = covRow * sinTheta * sinTheta + covCol * cosTheta * cosTheta - covRC * sin2Theta;
		covariance[row][col] = covariance[col][row] = (covCol - covRow) * sin2Theta / 2 + covRC * cos2Theta;

		for (i = 0; i < 3; i++) {
			if (i != row && i != col) {
				covRI = covariance[row][i];
				covCI = covariance[col][i];
				covariance[row][i] = covariance[i][row] = covRI * cosTheta + covCI * sinTheta;
				covariance[col][i] = covariance[i][col] = covCI * cosTheta - covRI * sinTheta;
			}
		}
		if (itCnt == 0) {
/*			eVec[row][row] = cosTheta;
			eVec[row][col] = -sinTheta;
			eVec[col][row] = sinTheta;
			eVec[col][col] = cosTheta;	*/		
			eVec[row][row] = cosTheta;
			eVec[col][row] = -sinTheta;
			eVec[row][col] = sinTheta;
			eVec[col][col] = cosTheta;
		}
		else {
			for (i = 0; i < 3; i++) {
/*				eVecIR = eVec[i][row];
				eVecIC = eVec[i][col];
				eVec[i][row] = eVecIR * cosTheta + eVecIC * sinTheta;
				eVec[i][col] = eVecIC * cosTheta - eVecIC * sinTheta;	*/			
				eVecRI = eVec[row][i];
				eVecCI = eVec[col][i];
				eVec[row][i] = eVecRI * cosTheta + eVecCI * sinTheta;
				eVec[col][i] = eVecCI * cosTheta - eVecRI * sinTheta;
			}
		}
		itCnt++;
	}
	for (i = 0; i < 3; i++) {
		eValue[i] = covariance[i][i];
	}
	return;
}

//input&output u, v, w(eVec)
void obbs::schmidtOrthogonal(glm::vec3 &u, glm::vec3 &v, glm::vec3 &w) {
	u = glm::normalize(u);
	v -= glm::dot(u, v) * u;
	v = glm::normalize(v);
	w = glm::cross(u, v);
}

//input vertices, axis, output center, halfDimension
void obbs::computerLen(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &axis, glm::vec3 &center, glm::vec3 &halfDimension) {
	assert(axis.size() == 3);
	std::vector<glm::vec3>::iterator it;
	glm::vec3 maxP(DBL_MIN, DBL_MIN, DBL_MIN);
	glm::vec3 minP(DBL_MAX, DBL_MAX, DBL_MAX);
	for (it = vertices.begin(); it != vertices.end(); ++it) {
		for (int i = 0; i < 3; i++) {
			minP[i] = std::fmin(minP[i], glm::dot((*it), axis[i]));
			maxP[i] = std::fmax(maxP[i], glm::dot((*it), axis[i]));
		}
	}
	halfDimension = (maxP - minP) / 2.0f;
	center = halfDimension + minP;
	center = center.x * axis[0] + center.y * axis[1] + center.z * axis[2];
}

//input vertices, output axis, center, halfDimension
obbs::OBB obbs::getOBB(std::vector<glm::vec3> &vertices) {
	OBB obb;
	for (int i = 0; i < 3; i++) {
		obb.axis[i][i] = 1;
	}
	std::vector<glm::vec3> verticesCP = vertices;
	glm::mat3 covariance;
	std::vector<double> eValue(3);
	computeCovarianceMat(verticesCP, covariance);
	jacobiSolver(covariance, eValue, obb.axis);
	//reorder axis
	int maxIdx = 0;
	for (int i = 1; i < 3; i++) {
		if (eValue[maxIdx] < eValue[i]) {
			maxIdx = i;
		}
	}
	schmidtOrthogonal(obb.axis[maxIdx], obb.axis[(maxIdx + 1) % 3], obb.axis[(maxIdx + 2) % 3]);
	computerLen(vertices, obb.axis, obb.center, obb.halfDimension);
	return obb;
}

obbs::OBB obbs::moveOBB(const OBB &obb, const glm::mat4 &modelMat) {
	std::vector<glm::vec3> points;
	for (int i = 0; i < 3; i++) {
		points.push_back(obb.axis[i] * obb.halfDimension[i] + obb.center);
	}
	glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3 (modelMat)));
	obbs::OBB newObb;
	newObb.center = glm::vec3(modelMat * glm::vec4(obb.center, 1.0f));
	for (int i = 0; i < 3; i++) {
		points[i] = glm::vec3(modelMat * glm::vec4(points[i], 1.0f));
		newObb.axis[i] = points[i] - newObb.center;
		newObb.halfDimension[i] =  sqrt(glm::dot(newObb.axis[i], newObb.axis[i]));
		newObb.axis[i] = glm::normalize(newObb.axis[i]);
	}
	return newObb;
}

bool obbs::collides(const OBB& a, const OBB &b) {
	glm::vec3 axis[15];
	axis[0] = a.axis[0];
	axis[1] = a.axis[1];
	axis[2] = a.axis[2];
	axis[3] = b.axis[0];
	axis[4] = b.axis[1];
	axis[5] = b.axis[2];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			axis[6 + i * 3 + j] = glm::normalize(glm::cross(a.axis[i], b.axis[j]));
		}
	}
	glm::vec3 vecAB = a.center - b.center;
	glm::vec3 halfA[3] = { a.halfDimension.x * a.axis[0], a.halfDimension.y * a.axis[1], a.halfDimension.z * a.axis[2] };
	glm::vec3 halfB[3] = { b.halfDimension.x * b.axis[0], b.halfDimension.y * b.axis[1], b.halfDimension.z * b.axis[2] };
	for (int i = 0; i < 15; i++) {
		double dis = 0;
		for (int j = 0; j < 3; j++) {
			dis += fabs(glm::dot(halfA[j], axis[i])) + fabs(glm::dot(halfB[j], axis[i]));
		}
		if (dis < fabs(glm::dot(axis[i], vecAB)))
			return false;
	}
	return true;
}

glm::vec3 obbs::getRV(const OBB &a, const OBB &b) {
	return glm::normalize(b.center - a.center);
}

//glm::vec3 obbs::getOverlap(const OBB& a, const OBB &b) {
//	glm::vec3 vecAB = a.center - b.center;
//	glm::vec3 norVecAB = glm::normalize(vecAB);
//	glm::vec3 halfA[3] = { a.halfDimension.x * a.axis[0], a.halfDimension.y * a.axis[1], a.halfDimension.z * a.axis[2] };
//	glm::vec3 halfB[3] = { b.halfDimension.x * b.axis[0], b.halfDimension.y * b.axis[1], b.halfDimension.z * b.axis[2] };
//	double dis = 0;
//	//for (int i = 0; i < 3; i++) {
//	//	dis += fabs(glm::dot(halfA[i], orientation)) + fabs(glm::dot(halfB[i], orientation));
//	//}
//	//return fabs(dis - fabs(glm::dot(orientation, vecAB)));
//	for (int i = 0; i < 3; i++) {
//		dis += fabs(glm::dot(halfA[i], norVecAB)) + fabs(glm::dot(halfB[i], norVecAB));
//	}
//	return (float)(dis - fabs(glm::dot(norVecAB, vecAB))) * norVecAB;
//}
