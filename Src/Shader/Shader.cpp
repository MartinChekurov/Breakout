#include "Shader.h"
#include "Errors.h"
#include <fstream>
#include <iostream>
#include <string>

Shader::Shader()
	: Shader{nullptr, nullptr}
{
	
}

Shader::Shader(const GLchar* vFile, const GLchar* fFile)
	: id{0}, vertexFile{vFile}, fragmentFile{fFile}, vertexCode{nullptr}, fragmentCode{nullptr}
{
	
}

Error Shader::setFiles(const GLchar* vertexFile, const GLchar* fragmentFile)
{
	CHECK_ERR(!vertexFile || !fragmentFile, ERR_INV_PAR);
	this->vertexFile = vertexFile;
	this->fragmentFile = fragmentFile;	
	return ERR_NO_ERR;
}

Error Shader::load()
{
	CHECK_ERR(!this->vertexFile || !this->fragmentFile, ERR_INV_VAL);

	std::string vertexCode, fragmentCode, line;
	std::ifstream iVertexFile(this->vertexFile), iFragmentFile(this->fragmentFile);

	CHECK_ERR(iVertexFile.fail() || iFragmentFile.fail(), ERR_ISTREAM);

	while(std::getline(iVertexFile, line)) {

		vertexCode += line;
		vertexCode += "\n";
	}	
	while(std::getline(iFragmentFile, line)) {

		fragmentCode += line;
		fragmentCode += "\n";
	}	

	iVertexFile.close();
	iFragmentFile.close();

	CHECK_ERR(!fragmentCode.length() || !vertexCode.length(), ERR_INV_VAL);

	this->vertexCode = vertexCode.c_str();
	this->fragmentCode = fragmentCode.c_str();
	return ERR_NO_ERR;
}

Error Shader::compile()
{
    GLuint vertex = 0, fragment = 0;
   	Error status = ERR_NO_ERR;

	CHECK_ERR(!this->fragmentCode || !this->vertexCode ||
			  !strlen(this->fragmentCode) || !strlen(this->vertexCode), ERR_INV_VAL);

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &this->vertexCode, NULL);
    glCompileShader(vertex);
	CHECK(this->checkCompilation(vertex));

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &this->fragmentCode, NULL);
    glCompileShader(fragment);
    CHECK(this->checkCompilation(fragment));

    this->id = glCreateProgram();
    glAttachShader(this->id, vertex);
    glAttachShader(this->id, fragment);

    glLinkProgram(this->id);
    CHECK(checkLinking(this->id));

    glDeleteShader(vertex);
    glDeleteShader(fragment);
	return ERR_NO_ERR;
}

void Shader::use() const
{
    glUseProgram(this->id);
}

Error Shader::setFloat(const GLchar *name, GLfloat value)
{
	CHECK_ERR(!name, ERR_INV_PAR);
	this->use();
    glUniform1f(glGetUniformLocation(this-> id, name), value);
	return ERR_NO_ERR;
}

Error Shader::setInteger(const GLchar *name, GLint value)
{
	CHECK_ERR(!name, ERR_INV_PAR);
	this->use();
    glUniform1i(glGetUniformLocation(this-> id, name), value);
	return ERR_NO_ERR;
}

Error Shader::setVector2f(const GLchar *name, GLfloat x, GLfloat y)
{
	CHECK_ERR(!name, ERR_INV_PAR);
	this->use();
    glUniform2f(glGetUniformLocation(this-> id, name), x, y);
	return ERR_NO_ERR;
}

Error Shader::setVector2f(const GLchar *name, const glm::vec2 &value)
{
	CHECK_ERR(!name, ERR_INV_PAR);
	this->use();
    glUniform2f(glGetUniformLocation(this-> id, name), value.x, value.y);
	return ERR_NO_ERR;
}

Error Shader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z)
{
	CHECK_ERR(!name, ERR_INV_PAR);
	this->use();
    glUniform3f(glGetUniformLocation(this-> id, name), x, y, z);
	return ERR_NO_ERR;
}

Error Shader::setVector3f(const GLchar *name, const glm::vec3 &value)
{
	CHECK_ERR(!name, ERR_INV_PAR);
	this->use();
    glUniform3f(glGetUniformLocation(this-> id, name), value.x, value.y, value.z);
	return ERR_NO_ERR;
}

Error Shader::setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	CHECK_ERR(!name, ERR_INV_PAR);
	this->use();
    glUniform4f(glGetUniformLocation(this-> id, name), x, y, z, w);
	return ERR_NO_ERR;
}

Error Shader::setVector4f(const GLchar *name, const glm::vec4 &value)
{
	CHECK_ERR(!name, ERR_INV_PAR);
	this->use();
    glUniform4f(glGetUniformLocation(this-> id, name), value.x, value.y, value.z, value.w);
	return ERR_NO_ERR;
}

Error Shader::setMatrix4(const GLchar *name, const glm::mat4 &matrix)
{
	CHECK_ERR(!name, ERR_INV_PAR);
	this->use();
    glUniformMatrix4fv(glGetUniformLocation(this-> id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	return ERR_NO_ERR;
}

GLuint Shader::getId() const
{
    return this->id;
}

Error Shader::checkCompilation(GLuint shader)
{
    GLint success = 0;
    GLchar error[1024]{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 1024, NULL, error);
		printf("\nCompilation error: %s", error);
		CHECK_ERR(error, ERR_SHADER_COMPILE);
	}
	return ERR_NO_ERR;
}

Error Shader::checkLinking(GLuint program)
{
    GLint success = 0;
    GLchar error[1024]{};
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 1024, NULL, error);
		printf("\nLinking error: %s", error);
		CHECK_ERR(error, ERR_SHADER_LINK);
	}
	return ERR_NO_ERR;
}
