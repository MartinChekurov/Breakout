#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glew.h"
#include "glm.hpp"

#include "Texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	enum MoveDirection {

		STATIC,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

    glm::vec2 position, size, velocity;
    glm::vec3 color;
    GLfloat rotation;

    GLboolean isSolid;
    GLboolean destroyed;
	GLboolean move;

    Texture2D sprite;	

	MoveDirection moveX;
	MoveDirection moveY;

    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity);

    void draw(SpriteRenderer &renderer);
};

#endif
