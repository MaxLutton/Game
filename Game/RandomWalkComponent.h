#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"
#include <string>

class RandomWalkComponent : public Component
{
public:
	int range;
	Vector2D velocity;
	bool getNewWalk = true;
	TransformComponent* transform;
	SpriteComponent* sprite;
	std::string animationId;
	
	int speed = 0;
	int distance = 0;
	
	RandomWalkComponent(Vector2D vel,int r, int sp) : velocity(vel), range(r), speed(sp) 
	{
		getNewWalk = false;
	}

	RandomWalkComponent()
	{
		velocity.x = 0;
		velocity.y = 0;
		range = 0;
		speed = 0;
		getNewWalk = true;;
	}


	~RandomWalkComponent(){}
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
		sprite = &entity->getComponent<SpriteComponent>();
		animationId = "Idle";
	}

	void update() override
	{
		distance += speed;

		if (distance >= range)
		{
			std::cout << "Scarecrow Out of Range" << std::endl;
			// Switch to Idle
			animationId = "Idle";
			sprite->play("Idle");
			getNewWalk = true;
			transform->velocity = Vector2D(0.0f, 0.0f);
			//entity->destroy();
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w ||
			transform->position.x < Game::camera.x ||
			transform->position.y > Game::camera.y + Game::camera.h ||
			transform->position.y < Game::camera.y)
		{
			std::cout << "Scarecrow Out of Bounds" << std::endl;
			animationId = "Idle";
			sprite->play("Idle");
			getNewWalk = true;
		}
		else if (velocity.x > 0)
		{
			animationId = "Walk";
			sprite->play("Walk");
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		else if (velocity.x < 0)
		{
			animationId = "Walk";
			sprite->play("Walk");
			sprite->spriteFlip = SDL_FLIP_NONE;
		}
	}

	void setNewWalk(Vector2D vel, int r, int sp)
	{
		transform->velocity = vel;
		range = r;
		speed = sp;
		getNewWalk = false;
	}


};