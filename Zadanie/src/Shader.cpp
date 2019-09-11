#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string & fileName)
{
	shaders[EShaderType::VERTEX] = CreateShader(LoadShader(fileName+".vert"), GL_VERTEX_SHADER);
	shaders[EShaderType::FRAGMENT] = CreateShader(LoadShader(fileName+".frag"), GL_FRAGMENT_SHADER);

	program = glCreateProgram();

	for (size_t ii = 0; ii < EShaderType::COUNT; ++ii)
		glAttachShader(program, shaders[ii]);

	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Program linking error");

	glValidateProgram(program);
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Program invalid");

	glDeleteShader(shaders[EShaderType::VERTEX]);
	glDeleteShader(shaders[EShaderType::FRAGMENT]);
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::Bind() const
{
	glUseProgram(program);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

std::string Shader::LoadShader(const std::string & fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::stringstream shaderStream;

	shaderStream << file.rdbuf();
	file.close();

	return shaderStream.str();
}

GLuint Shader::CreateShader(const std::string & text, GLenum shaderType)
{
	const char* shaderCode = text.c_str();
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		std::cout << "Failed to create shader\n";
		return 0;
	}

	glShaderSource(shader, 1, &shaderCode, NULL);

	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "shader compile error");

	return shader;
}


void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cout << std::endl << errorMsg << ": " << error << std::endl;
	}
}

void Shader::SetUniformMat4f(const std::string & name, glm::mat4 & mat)
{
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetUniform3f(const std::string & name, const glm::vec3 & color)
{
	glUniform3f(glGetUniformLocation(program, name.c_str()), color.x, color.y, color.z);
}

