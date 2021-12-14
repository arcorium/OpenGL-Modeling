#pragma once
#include "Mesh.h"

bool TextureFromFile(const char* path, const std::string& directory, unsigned int* id, bool gamma = false);



class Model
{
	std::vector<Mesh> m_meshes;
	std::unordered_map<std::string, bool> m_loadedTexture;
	std::string m_directory;

public:
	Model(const std::string& path);
	void Draw(Shader& shader);

private:
	void LoadModel(const std::string& path);
	void ProcessNode(aiNode *node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
};

