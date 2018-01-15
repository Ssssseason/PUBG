#include "stdafx.h"
#include "skybox.h"
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool SKYBOX::load(std::string path) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenTextures(1, &texture);
	//std::vector<std::string> cubeNames = { "right.jpg", "left.jpg", "bottom.jpg", "top.jpg", "back.jpg", "front.jpg" };
	std::vector<std::string> cubeNames = { "right.bmp", "left.bmp", "top.bmp", "bottom.bmp", "front.bmp", "back.bmp"};
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	unsigned char* data;
	int width, height, channels;
	for (unsigned int i = 0; i < cubeNames.size(); i++)
	{
		data = stbi_load((path + "/" + cubeNames[i]).c_str(), &width, &height, &channels, 0);
		if (data) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			std::cout << "load " << cubeNames[i] << std::endl;
			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load " << cubeNames[i] << std::endl;
			return false;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return true;
};

void SKYBOX::draw() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//clear
	glBindVertexArray(0);
}