#pragma once

#include "ECS.h"
#include "Components.h"
#include <time.h>
#include <string>

class HealthComponent : public Component
{
public:
	int lives;
	bool dead;
	time_t zeroLives;
	SpriteComponent* sprite;
	std::string tag;
	
	HealthComponent(int l, std::string s) : lives(l), tag(s) { zeroLives = 0; 	dead = false; }
	void takeLife() { lives--; }
	void setDeath(time_t t) { zeroLives = t; }

	void init() override
	{
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (lives <= 0)
		{
			sprite->play("Explode");
			if (dead == false)
			{
				setDeath(time(0));
				dead = true;
				Game::enemiesLeftToKill--;
			}
			else if (difftime(time(0), zeroLives) > 1)
			{
				if (tag != "player")
				this->entity->destroy();
			}
		}
	}
};