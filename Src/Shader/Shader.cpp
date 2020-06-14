#include "Shader.h"
#include <iostream>

Shader &Shader::use()
{
    glUseProgram(id);
    return *this;
}

GLuint Shader::getId()
{
    return id;
}

void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
    GLuint sVertex, sFragment, gShader;
    
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, SHADER);
    
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, SHADER);

    if (geometrySource) {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(gShader, SHADER);
    }
    
    id = glCreateProgram();
    glAttachShader(id, sVertex);
    glAttachShader(id, sFragment);
    if (geometrySource) {
        glAttachShader(id, gShader);
    }
    glLinkProgram(id);
    checkCompileErrors(id, PROGRAM);
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource) {
        glDeleteShader(gShader);
    }
}

void Shader::setFloat(const GLchar *name, GLfloat value, GLboolean useShader)
{
	if (useShader)
        this->use();
    glUniform1f(glGetUniformLocation(this-> id, name), value);
}

void Shader::setInteger(const GLchar *name, GLint value, GLboolean useShader)
{
	if (useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this-> id, name), value);
}

void Shader::setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this-> id, name), x, y);
}

void Shader::setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader)
{
	if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this-> id, name), value.x, value.y);
}

void Shader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this-> id, name), x, y, z);
}

void Shader::setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader)
{
	if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this-> id, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this-> id, name), x, y, z, w);
}
void Shader::setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader)
{
	if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this-> id, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader)
{
	if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this-> id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(GLuint object, CompileType type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != PROGRAM) {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "\nShader compile error type: " << type << "\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "\nProgram compile error type: " << type << "\n" << infoLog << std::endl;
        }
    }
}