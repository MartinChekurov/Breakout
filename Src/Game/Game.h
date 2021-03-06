#ifndef GAME_H
#define GAME_H

#include <vector>

#include "BallObject.h"
#include "Errors.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "ParticleGenerator.h"
#include "glew.h"
#include "glfw3.h"

#include "GameLevel.h"

#define SCREEN_WIDTH (700)
#define SCREEN_HEIGHT (500)
#define PLAYER_WIDTH  ((SCREEN_WIDTH) * 12 / 100)
#define PLAYER_HEIGHT ((SCREEN_HEIGHT) * 5 / 100)

const glm::vec2 PLAYER_INITIAL_VELOCITY{700.0f, 700.0f};

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
	GLuint maxLevels = 0;
    GameState state;	
    GLboolean keys[1024];
    GLuint width, height;
    
    Game(GLuint width, GLuint height);
    ~Game();
    
    Error init();
    
    void processInput(GLfloat dt);
    void update(GLfloat dt);
    void render();
	void reset();
	void resetPlayer();
	void resetBall();

private:
	void doCollisions();
};

#endif
