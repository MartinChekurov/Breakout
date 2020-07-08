#include "SpriteRenderer.h"
#include<iostream>
SpriteRenderer::SpriteRenderer()
	: shader{}, quadVAO{}
{
	
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::setShader(const Shader& shader)
{
	this->shader = shader;
}

void SpriteRenderer::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos          // Tex
        0.0f, 1.0f,	    0.0f, 1.0f,
        1.0f, 0.0f,		1.0f, 0.0f,
        0.0f, 0.0f,		0.0f, 0.0f,
        0.0f, 1.0f,		0.0f, 1.0f,
        1.0f, 1.0f,		1.0f, 1.0f,
        1.0f, 0.0f,		1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(this->quadVAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat angle, glm::vec3 color)
{
    glm::mat4 model(1.0), translate(1.0), rotate(1.0), scale(1.0);
    translate = glm::translate(translate, glm::vec3(position, 0.0f));
    rotate = glm::rotate(rotate, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    scale = glm::scale(scale, glm::vec3(size, 1.0f));
	model = model * translate * rotate * scale;

    this->shader.use();
    this->shader.setMatrix4("model", model);
    this->shader.setVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
