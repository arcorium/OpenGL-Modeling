#pragma once
#include "Mesh.h"

unsigned TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
unsigned char* LoadImg(const std::string& path, int* width, int* height, int* nChannel);


class Model
{

	//std::vector<Vertex> m_vertexs;
	std::vector<Mesh> m_meshes;
	std::unordered_map<std::string, Texture> m_loadedTexture;
	std::string m_directory;

public:
	Model(const std::string& path);
	void Draw(Shader& shader);

private:
	void LoadModel(const std::string& path);
	//void InitMesh(const aiMesh* mesh, const aiScene* scene);
	void ProcessNode(aiNode *node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
};

