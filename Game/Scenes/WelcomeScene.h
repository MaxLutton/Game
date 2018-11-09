#pragma once
#include "Scene.h"
#include <iostream>
#include <SDL.h>


class WelcomeScene : public Scene
{
public:
	void init();
	void update();
	void render();

	enum groupLabels : std::size_t
	{
		groupMap
	};

};