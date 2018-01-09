// PUBG.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ctime>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "OBJ.h"
#include "MGR.h"
#include "Player.h"
#include "Light.h"
#include "shader.h"
#include "skybox.h"

int screenWidth = 800, screenHeight = 600;
Player myPlayer;
float lastFrame, deltaTime, lastScreenshotTime;
bool screenShot = false;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	myPlayer.updateScreenSize(width, height);
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	myPlayer.Rotate(xpos, ypos);
}

void mouseBtnCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			std::cout << "left mouse button pressed!\n";
		}
	}
}

void scrollCallback(GLFWwindow* window, double xOff, double yOff) {
	myPlayer.Zoom(yOff);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		myPlayer.Move(Camera::FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		myPlayer.Move(Camera::LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		myPlayer.Move(Camera::RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		myPlayer.Move(Camera::BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		screenShot = true;
	}
}

bool takeScreenShot(const char* filePath, int screenWidth, int screenHeight) {
	unsigned char* data = (unsigned char*)malloc(screenWidth * screenHeight * 3);
	if (data) {
		for (int i = 0; i < screenHeight; i++) {
			glReadPixels(0, screenHeight - i - 1, screenWidth, 1, GL_RGB, GL_UNSIGNED_BYTE, data + i * screenWidth * 3);
		}
		stbi_write_bmp(filePath, screenWidth, screenHeight, 3, data);
		free(data);
		return true;
	}
	else {
		return false;
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetMouseButtonCallback(window, mouseBtnCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	glm::vec3 playerLoc = glm::vec3(0, -3, 1);
	glm::vec3 playerFront = glm::vec3(0, 1, 0);
	glm::vec3 playerUp = glm::vec3(0, 0, 1);
	myPlayer = Player(playerLoc, playerFront, playerUp, 0.01, 0.1);
	myPlayer.updateScreenSize(screenWidth, screenHeight);

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;

	Shader ourShader("shaders/model_loading.vs", "shaders/model_loading.fs");
	Shader cubeShader("shaders/cubeShader.vs", "shaders/cubeShader.fs");
	OBJ ourFemaleOBJ(model, "models/female/female02.obj");
	OBJ ourTreeOBJ(model, "models/tree/tree_low.obj");
	OBJ ourSceneOBJ(model, "models/terrain/mountains_4.obj");

	SKYBOX mySkyBox;
	mySkyBox.load("skybox");

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		myPlayer.setSpeed(deltaTime * 2.5);
		processInput(window);
		myPlayer.updateOBB();

		// view/projection transformations
		model = glm::mat4();
		proj = myPlayer.GetProj();
		glDepthMask(GL_FALSE);
		view = glm::mat4(glm::mat3(myPlayer.GetView()));
		cubeShader.use();
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", proj);
		cubeShader.setInt("cubemap", 0);
		mySkyBox.draw();

		ourShader.use();	//enable shader before setting uniforms
		// render the loaded model
		model = glm::mat4();
		model = glm::rotate(model, (float)M_PI_2, glm::vec3(1, 0, 0));
		model = glm::translate(model, glm::vec3(0.0f, -2.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		ourFemaleOBJ.setModelMat(model);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 10.f, 0.0f));
		model = glm::rotate(model, (float)M_PI_2, glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(2.f, 2.0f, 2.0f));
		ourTreeOBJ.setModelMat(model);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(10.0f, -20.75f, 0.0f));
		model = glm::rotate(model, (float)M_PI_2, glm::vec3(1, 0, 0));
		//model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		ourSceneOBJ.setModelMat(model);

		myPlayer.checkCollision(ourTreeOBJ);
		myPlayer.checkCollision(ourFemaleOBJ);
		myPlayer.checkCollision(ourSceneOBJ);

		glDepthMask(GL_TRUE);
		view = myPlayer.GetView();
		ourShader.setMat4("projection", proj);
		ourShader.setMat4("view", view);
		ourTreeOBJ.Draw(ourShader);
		ourFemaleOBJ.Draw(ourShader);
		ourSceneOBJ.Draw(ourShader);
		myPlayer.clearLastOps();

		//take at most one screenshot in a second. press F1
		if (screenShot) {
			if (currentFrame - lastScreenshotTime > 1) {
				//debug
				ourTreeOBJ.showOBB();
				ourFemaleOBJ.showOBB();
				ourSceneOBJ.showOBB();
				myPlayer.showOBB();
				std::cout << std::endl;
				//screenshot
				lastScreenshotTime = currentFrame;
				time_t now = time(NULL);
				char filePath[256];
				memset(filePath, 0, 256 * sizeof(char));
				tm *timer = localtime(&now);
				sprintf(filePath, "screenshot/%d%02d%02d%02d%02d%02d.bmp", timer->tm_year + 1900, timer->tm_mon + 1, timer->tm_mday, timer->tm_hour, timer->tm_min, timer->tm_sec);
				if (takeScreenShot(filePath, screenWidth, screenHeight)) {
					std::cout << "screenshot: " << filePath << std::endl;
				}
				else {
					std::cout << "failed to take screenshot!\n";
				}
				screenShot = false;
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
