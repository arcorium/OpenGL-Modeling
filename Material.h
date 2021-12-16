#pragma once

#include "pch.h"

struct Material
{
	glm::vec3 ambientColor = { 0.0f, 0.0f, 0.0f };
	glm::vec3 diffuseColor = { 0.0f, 0.0f, 0.0f };
	glm::vec3 specularColor = { 0.0f, 0.0f, 0.0f };
	float shininess = 0.0f;

	Texture* diffuseTexture = nullptr;
	Texture* specularTexture = nullptr;
};