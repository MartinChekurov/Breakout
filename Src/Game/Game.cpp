#include "Errors.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Game.h"
#include "GameObject.h"
#include "BallObject.h"
#include "glfw3.h"
#include<iostream>

GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // Collision x-axis?
    bool collisionX = one.position.x + one.size.x >= two.position.x &&
        two.position.x + two.size.x >= one.position.x;
    // Collision y-axis?
    bool collisionY = one.position.y + one.size.y >= two.position.y &&
        two.position.y + two.size.y >= one.position.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}

/*GLint xPenetration(GameObject &one, GameObject &two)
{
	if (one.direction == LEFT)
	{
		one.position.x -= (two.position.x + two.size.x) - one.position.x - 1;
	}
	if (one.direction == RIGHT)
	{
		one.position.x += (one.position.x + one.size.x) - two.position.x - 1;
	}
}

GLint yPenetration(GameObject &one, GameObject &two)
{
	if (one.direction == UP)
	{
		one.position.y -= (two.position.y + two.size.y) - one.position.y - 1;
	}
	if (one.direction == DOWN)
	{
		one.position.y += (one.position.y + one.size.y) - two.position.y - 1;
	}
}*/

void Game::doCollisions()
{	bool collision = false;
	for(GameObject& obj : this->levels[this->level].Bricks) {
		if (!obj.destroyed) {
			if (CheckCollision(this->ball, obj) && (this->ball.directionY == UP || this->ball.directionY == DOWN)) {
				if (!obj.isSolid) {
					obj.destroyed = true;
				}
				std::cout<<"\nCOLLISION";
				collision = true;
			}
		}
	}	
	if (collision == true) {
		std::cout<<"\nCOLLISION1";
		if (this->ball.directionY == UP) {
			this->ball.directionY = DOWN;
		} else {
			this->ball.directionY = UP;
		}
		this->ball.velocity.x = -this->ball.velocity.x;
		this->ball.velocity.y = -this->ball.velocity.y;
	}
	if (CheckCollision(this->ball, this->player) && !this->ball.stuck) {
	    GLfloat centerBoard = this->player.position.x + this->player.size.x / 2;
        GLfloat distance = (this->ball.position.x + this->ball.radius) - centerBoard;
        GLfloat percentage = distance / (this->player.size.x / 2);
        // Then move accordingly
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = this->ball.velocity;
        this->ball.velocity.x = 100.0f * percentage * strength; 
        //Ball->Velocity.y = -Ball->Velocity.y;
        this->ball.velocity = glm::normalize(this->ball.velocity) * glm::length(oldVelocity); // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        // Fix sticky paddle
        this->ball.velocity.y = -1 * abs(this->ball.velocity.y);				
	}
}

Game::Game(GLuint width, GLuint height) 
	: state(GAME_ACTIVE), keys(), width(width), height(height) 
{ 

}

Game::~Game()
{
}	

Error Game::init()
{
	Error status = ERR_NO_ERR;
	Shader shader("Src/Shaders/sprite.vs", "Src/Shaders/sprite.frag");
	glm::mat4 projection = glm::ortho(0.0f, (GLfloat)this->width, (GLfloat)this->height, 0.0f, -1.0f, 1.0f);
	const GLchar* textures[5] = {
		"Res/Textures/background.png",
		"Res/Textures/ball.png",
		"Res/Textures/block.png",
		"Res/Textures/block_solid.png",
		"Res/Textures/paddle.png"
	};
	const ResourceManagerTexture2D texturesIds[5] = {
		RESOURCE_MANAGER_TEXTURE2D_BACKGROUND,
		RESOURCE_MANAGER_TEXTURE2D_AWESOME_FACE,
		RESOURCE_MANAGER_TEXTURE2D_BLOCK,
		RESOURCE_MANAGER_TEXTURE2D_SOLID_BLOCK,
		RESOURCE_MANAGER_TEXTURE2D_PADDLE
	};
	GLuint i = 0;

	CHECK(shader.load());
	CHECK(shader.compile());
	CHECK(shader.setMatrix4("projection", projection));
	CHECK(ResourceManagerLoadShader(shader, RESOURCE_MANAGER_SHADER_SPRITE));

	for (i = 0 ; i < 5 ; i++) {
		Texture2D texture(textures[i]);
		CHECK(texture.load());
		CHECK(texture.generate());
		CHECK(ResourceManagerLoadTexture(texture, texturesIds[i]));
	}
			
	this->renderer.setShader(shader);
	this->renderer.initRenderData();

	GameLevel one, two, three, four;
	one.load("Res/Levels/one.json", this->width, this->height / 2);
	this->levels.push_back(one);
	this->level = 0;
	player.position.x = (this->width - PLAYER_WIDTH) / 2;
	player.position.y = this->height - PLAYER_HEIGHT;
	player.size = glm ::vec2 (PLAYER_WIDTH, PLAYER_HEIGHT);
	player.sprite = *ResourceManagerGetTexture(RESOURCE_MANAGER_TEXTURE2D_PADDLE);
	player.velocity = glm::vec2(700, 500);

	ball.position.x = player.position.x + ((PLAYER_WIDTH - BALL_RADIUS*2) / 2);
	ball.position.y = player.position.y - BALL_RADIUS*2;
	ball.size = glm::vec2 (BALL_RADIUS*2, BALL_RADIUS*2);
	ball.sprite = *ResourceManagerGetTexture(RESOURCE_MANAGER_TEXTURE2D_AWESOME_FACE);
	ball.velocity = glm::vec2(100.0f, -350.0f);
	ball.color = glm::vec3(0.0f, 0.6f, 0.8f);
	return ERR_NO_ERR;
}

void Game::update(float dt)
{
	this->ball.move(dt, this->width);
	this->doCollisions();
}

void Game::processInput(float dt)
{
	GLfloat velocity = 0;
	if (this->state == GAME_ACTIVE) {
		velocity = player.velocity.x * dt;
	    if (this->keys[GLFW_KEY_A]) {
			if (player.position.x) {
				if (velocity > player.position.x) {
					velocity = 0;
				}
				player.position.x -= velocity;
				if (this->ball.stuck) {
					this->ball.position.x -= velocity;	
				}
			}
		}
		if (this->keys[GLFW_KEY_D]) {
			if (player.position.x + PLAYER_WIDTH < this->width) {
				if (player.position.x + velocity + PLAYER_WIDTH > this->width) {
					velocity = 0;
				}
				player.position.x += velocity;
				if (this->ball.stuck) {
					this->ball.position.x += velocity;	
				}
			}	
		}
		if (this->keys[GLFW_KEY_SPACE]) {
			this->ball.stuck = false;
		}
	}
}

void Game::render()
{		
    if (this->state == GAME_ACTIVE) {
        Texture2D texture = *ResourceManagerGetTexture(RESOURCE_MANAGER_TEXTURE2D_BACKGROUND);
		this->renderer.drawSprite(texture, glm::vec2(0,0), glm::vec2(this->width, this->height), 0.0f);
		this->levels[this->level].draw(this->renderer);
		this->player.draw(this->renderer);
		this->ball.draw(this->renderer);
    }
}

