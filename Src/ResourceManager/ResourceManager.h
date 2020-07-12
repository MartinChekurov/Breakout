#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#define GLEW_STATIC
#include "glew.h"

#include "Texture.h"
#include "Shader.h"
#include "Errors.h"

enum ResouceManagerShader
{
	RESOURCE_MANAGER_SHADER_SPRITE,
	RESOURCE_MANAGER_SHADER_PARTICLE
};

enum ResourceManagerTexture2D
{
	RESOURCE_MANAGER_TEXTURE2D_BACKGROUND,
	RESOURCE_MANAGER_TEXTURE2D_AWESOME_FACE,
	RESOURCE_MANAGER_TEXTURE2D_BLOCK,
	RESOURCE_MANAGER_TEXTURE2D_SOLID_BLOCK,
	RESOURCE_MANAGER_TEXTURE2D_PADDLE,
	RESOURCE_MANAGER_TEXTURE2D_PARTICLE
};

Error ResourceManagerLoadShader(const Shader& shader, enum ResouceManagerShader id);
Shader* ResourceManagerGetShader(enum ResouceManagerShader id);

Error ResourceManagerLoadTexture(const Texture2D& texture, enum ResourceManagerTexture2D id);
Texture2D* ResourceManagerGetTexture(enum ResourceManagerTexture2D id);

void ResourceManagerClear();
#endif
