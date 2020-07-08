#include "texture.h"
#include "Errors.h"
#include "stb_image.h"

Texture2D::Texture2D()
	: Texture2D{nullptr}
{

}

Texture2D::Texture2D(const GLchar* file)
	: id{0}, width{0}, height{0}, textureFile{file}, textureData{nullptr}
{
    glGenTextures(1, &this->id);
}

Error Texture2D::setFile(const GLchar* file)
{	
	CHECK_ERR(!file, ERR_INV_PAR);
	this->textureFile = file;		
	return ERR_NO_ERR;
}	

Error Texture2D::load()
{
    int width = 0, height = 0, desiderChannels = 0;
	unsigned char* image = nullptr; 
	CHECK_ERR(!this->textureFile, ERR_INV_VAL);
    //stbi_set_flip_vertically_on_load(1);
    image = stbi_load(this->textureFile, &width, &height, &desiderChannels, 4);
	CHECK_ERR(!image, ERR_INV_VAL);
	this->width = width;
	this->height = height;
	this->textureData = image;
	return ERR_NO_ERR;
}

Error Texture2D::generate()
{
	CHECK_ERR(!this->textureData, ERR_INV_VAL);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->textureData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free((void*)this->textureData);
	return ERR_NO_ERR;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->id);
}

GLuint Texture2D::getId() const
{
	return this->id;	
}

GLuint Texture2D::getWidth() const
{
	return this->width;	
}

GLuint Texture2D::getHeight() const
{
	return this->height;	
}
