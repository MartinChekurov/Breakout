#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glew.h"
#include "glm.hpp"

#include "Texture.h"
#include "SpriteRenderer.h"

enum ObjectDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class GameObject
{
public:
    glm::vec2 position, size, velocity;
    glm::vec3 color;
    GLfloat rotation;
    GLboolean isSolid;
    GLboolean destroyed;
    Texture2D sprite;	
	ObjectDirection directionX;
	ObjectDirection directionY;
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    void draw(SpriteRenderer &renderer);
	void setDirection();
};

#endif
