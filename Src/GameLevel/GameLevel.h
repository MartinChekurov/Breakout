#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <vector>
#include <string>

#include "Errors.h"
#include "glew.h"
#include "glm.hpp"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Json.h"

class GameLevel
{
public:
    std::vector<GameObject> Bricks;

    GameLevel() { }

    Error load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
    void draw(SpriteRenderer &renderer);
    GLboolean isCompleted();

private:
    Error init(JSON_Value* level, GLuint levelWidth, GLuint levelHeight);
};

#endif
