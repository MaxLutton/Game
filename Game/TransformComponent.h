#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component {

public:

	Vector2D position;
	Vector2D velocity;

	int speed = 3;

	int height = 32;
	int width = 32;
	int scale = 1;

	TransformComponent() {
		position.zero();
	}
	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc) {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	TransformComponent(int sc) {
		position.zero();
		scale = sc;
	}


	void init() override {
		velocity.zero();
	}

	void update() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}


	void setPos(float x, float y) {
		position.x = x;
		position.y = y;
	}
};