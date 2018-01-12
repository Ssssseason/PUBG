#pragma once
#ifndef DEPTHMAP_H
#include <GL/glew.h>
class DepthMap {
public:
	DepthMap(int width, int height);
	GLuint shadowWidth, shadowHeight;
	GLuint depthMapFBO;
	GLuint depthMap;
};

#endif // !DEPTHMAP_H
