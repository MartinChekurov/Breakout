#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Game.h"

SpriteRenderer  *Renderer;

Game::Game(GLuint width, GLuint height) 
	: state(GAME_ACTIVE), keys(), width(width), height(height) 
{ 

}

Game::~Game()
{
	delete Renderer;
}	

void Game::init()
{
      // Load shaders
    ResourceManager::loadShader("Src/Shaders/sprite.vs", "Src/Shaders/sprite.frag", nullptr, "sprite");
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);

    // Set render-specific controls
	Shader shader = ResourceManager::getShader("sprite");
    Renderer = new SpriteRenderer(shader);  

	// Load textures
    ResourceManager::loadTexture("Res/Textures/awesomeface.png", GL_FALSE, "face");
}

void Game::update(float dt)
{

}

void Game::processInput(float dt)
{

}

void Game::render()
{	
	Texture2D texture = ResourceManager::getTexture("face");
	//todo
    Renderer->DrawSprite(texture, glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}