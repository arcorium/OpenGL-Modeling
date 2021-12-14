#include "pch.h"

#include "Shader.h"

#include "Logger.h"

Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
	: m_ID(glCreateProgram()), m_name(name), m_cache()
{

	std::string vertexSource = ReadFile(vertexPath);
	std::string fragmentSource = ReadFile(fragmentPath);
	std::string geometrySource;
	const unsigned int vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
	const unsigned int fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
	unsigned int geometryShader;


	// Attaching
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);

	if (!geometryPath.empty())
	{
		geometrySource = ReadFile(geometryPath);
		geometryShader = CompileShader(geometrySource, GL_GEOMETRY_SHADER);
		glAttachShader(m_ID, geometryShader);
	}

	// Linking
	glLinkProgram(m_ID);

	// Error Check
	int isSuccess;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &isSuccess);
	if (!isSuccess)
	{
		char log[512];
		glGetProgramInfoLog(m_ID, 512, nullptr, log);
		std::cout << log << std::endl;
		return;
	}

	// Clean Shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (!geometrySource.empty())
		glDeleteShader(geometryShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::Use()
{
	glUseProgram(m_ID);
}

void Shader::DisUse()
{
	glUseProgram(0);
}

void Shader::SetUniform(const std::string& name, bool value)
{
	if (ProcessCache(name))
		glUniform1i(m_cache[name], (int)value);
}

void Shader::SetUniform(const std::string& name, int value)
{
	if (ProcessCache(name))
		glUniform1i(m_cache[name], value);

}

void Shader::SetUniform(const std::string& name, unsigned int value)
{
	if (ProcessCache(name))
		glUniform1ui(m_cache[name], value);

}

void Shader::SetUniform(const std::string& name, float value)
{
	if (ProcessCache(name))
		glUniform1f(m_cache[name], value);

}

void Shader::SetUniform(const std::string& name, double value)
{
	if (ProcessCache(name))
		glUniform1d(m_cache[name], value);

}

void Shader::SetUniform(const std::string& name, glm::vec3 vec3)
{
	if (ProcessCache(name))
	{
		glUniform3fv(m_cache[name], 1, glm::value_ptr(vec3));
		// Transpose means that will rearrange the data that the column become row and the row become column
	}

}

void Shader::SetUniform(const std::string& name, glm::mat4 mat4)
{
	if (ProcessCache(name))
	{
		glUniformMatrix4fv(m_cache[name], 1, GL_FALSE, glm::value_ptr(mat4));
		// Transpose means that will rearrange the data that the column become row and the row become column
	}

}

void Shader::SetUniform(const std::string& name, float value1, float value2, float value3)
{
	if (ProcessCache(name))
		glUniform3f(m_cache[name], value1, value2, value3);

}

void Shader::SetUniform(const std::string& name, float value1, float value2, float value3, float value4)
{
	if (ProcessCache(name))
		glUniform4f(m_cache[name], value1, value2, value3, value4);

}

void Shader::SetUniform(const std::string& name, double value1, double value2, double value3)
{
	if (ProcessCache(name))
		glUniform3d(m_cache[name], value1, value2, value3);

}

void Shader::SetUniform(const std::string& name, double value1, double value2, double value3, double value4)
{

	if (ProcessCache(name))
		glUniform4d(m_cache[name], value1, value2, value3, value4);

}

unsigned Shader::GetId() const
{
	return m_ID;
}

std::string Shader::ReadFile(const std::string& filePath)
{
	std::string result;

	std::ifstream file{ filePath };
	try
	{
		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				result += line + "\n";
			}
		}
	}
	catch (std::fstream::failure& e)
	{
		std::cout << "ERROR : " << e.what() << std::endl;
	}

	if (file.is_open())
		file.close();

	return result;
}

unsigned int Shader::CompileShader(std::string& source, GLenum type)
{
	unsigned int shader = glCreateShader(type);
	char* shaderSrc = source.data();
	glShaderSource(shader, 1, &shaderSrc, nullptr);
	glCompileShader(shader);

	// Check
	int isSuccess;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isSuccess);

	if (!isSuccess)
	{
		char log[512];
		glGetShaderInfoLog(shader, 512, nullptr, log);
		std::cout << log << std::endl;
	}

	return shader;
}

bool Shader::ProcessCache(const std::string& name)
{
	if (!m_cache.contains(name))
	{
		int location = glGetUniformLocation(m_ID, name.c_str());


		if (location == -1)
		{
			return false;
		}
		Logger::Log(LogType::SUCCESS, m_name, "Register new variable : " + name, location);

		m_cache.insert(std::make_pair(name, location));
	}

	return true;
}

