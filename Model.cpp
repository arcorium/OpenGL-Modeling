#include "pch.h"
#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION

// STB
#include "Logger.h"
#include "stb_image.h"

Model::Model(const std::string& path)
	: m_meshes(), m_directory()
{
	//stbi_set_flip_vertically_on_load(1);
	LoadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (uint32_t i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i] .Draw(shader);
	}
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);	// Flip UV because OpenGL is flipping in Y axis

	// Checking if importing is already done successfully
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Logger::Log(LogType::FAILED, "ASSIMP", "Failed to Load Image");
		return;
	}

	m_directory = path.substr(0, path.find_last_of('\\'));

	// Check Material
	Logger::Log(LogType::INFO, "MODEL", "This model has " + std::to_string(scene->mNumMaterials) + " materials");
	// Get meshes from all node starting to root node
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process all current node's meshes (if any)
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];	// Accessing scene meshes with node meshes
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}

	// Do same with the children until there's no children left
	for (uint32_t i = 0;i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Vertex
	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex{};

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}


		if (mesh->HasTextureCoords(0))
		{
			vertex.texCoord.x = mesh->mTextureCoords[0][i].x;		// ASSIMP allowing to have up to 8 texture coordinate in 1 vertex
			vertex.texCoord.y = mesh->mTextureCoords[0][i].y;		// ASSIMP allowing to have up to 8 texture coordinate in 1 vertex
		}
		else
		{
			vertex.texCoord.x = 0.0f;
			vertex.texCoord.y = 0.0f;
		}

		//if (mesh->HasTangentsAndBitangents())
		//{
		//	// Tangent
		//	vertex.tangent.x = mesh->mTangents[i].x;
		//	vertex.tangent.y = mesh->mTangents[i].y;
		//	vertex.tangent.z = mesh->mTangents[i].z;

		//	// Bitangent
		//	vertex.bitangent.x = mesh->mBitangents[i].x;
		//	vertex.bitangent.y = mesh->mBitangents[i].y;
		//	vertex.bitangent.z = mesh->mBitangents[i].z;
		//}


		// Add it to vector
		vertices.push_back(vertex);
	}

	// Indices
	// Take every indices in every face
	for(uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i];

		assert(face.mNumIndices == 3);

		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	// Material (Texture)
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// Diffuse maps

		std::vector<Texture> diffuseMaps = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		// Insert to last index
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// Specular maps

		std::vector<Texture> specularMaps = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
		// Insert to last index
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//// Normal maps

		//std::vector<Texture> normalMaps = LoadMaterialTexture(material, aiTextureType_NORMALS, "texture_normal");

		//if (!normalMaps.empty())
		//	Logger::Log(LogType::INFO, "NORMALS", std::to_string(diffuseMaps.size()));
		//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//// Height maps

		//std::vector<Texture> heightMaps = LoadMaterialTexture(material, aiTextureType_HEIGHT, "texture_height");

		//if (!normalMaps.empty())
		//	Logger::Log(LogType::INFO, "HEIGHT", std::to_string(diffuseMaps.size()));
		//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	}

	return Mesh{ vertices, indices, textures };
}

std::vector<Texture> Model::LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	uint32_t texCount = material->GetTextureCount(type);

	for (uint32_t i = 0; i < texCount; i++)
	{
		aiString str;

		material->GetTexture(type, i, &str);

		// Checking if the texture is already loaded
		bool skip = false;
		for (uint32_t j = 0; j < m_loadedTexture.size(); j++)
		{
			if (m_loadedTexture.contains(str.C_Str()))
			{
				textures.push_back(m_loadedTexture[str.C_Str()]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;

			// Checking if the image is properly loaded and assigned into texture
			texture.id = TextureFromFile(str.C_Str(), m_directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			m_loadedTexture.insert(std::make_pair(texture.path, texture));
			textures.push_back(texture);
		}

	}

	return textures;
}

unsigned TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string fullpath = std::string(directory + "\\" + path);

	int width, height, nChannel;
	unsigned char* data = stbi_load(fullpath.c_str(), &width, &height, &nChannel, NULL);
	if (!data)
	{
		Logger::Log(LogType::FAILED, "Texture", "Load Image " + fullpath);
		exit(-1);
	}

	unsigned int texture;
	glGenTextures(1, &texture);

	GLenum format = GL_RGB;
	if (nChannel == 1)
		format = GL_RED;
	else if (nChannel == 3)
		format = GL_RGB;
	else if (nChannel == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	stbi_image_free(data);
	Logger::Log(LogType::SUCCESS, "Texture", "Load Image " + fullpath);

	return texture;
}