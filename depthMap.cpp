#include "stdafx.h"
#include "depthMap.h"
DepthMap::DepthMap(int width, int height) {
	shadowWidth = width;
	shadowHeight = height;

	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);//空间用于保存深度值，尚无上传数据
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//超出设置为边缘色（1），深度最大
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);	//与帧缓冲对象绑定
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);	//帧缓冲对象绑定深度纹理，数据渲染到纹理空间
	glDrawBuffer(GL_NONE);	//帧缓冲对象包含颜色缓冲、深度缓冲等
	glReadBuffer(GL_NONE);	//显式说明不用颜色数据：写入颜色缓冲区，读取颜色缓冲区
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	//注意解绑！
}
