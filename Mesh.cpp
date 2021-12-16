#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::vector<Texture>& textures)
	: m_vertices(vertices),
	m_indices(indices),
	m_textures(textures),
	m_vao(0), m_vbo(0), m_ebo(0)
{
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int nDiffuse = 1;
	unsigned int nSpecular = 1;
	unsigned int nNormal = 1;
	unsigned int nHeight = 1;

	// Set index for texture diffuse and specular
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;

		if (m_textures[i].type == "texture_diffuse")
			number = std::to_string(nDiffuse++);
		else if (m_textures[i].type == "texture_specular")
			number = std::to_string(nSpecular++);
		else if (m_textures[i].type == "texture_normal")
			number = std::to_string(nNormal++);
		else if (m_textures[i].type == "texture_height")
			number = std::to_string(nHeight++);

		shader.SetUniform(std::string("material." + m_textures[i].type + number), i);

		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}


	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));

	// Set off
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

std::vector<Vertex>& Mesh::GetVertices()
{
	return m_vertices;
}

std::vector<unsigned int>& Mesh::GetIndices()
{
	return m_indices;
}

std::vector<Texture>& Mesh::GetTextures()
{
	return m_textures;
}

void Mesh::SetupMesh()
{
	// Generate
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	// Bind Vao
	glBindVertexArray(m_vao);
	// Set data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	// Set location
	const GLsizei stride = sizeof(Vertex);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, texCoord)));
	glEnableVertexAttribArray(2);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, bitangent)));
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, boneIDs)));
	//glEnableVertexAttribArray(5);
	//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, weights)));
	//glEnableVertexAttribArray(6);
	
	glBindVertexArray(0);
}

