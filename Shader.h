#pragma once

class Shader
{
	// Program ID
	unsigned int m_ID;
	std::string m_name;
	std::unordered_map<std::string, int> m_cache;
public:
	inline static std::set<std::string> m_false;

	/**
	 * \brief Shader Initialization
	 * \param vertexPath Relative path into vertex file
	 * \param fragmentPath Relative path into fragment file
	 */
	Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = std::string{});

	~Shader();
	/**
	 * \brief Use this program as current shader program
	 */
	void Use();

	void DisUse();

	void SetUniform(const std::string& name, bool value);
	void SetUniform(const std::string& name, int value);
	void SetUniform(const std::string& name, unsigned int value);
	void SetUniform(const std::string& name, float value);
	void SetUniform(const std::string& name, double value);
	void SetUniform(const std::string& name, glm::vec3 vec3);
	void SetUniform(const std::string& name, float value1, float value2, float value3);
	void SetUniform(const std::string& name, double value1, double value2, double value3);
	void SetUniform(const std::string& name, glm::mat4 mat4);
	void SetUniform(const std::string& name, float value1, float value2, float value3, float value4);
	void SetUniform(const std::string& name, double value1, double value2, double value3, double value4);

	/**
	 * \brief Get program shader ID
	 * \return program shader ID
	 */
	unsigned int GetId() const;

	static unsigned GetFalseUniform();

private:
	/**
	 * \brief Read file and return its value
	 * \param filePath Relative path into file
	 * \return value on file
	 */
	std::string ReadFile(const std::string& filePath);

	/**
	 * \brief generating shader with type entered, and attaching the source and compile into it.
	 * \param source shader source
	 * \param type GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	 * \return shader id
	 */
	unsigned int CompileShader(std::string& source, GLenum type);

	/**
	 * \brief Checking if the uniform variable location is already in the map.
	 * \param name uniform variable name
	 * \return is it exist or not
	 */
	bool ProcessCache(const std::string& name);
};

