#pragma once
#include "pch.h"

#define MAX_BONE_INFLUENCE 4 

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	int boneIDs[MAX_BONE_INFLUENCE];
	float weights[MAX_BONE_INFLUENCE];
};