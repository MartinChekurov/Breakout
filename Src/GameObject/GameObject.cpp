#include "GameObject.h"
#include "fwd.hpp"

GameObject::GameObject() 
    : position{0,0}, size{0,0}, velocity{0,0}, color{1.0f}, rotation{0}, isSolid{false}, destroyed{false}, move{false}, sprite{}, moveX{GameObject::STATIC}, moveY{GameObject::STATIC}
{

}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity) 
    : position{pos}, size{size}, velocity{velocity}, color{color}, rotation{0.0f}, isSolid{false}, destroyed{false}, move{false}, sprite{sprite}, moveX{GameObject::STATIC}, moveY{GameObject::STATIC}
{

}

void GameObject::draw(SpriteRenderer &renderer)
{
    renderer.drawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}

