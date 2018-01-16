
#include "stdafx.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h> 
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
#include "depthMap.h"
#include "ParticleSystem.h"

BulletMGR bulletMgr;
Player myPlayer;
ParticleSystem Blood;

int screenWidth = 1280, screenHeight = 800;
float lastFrame, deltaTime, lastScreenshotTime;
bool enScreenShot = false;
bool isMidNight = false;
int lasttime = 0;
float lightIntensity = 1.0f;

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
			glm::vec3 loc = myPlayer.getLoc();
			glm::vec3 front = myPlayer.getFront();
			glm::vec3 up = myPlayer.getUp();
			//std::cout << "location:" << loc[0] << " " << loc[1] << " " << loc[2] << "\t";
			std::cout << front.x << " " << front.y << " " << front.z << endl;
			bulletMgr.CreateNewBullet(loc, front, 0.5f, up, myPlayer.getYaw(), myPlayer.getPitch());
			myPlayer.model = myPlayer.model2;
			lasttime = 3;
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
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (!myPlayer.jump_alr)
		{
			myPlayer.Move(Camera::UP);
			myPlayer.jump_alr = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		enScreenShot = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		isMidNight = !isMidNight;
	}
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
		lightIntensity -= 0.01f;
		if (lightIntensity < 0) {
			lightIntensity = 0;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
		lightIntensity += 0.01f;
		if (lightIntensity > 1) {
			lightIntensity = 1;
		}
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

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "PUBG", NULL, NULL);
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

	srand((unsigned)time(NULL));

	glEnable(GL_DEPTH_TEST);

	//initialize player position and view direction
	glm::vec3 playerLoc = glm::vec3(0, -3, 0);
	glm::vec3 playerFront = glm::vec3(0, 1, 0);
	glm::vec3 playerUp = glm::vec3(0, 0, 1);
	myPlayer = Player(playerLoc, playerFront, playerUp, 0.05, 0.1);
	myPlayer.updateScreenSize(screenWidth, screenHeight);

	//load shaders
	Shader cubeShader("shaders/cubeShader.vs", "shaders/cubeShader.fs");
	Shader depthMapShader("shaders/depthMapShader.vs", "shaders/depthMapShader.fs");
	Shader modelShadowShader("shaders/modelShadowShader.vs", "shaders/modelShadowShader.fs");
	Shader bloodShader("shaders/bloodShader.vs", "shaders/bloodShader.fs");

	//initialize managers
	SceneMGR sceneMgr;
	NPCMGR npcMgr;
	//load blood system
	Blood.load();
	//set blood system in managers
	npcMgr.setBlood(&Blood);
	bulletMgr.setBlood(&Blood);

	SKYBOX daySkyBox("skybox/daytime"), nightSkyBox("skybox/night");

	DepthMap myDepthMap(4098, 4098);

	glm::mat4 model, view, proj;
	glm::mat4 lightProj, lightView, lightSpaceMat;

	glm::vec3 lightPos;
	glm::vec3 viewPos;
	int frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//balance moving speed
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		myPlayer.setSpeed((float)deltaTime * 12.5);

		//update all objs
		//process user input: move, rotate, zoom, shot, screenshot, etc.
		processInput(window);
		//manually handle it because of multikey
		myPlayer.updateVertical(deltaTime);
		myPlayer.updateOBB();

		//update npcs and bullets
		npcMgr.UpdateAll(deltaTime);
		bulletMgr.UpdateAll();

		//check collisions for NPC, bullet, player
		sceneMgr.DetectCollisionALL(myPlayer);
		npcMgr.DetectCollisionALL(sceneMgr);
		npcMgr.DetectCollisionALL(myPlayer);
		npcMgr.DetectCollisionIn();
		bulletMgr.DetectCollisionALL(npcMgr);

		//render all
		//render skybox
		model = glm::mat4();
		proj = myPlayer.GetProj();
		glDepthMask(GL_FALSE);
		view = glm::mat4(glm::mat3(myPlayer.GetView()));
		cubeShader.use();
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", proj);
		cubeShader.setFloat("lightIntensity", lightIntensity);
		cubeShader.setInt("cubemap", 0);

		//render skybox and set light attrs
		if (isMidNight) {
			nightSkyBox.draw();
			lightPos = myPlayer.getLoc();
			lightView = myPlayer.GetView();
			lightProj = glm::perspective(glm::radians(90.0f), 1.f, 1.f, 100.f);
		}
		else {
			daySkyBox.draw();
			lightPos = glm::vec3(0, 1, -1);
			lightView = glm::lookAt(glm::vec3(0, -5, 20), glm::vec3(0.0f), glm::vec3(0.0, 10.0, 0.0));
			lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.f);	//平行
		}
		glDepthMask(GL_TRUE);

		//render blood
		view = myPlayer.GetView();
		proj = myPlayer.GetProj();
		viewPos = myPlayer.getLoc();
		bloodShader.use();
		bloodShader.setMat4("view", view);
		bloodShader.setMat4("projection", proj);
		Blood.renderEmitter(bloodShader);

		//render depth
		lightSpaceMat = lightProj * lightView;
		depthMapShader.use();
		depthMapShader.setMat4("lightSpaceMatrix", lightSpaceMat);
		glViewport(0, 0, myDepthMap.shadowWidth, myDepthMap.shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, myDepthMap.depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		sceneMgr.DrawAll(depthMapShader);
		npcMgr.DrawAll(depthMapShader);
		bulletMgr.DrawAll(depthMapShader);
		myPlayer.Draw(depthMapShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	//解绑！

		//formally render all
		view = myPlayer.GetView();
		viewPos = myPlayer.getLoc();
		glViewport(0, 0, screenWidth, screenHeight);
		modelShadowShader.use();	//enable shader before setting uniforms
		modelShadowShader.setMat4("projection", proj);
		modelShadowShader.setMat4("view", view);
		modelShadowShader.setVec3("viewPos", viewPos);
		modelShadowShader.setVec3("lightPos", lightPos);
		modelShadowShader.setVec3("lightColor", glm::vec3(lightIntensity));
		modelShadowShader.setBool("midNight", isMidNight);
		modelShadowShader.setVec3("front", myPlayer.getFront());
		modelShadowShader.setMat4("lightSpaceMatrix", lightSpaceMat);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myDepthMap.depthMap);
		modelShadowShader.setInt("depthMap", 0);

		sceneMgr.DrawAll(modelShadowShader);
		npcMgr.DrawAll(modelShadowShader);
		bulletMgr.DrawAll(modelShadowShader);
		myPlayer.Draw(modelShadowShader);

		myPlayer.clearLastOps();

		if (lasttime > 0)
			lasttime--;
		if (lasttime == 0)
			myPlayer.model = myPlayer.model1;

		//take at most one screenshot in a second. press F1
		if (enScreenShot) {
			if (currentFrame - lastScreenshotTime > 1) {
				//debug
				sceneMgr.ShowInfo();
				npcMgr.ShowInfo();
				bulletMgr.ShowInfo();
				myPlayer.showInfo();
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
				enScreenShot = false;
			}
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
