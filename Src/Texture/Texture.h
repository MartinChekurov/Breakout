
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Errors.h"
#include "glew.h"

class Texture2D
{
public:
	Texture2D();
	Texture2D(const GLchar* file);
	
	Error setFile(const GLchar* file);	
	Error load();
    Error generate();
    void bind() const;

	GLuint getId() const;
	GLuint getWidth() const;
	GLuint getHeight() const;

private:
	GLuint id;
	GLuint width, height;

	const GLchar* textureFile;
	const GLubyte* textureData;
};

#endif
