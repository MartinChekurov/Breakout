#ifndef GAME_H
#define GAME_H

#include <vector>

#include "glew.h"
#include "glfw3.h"

#include "GameLevel.h"

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Represents the four possible (collision) directions
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// Defines a Collision typedef that represents collision data
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100, 20);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

class Game
{
public:
    GameState state;	
    GLboolean keys[1024];
    GLuint width, height;
    std::vector<GameLevel> Levels;
    GLuint                 Level;
    
    Game(GLuint width, GLuint height);
    ~Game();
    
    void init();
    
    void processInput(GLfloat dt);
    void update(GLfloat dt);
    void render();
    void doCollisions();

    void ResetLevel();
    void ResetPlayer();
};

#endif