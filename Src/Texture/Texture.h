
#ifndef TEXTURE_H
#define TEXTURE_H

#include "glew.h"

class Texture2D
{
public:
    GLuint id;
    
    GLuint width, height;
   
    GLuint internalFormat;
    GLuint imageFormat;
   
    GLuint wrapS; 
    GLuint wrapT; 
    GLuint filterMin;
    GLuint filterMax; 
    
    Texture2D();
    void generate(GLuint width, GLuint height, unsigned char* data);
    void bind() const;
};

#endif