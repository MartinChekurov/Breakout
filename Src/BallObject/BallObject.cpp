#include "BallObject.h"
#include "GameObject.h"
#include "Game.h"
#include<iostream>

BallObject::BallObject() 
    : GameObject(), radius(12.5f), stuck(true)  { }

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
    :  GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), radius(radius), stuck(true) { }

glm::vec2 BallObject::move(GLfloat dt, GLuint window_width, GLuint window_height)
{
	glm::vec2 velocity = this->velocity * dt;
    if (!this->stuck)
    {
		if (this->moveX == GameObject::LEFT) {
			if (this->position.x) {
				if (velocity.x > this->position.x) {
					velocity.x  = this->position.x;
				}
				this->position.x -= velocity.x;
			} else {
				this->moveX = GameObject::RIGHT;
			}
		} else if (this->moveX == GameObject::RIGHT) {
			if (this->position.x + this->size.x < window_width) {
				if (this->position.x + velocity.x + this->size.x > window_width) {
					this->position.x = window_width - this->size.x;
					velocity.x = 0;
				}
				this->position.x += velocity.x;
			} else {
				this->position.x = window_width - this->size.x;
				this->moveX = GameObject::LEFT;
			}
		}
		if (this->moveY == GameObject::UP) {
			if (this->position.y) {
				if (velocity.y > this->position.y) {
					velocity.y = this->position.y;
				}
				this->position.y -= velocity.y;
			} else {
				this->moveY = GameObject::DOWN;
			}
		} else if (this->moveY == GameObject::DOWN) {
			this->position.y += velocity.y;
		}
    }
    return this->position;
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity)
{
    this->velocity = velocity;
    this->position = position;
    this->stuck = true;
}
