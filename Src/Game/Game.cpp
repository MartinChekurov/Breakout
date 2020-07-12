#include "Errors.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Game.h"
#include "GameObject.h"
#include "BallObject.h"
#include "Texture.h"
#include "fwd.hpp"
#include "glfw3.h"
#include <cstdlib>
#include<iostream>
#include "irrKlang.h"

using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();
ParticleGenerator *Particles;

static GLfloat checkXCollision(GameObject &one, GameObject &two) 
{
	GLfloat penetration = 0.0f;
	if (one.position.x <= two.position.x + two.size.x &&
		one.position.x >= two.position.x) {
		penetration = (two.position.x + two.size.x) - one.position.x;
	}
	if (one.position.x + one.size.x >= two.position.x &&
		one.position.x + one.size.x <= two.position.x + two.size.x) {
		penetration = (one.position.x + one.size.x) - two.position.x;
	}
	return penetration;
}

static GLfloat checkYCollision(GameObject &one, GameObject &two) 
{
	GLfloat penetration = 0.0f;
	if (one.position.y <= two.position.y + two.size.y &&
		one.position.y >= two.position.y) {
		penetration = (two.position.y + two.size.y) - one.position.y;
	}
	if (one.position.y + one.size.y >= two.position.y &&
		one.position.y + one.size.y <= two.position.y + two.size.y) {
		penetration = (one.position.y + one.size.y) - two.position.y;
	}
	return penetration;
}

void Game::doCollisions()
{
	GLfloat xcollision = 0.0f, ycollision = 0.0f;
	for(GameObject& obj : this->levels[this->level].Bricks) {
		if (!obj.destroyed) {
			xcollision = checkXCollision(this->ball, obj);
			ycollision = checkYCollision(this->ball, obj);
			if (xcollision && ycollision) {
				SoundEngine->play2D("Res/Audios/bleep.mp3", GL_FALSE);
				if (!obj.isSolid) {
					obj.destroyed = true;
				}
				if (ycollision < xcollision) { //vertical collision
					if (this->ball.moveY == GameObject::UP) {
						this->ball.position.y += ycollision + 0.00001f;
						this->ball.moveY = GameObject::DOWN;
					} else {
						this->ball.position.y -= ycollision + 0.00001f;
						this->ball.moveY = GameObject::UP;
					}
				} else { //horizontal collision
					if (this->ball.moveX == GameObject::LEFT) {
						this->ball.position.x += xcollision + 0.00001f;
						this->ball.moveX = GameObject::RIGHT;
					} else {
						this->ball.position.x -= xcollision + 0.00001f;
						this->ball.moveX = GameObject::LEFT;
					}
				}
			}
		}
	}	
	xcollision = checkXCollision(this->ball, this->player);
	ycollision = checkYCollision(this->ball, this->player);
	if (xcollision && ycollision && !this->ball.stuck) {
		SoundEngine->play2D("Res/Audios/bleep.wav", GL_FALSE);
		this->ball.moveY = GameObject::UP;
		this->ball.position.y -= ycollision + 0.00001f;
	    GLfloat centerBoard = this->player.position.x + (this->player.size.x / 2);
        GLfloat distance = 0;
		if ((this->ball.position.x + this->ball.radius) < centerBoard) {
			this->ball.moveX = GameObject::LEFT;
        	distance = centerBoard - (this->ball.position.x + this->ball.radius);
		} else if ((this->ball.position.x + this->ball.radius) > centerBoard) {
			this->ball.moveX = GameObject::RIGHT;
        	distance = (this->ball.position.x + this->ball.radius) - centerBoard;
		} else {
			this->ball.moveX = GameObject::STATIC;
			distance = 0;
		}
        GLfloat percentage = distance / (this->player.size.x / 2);
        GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = this->ball.velocity;
		this->ball.velocity.x = BALL_INITIAL_VELOCITY.x * percentage * strength;
		this->ball.velocity = glm::normalize(this->ball.velocity) * glm::length(oldVelocity);
	}
}

Game::Game(GLuint width, GLuint height) 
	: state(GAME_ACTIVE), keys(), width(width), height(height) 
{ 

}

Game::~Game()
{
	delete Particles;
	SoundEngine->drop();
}	

Error Game::init()
{
	Error status = ERR_NO_ERR;
	Shader shader("Src/Shaders/sprite.vs", "Src/Shaders/sprite.frag");
	Shader shaderParticle("Src/Shaders/particle.vs", "Src/Shaders/particle.frag");
	glm::mat4 projection = glm::ortho(0.0f, (GLfloat)this->width, (GLfloat)this->height, 0.0f, -1.0f, 1.0f);
	const GLchar* textures[6] = {
		"Res/Textures/background2.jpg",
		"Res/Textures/ball.png",
		"Res/Textures/block.png",
		"Res/Textures/block_solid.png",
		"Res/Textures/paddle.png",
		"Res/Textures/particle.png"
	};
	const ResourceManagerTexture2D texturesIds[6] = {
		RESOURCE_MANAGER_TEXTURE2D_BACKGROUND,
		RESOURCE_MANAGER_TEXTURE2D_AWESOME_FACE,
		RESOURCE_MANAGER_TEXTURE2D_BLOCK,
		RESOURCE_MANAGER_TEXTURE2D_SOLID_BLOCK,
		RESOURCE_MANAGER_TEXTURE2D_PADDLE,
		RESOURCE_MANAGER_TEXTURE2D_PARTICLE
	};
	GLuint i = 0;

	CHECK(shader.load());
	CHECK(shader.compile());
	CHECK(shader.setMatrix4("projection", projection));
	CHECK(ResourceManagerLoadShader(shader, RESOURCE_MANAGER_SHADER_SPRITE));

	CHECK(shaderParticle.load());
	CHECK(shaderParticle.compile());
	CHECK(shaderParticle.setMatrix4("projection", projection));
	CHECK(ResourceManagerLoadShader(shaderParticle, RESOURCE_MANAGER_SHADER_PARTICLE));
	
	for (i = 0 ; i < 6 ; i++) {
		Texture2D texture(textures[i]);
		CHECK(texture.load());
		CHECK(texture.generate());
		CHECK(ResourceManagerLoadTexture(texture, texturesIds[i]));
	}
	
	Particles = new ParticleGenerator(shaderParticle, *ResourceManagerGetTexture(RESOURCE_MANAGER_TEXTURE2D_PARTICLE), 500);
		
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
	player.velocity = PLAYER_INITIAL_VELOCITY;

	ball.position.x = player.position.x + ((PLAYER_WIDTH - BALL_RADIUS*2) / 2);
	ball.position.y = player.position.y - BALL_RADIUS*2;
	ball.size = glm::vec2 (BALL_RADIUS*2, BALL_RADIUS*2);
	Texture2D* ballTexture = ResourceManagerGetTexture(RESOURCE_MANAGER_TEXTURE2D_AWESOME_FACE);
	CHECK_ERR(!ballTexture, ERR_INV_VAL);
	ball.sprite = *ballTexture;
	ball.velocity = BALL_INITIAL_VELOCITY;
	ball.color = glm::vec3(0.0f, 0.6f, 0.8f);

	SoundEngine->play2D("Res/Audios/breakout.mp3", GL_TRUE);

	return ERR_NO_ERR;
}

void Game::update(float dt)
{
	this->ball.move(dt, this->width, this->height);
	this->doCollisions();
    Particles->Update(dt, this->ball, 2, glm::vec2(this->ball.radius / 2));
	if ((this->ball.position.y + this->ball.size.y >= this->height) ||
		this->levels[this->level].isCompleted()) {
		this->reset();
	}
}

void Game::processInput(float dt)
{
	GLfloat velocity = 0;
	if (this->state == GAME_ACTIVE) {
		velocity = player.velocity.x * dt;
	    if (this->keys[GLFW_KEY_A] || this->keys[GLFW_KEY_LEFT]) {
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
		if (this->keys[GLFW_KEY_D] || this->keys[GLFW_KEY_RIGHT]) {
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
		if (this->keys[GLFW_KEY_SPACE] || this->keys[GLFW_KEY_UP]) {
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
		Particles->Draw();
		this->ball.draw(this->renderer);
    }
}

void Game::reset()
{
	this->player.position.x = (this->width - PLAYER_WIDTH) / 2;
	this->player.position.y = this->height - PLAYER_HEIGHT;

	this->ball.position.x = player.position.x + ((PLAYER_WIDTH - BALL_RADIUS*2) / 2);
	this->ball.position.y = player.position.y - BALL_RADIUS*2;
	this->ball.stuck = true;

	this->levels[this->level].reset();
}
