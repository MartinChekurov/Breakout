#include "ResourceManager.h"
#include "Errors.h"
#include "Texture.h"
#include <vector>


static std::vector<Shader> Shaders{1};
static std::vector<Texture2D> Textures{5};

Error ResourceManagerLoadShader(const Shader& shader, enum ResouceManagerShader id)
{
	CHECK_ERR(Shaders.size() <= id, ERR_BOUNDS);
	Shaders.insert(Shaders.begin() + id, shader);
	return ERR_NO_ERR;
}

Shader* ResourceManagerGetShader(enum ResouceManagerShader id)
{
	CHECK_ERR(Shaders.size() <= id, nullptr);
	return &Shaders[id];
}

Error ResourceManagerLoadTexture(const Texture2D& texture, enum ResourceManagerTexture2D id)
{	CHECK_ERR(Textures.size() <= id, ERR_BOUNDS);
	Textures.insert(Textures.begin() + id, texture);
	return ERR_NO_ERR;
}

Texture2D* ResourceManagerGetTexture(enum ResourceManagerTexture2D id)
{
	CHECK_ERR(Textures.size() <= id, nullptr);
	return &Textures[id];
}

void ResourceManagerClear()
{
	GLuint id = 0;
    for (Shader shader : Shaders)
       glDeleteProgram(shader.getId());
    for (Texture2D texture : Textures) {
		id = texture.getId();
        glDeleteTextures(1, &id);
	}
}
