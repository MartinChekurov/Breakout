/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "GameObject.h"


GameObject::GameObject() 
    : position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), isSolid(false), destroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity) 
    : position(pos), size(size), velocity(velocity), color(color), rotation(0.0f), sprite(sprite), isSolid(false), destroyed(false) { }

void GameObject::draw(SpriteRenderer &renderer)
{
    renderer.drawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}
void GameObject::setDirection()
{
	if (this->velocity.x >= 0) {
		this->directionX = RIGHT;
	} else {
		this->directionX = LEFT;
	}
	if (this->velocity.y >= 0) {
		this->directionY = DOWN;
	} else {
		this->directionY = UP;
	}
}
