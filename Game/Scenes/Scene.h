#pragma once

class Scene {
public:
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void init() = 0;
};

