#ifndef GAME_H
#define GAME_H

#include <vector>

#include "BallObject.h"
#include "Errors.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "glew.h"
#include "glfw3.h"

#include "GameLevel.h"

#define PLAYER_WIDTH  (100)
#define PLAYER_HEIGHT (20)

#define BALL_RADIUS (12.5f)

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
public:
	SpriteRenderer renderer;
	GameObject player;
	BallObject ball;
	std::vector<GameLevel> levels;
	GLuint level = 0;
    GameState state;	
    GLboolean keys[1024];
    GLuint width, height;
    
    Game(GLuint width, GLuint height);
    ~Game();
    
    Error init();
    
    void processInput(GLfloat dt);
    void update(GLfloat dt);
    void render();

private:
	void doCollisions();
};

#endif
