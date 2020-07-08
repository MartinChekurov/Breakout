#include "BallObject.h"
#include<iostream>

BallObject::BallObject() 
    : GameObject(), radius(12.5f), stuck(true)  { }

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
    :  GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), radius(radius), stuck(true) { }

glm::vec2 BallObject::move(GLfloat dt, GLuint window_width)
{
    // If not stuck to player board
    if (!this->stuck)
    {
        // Move the ball
        this->position += this->velocity * dt;
        
        // Then check if outside window bounds and if so, reverse velocity and restore at correct position
        if (this->position.x <= 0.0f)
        {
            this->velocity.x = -this->velocity.x;
            this->position.x = 0.0f;
        }
        else if (this->position.x + this->size.x >= window_width)
        {
            this->velocity.x = -this->velocity.x;
            this->position.x = window_width - this->size.x;
        }
        if (this->position.y <= 0.0f)
        {
            this->velocity.y = -this->velocity.y;
            this->position.y = 0.0f;
        }
    }
	this->setDirection();
    return this->position;
}

// Resets the ball to initial Stuck position (if ball is outside window bounds)
void BallObject::reset(glm::vec2 position, glm::vec2 velocity)
{
    this->position = position;
    this->velocity = velocity;
    this->stuck = true;
}
