#pragma once
#ifndef LIGHT_H
#include <glm\glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 &a, glm::vec3 &d, glm::vec3 &s);
	~Light();
	void SetAmbient(glm::vec3 &a);
	void SetDiffuse(glm::vec3 &d);
	void SetSpecular(glm::vec3 &s);
	glm::vec3 GetAmbient() const;
	glm::vec3 GetDiffuse() const;
	glm::vec3 GetSpecular() const;
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

#endif // !LIGHT_H
