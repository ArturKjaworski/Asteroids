#pragma once

#include<string>

class Shader
{
public:
	Shader(const std::string& fileName);
	~Shader();

	void Bind() const;
	void Unbind() const;


	void SetUniformMat4f(const std::string & name, const glm::mat4 & mat);
	


private:

	enum EShaderType
	{
		  VERTEX = 0
		, FRAGMENT
		, COUNT
	};

	GLuint shaders[EShaderType::COUNT];
	GLuint program;

	std::string LoadShader(const std::string& fileName);
	GLuint CreateShader(const std::string& text, GLenum shaderType);

	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg);

};


