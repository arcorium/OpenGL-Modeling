#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

class Mesh
{
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	unsigned int m_vao, m_vbo, m_ebo;

public:
	Mesh(const std::vector<Vertex>& mVertices, const std::vector<unsigned>& mIndices,
		const std::vector<Texture>& mTexture);

	void Draw(Shader& shader);

	std::vector<Vertex>& GetVertices();
	std::vector<unsigned int>& GetIndices();
	std::vector<Texture>& GetTextures();
private:
	void SetupMesh();
};

