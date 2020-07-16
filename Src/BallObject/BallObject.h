#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include "glew.h"
#include "glm.hpp"

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

#define BALL_RADIUS ((PLAYER_WIDTH) * 16.6f / 100)

const glm::vec2 BALL_INITIAL_VELOCITY{250.0f, 250.0f};

class BallObject : public GameObject
{
public:
    GLfloat radius;
    GLboolean stuck;
    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
    glm::vec2 move(GLfloat dt, GLuint window_width, GLuint window_height);
};

#endif
