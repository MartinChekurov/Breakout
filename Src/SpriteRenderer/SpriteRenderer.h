#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#define GLEW_STATIC
#include "glew.h"
#include "Shader.h"
#include "Texture.h"

class SpriteRenderer
{
public:
    SpriteRenderer();
    ~SpriteRenderer();

	void setShader(const Shader& shader);
    void initRenderData();
    void drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat angle = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    Shader shader;
    GLuint quadVAO;
};

#endif
