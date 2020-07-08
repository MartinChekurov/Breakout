#ifndef SHADER_H
#define SHADER_H

#include "Errors.h"
#define GLEW_STATIC
#include "glew.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"

class Shader {
public:    
	Shader();
	Shader(const GLchar* vertexFile, const GLchar* fragmentFile);
	
	Error setFiles(const GLchar* vertexFile, const GLchar* fragmentFile);
	Error load();
    Error compile();
    void use() const;

    Error setFloat(const GLchar* name, GLfloat value);
    Error setInteger(const GLchar* name, GLint value);
    Error setVector2f(const GLchar* name, GLfloat x, GLfloat y);
    Error setVector2f(const GLchar* name, const glm::vec2 &value);
    Error setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
    Error setVector3f(const GLchar* name, const glm::vec3 &value);
    Error setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    Error setVector4f(const GLchar* name, const glm::vec4 &value);
    Error setMatrix4(const GLchar* name, const glm::mat4 &matrix);

    GLuint getId() const;

private:
    GLuint id;

	const char* vertexFile;
	const char* fragmentFile;

	const char* vertexCode;
	const char* fragmentCode;

	Error checkCompilation(GLuint shader);
	Error checkLinking(GLuint program);
};

#endif
