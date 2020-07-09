#include "Errors.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Game.h"
#include "GameObject.h"
#include "BallObject.h"
#include "Texture.h"
#include "fwd.hpp"
#include "glfw3.h"
#include<iostream>

GLfloat CheckXCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
	GLfloat penetration = 0.0f;
	if (one.moveX == GameObject::LEFT) {
		if (one.position.x <= two.position.x + two.size.x &&
			one.position.x >= two.position.x) {
			penetration = (two.position.x + two.size.x) - one.position.x;
		}
	}
	if (one.moveX == GameObject::RIGHT) {
		if (one.position.x + one.size.x >= two.position.x &&
			one.position.x + one.size.x <= two.position.x + two.size.x) {
			penetration = (one.position.x + one.size.x) - two.position.x;
		}
	}
	return penetration;
}

GLfloat CheckYCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
	GLfloat penetration = 0.0f;
	if (one.moveY == GameObject::UP) {
		if (one.position.y <= two.position.y + two.size.y &&
			one.position.y >= two.position.y) {
			penetration = (two.position.y + two.size.y) - one.position.y;
		}
	}
	if (one.moveY == GameObject::DOWN) {
		if (one.position.y + one.size.y >= two.position.y &&
			one.position.y + one.size.y <= two.position.y + two.size.y) {
			penetration = (one.position.y + one.size.y) - two.position.y;
		}
	}
	return penetration;
}

void Game::doCollisions()
{	bool collision = false;
	GLfloat xcollision = 0.0f, ycollosion = 0.0f;
	for(GameObject& obj : this->levels[this->level].Bricks) {
		if (!obj.destroyed) {
			xcollision = CheckXCollision(this->ball, obj);
			ycollosion = CheckYCollision(this->ball, obj);
			if (xcollision && ycollosion) {
				if (!obj.isSolid) {
					obj.destroyed = true;
				}
				collision = true;
				std::cout << "\nballx1: " <<this->ball.position.x << " ballx2: " << this->ball.position.x + this->ball.size.x;
				std::cout << "\nbally1: " <<this->ball.position.y << " bally2: " << this->ball.position.y + this->ball.size.y;
				std::cout << "\ntilex1: " <<obj.position.x << " tilex2: " << obj.position.x + obj.size.x;
				std::cout << "\ntiley1: " <<obj.position.y << " tiley2: " << obj.position.y + obj.size.y;
				std::cout << "\nxcollision: " << xcollision << " ycollosion: " << ycollosion;
				if (this->ball.moveY == GameObject::UP) {
					this->ball.position.y += ycollosion + 0.0001f;
				} else {
					this->ball.position.y -= ycollosion + 0.0001f;
				}
				if (this->ball.moveX == GameObject::LEFT) {
					this->ball.position.x += xcollision + 0.0001f;
				} else {
					this->ball.position.x -= xcollision + 0.0001f;
				}
			}
		}
	}	
	if (collision == true) {
		if (this->ball.moveY == GameObject::UP) {
			this->ball.moveY = GameObject::DOWN;
		} else {
			this->ball.moveY = GameObject::UP;
		}
		if (this->ball.moveX == GameObject::LEFT) {
			this->ball.moveX = GameObject::RIGHT;
		} else {
			this->ball.moveX = GameObject::LEFT;
		}
		//this->ball.velocity.x = -this->ball.velocity.x;
		//this->ball.velocity.y = -this->ball.velocity.y;
	}
	xcollision = CheckXCollision(this->ball, this->player);
	ycollosion = CheckYCollision(this->ball, this->player);
	if (xcollision && ycollosion && !this->ball.stuck) {
		if (this->ball.moveY == GameObject::UP) {
			this->ball.moveY = GameObject::DOWN;
		} else {
			this->ball.moveY = GameObject::UP;
		}
		if (this->ball.moveX == GameObject::LEFT) {
			this->ball.moveX = GameObject::RIGHT;
		} else {
			this->ball.moveX = GameObject::LEFT;
		}
	    GLfloat centerBoard = this->player.position.x + this->player.size.x / 2;
        GLfloat distance = (this->ball.position.x + this->ball.radius) - centerBoard;
        GLfloat percentage = distance / (this->player.size.x / 2);
        GLfloat strength = 2.0f;
        this->ball.velocity.x = 100.0f * percentage * strength; 
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

	GameLevel levelOne;
	CHECK(levelOne.load("Res/Levels/one.json", this->width, this->height / 2));
	this->levels.push_back(levelOne);
	this->level = 0;

	player.position.x = (this->width - PLAYER_WIDTH) / 2;
	player.position.y = this->height - PLAYER_HEIGHT;
	player.size = glm ::vec2 (PLAYER_WIDTH, PLAYER_HEIGHT);
	Texture2D* playerTexture = ResourceManagerGetTexture(RESOURCE_MANAGER_TEXTURE2D_PADDLE);
	CHECK_ERR(!playerTexture, ERR_INV_VAL);
	player.sprite = *playerTexture;
	player.velocity = glm::vec2(700, 500);

	ball.position.x = player.position.x + ((PLAYER_WIDTH - BALL_RADIUS*2) / 2);
	ball.position.y = player.position.y - BALL_RADIUS*2;
	ball.size = glm::vec2 (BALL_RADIUS*2, BALL_RADIUS*2);
	Texture2D* ballTexture = ResourceManagerGetTexture(RESOURCE_MANAGER_TEXTURE2D_AWESOME_FACE);
	CHECK_ERR(!ballTexture, ERR_INV_VAL);
	ball.sprite = *ballTexture;
	ball.velocity = glm::vec2(100.0f, 350.0f);
	ball.color = glm::vec3(0.0f, 0.6f, 0.8f);

	return ERR_NO_ERR;
}

void Game::update(float dt)
{
	this->ball.move(dt, this->width, this->height);
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
					velocity  = player.position.x;
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
					player.position.x = this->width - PLAYER_WIDTH;
					velocity = 0;
				}
				player.position.x += velocity;
				if (this->ball.stuck) {
					this->ball.position.x += velocity;	
				}
			} else {
				player.position.x = this->width - PLAYER_WIDTH;
			}
		}
		if (this->keys[GLFW_KEY_SPACE]) {
			this->ball.stuck = false;
			this->ball.moveX = GameObject::LEFT;
			this->ball.moveY = GameObject::UP;
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

