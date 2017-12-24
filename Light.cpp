#include "stdafx.h"
#include "Light.h"

Light::Light() {

}

Light::~Light() {

}

Light::Light(glm::vec3 &a, glm::vec3 &d, glm::vec3 &s):ambient(a),diffuse(d),specular(s) {
}

void Light::SetAmbient(glm::vec3 &a) {
	this->ambient = a;
}

void Light::SetDiffuse(glm::vec3 &d) {
	this->diffuse = d;
}

void Light::SetSpecular(glm::vec3 &s) {
	this->specular = s;
}

glm::vec3 Light::GetAmbient() const {
	return this->ambient;
}

glm::vec3 Light::GetDiffuse() const {
	return this->diffuse;
}

glm::vec3 Light::GetSpecular() const {
	return this->specular;
}