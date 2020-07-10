#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include "glew.h"
#include "glm.hpp"

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

#define BALL_RADIUS (12.5f)

const glm::vec2 BALL_INITIAL_VELOCITY{400.0f, 400.0f};

class BallObject : public GameObject
{
public:
    GLfloat radius;
    GLboolean stuck;
    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
    glm::vec2 move(GLfloat dt, GLuint window_width, GLuint window_height);
    void reset(glm::vec2 position, glm::vec2 velocity);
};

#endif
