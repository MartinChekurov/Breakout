#include "ResourceManager.h"
#include "stb_image.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;


Shader ResourceManager::loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return shaders[name];
}

Shader ResourceManager::getShader(std::string name)
{
    return shaders[name];
}

Texture2D ResourceManager::loadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
    textures[name] = loadTextureFromFile(file, alpha);
    return textures[name];
}

Texture2D ResourceManager::getTexture(std::string name)
{
    return textures[name];
}

void ResourceManager::clear()
{
    for (auto iter : shaders)
        glDeleteProgram(iter.second.getId());
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.id);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
        
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        
        vertexShaderFile.close();
        fragmentShaderFile.close();
       
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if (gShaderFile != nullptr) {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e) {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();

    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{   
    int desiderChannels = 0;
    Texture2D texture;
    if (alpha) {
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    }

    int width, height;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* image = stbi_load(file, &width, &height, &desiderChannels, 4);
    texture.generate(width, height, image);
    stbi_image_free(image);
    // unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.imageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    // texture.generate(width, height, image);
    // SOIL_free_image_data(image);
    return texture;
}