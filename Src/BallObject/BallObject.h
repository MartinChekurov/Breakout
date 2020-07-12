#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include "glew.h"
#include "glm.hpp"

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

#define BALL_RADIUS (20.0f)

const glm::vec2 BALL_INITIAL_VELOCITY{500.0f, 500.0f};

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
