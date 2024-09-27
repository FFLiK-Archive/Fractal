#pragma once
#include "Fractal.h"
#include <SDL.h>
class Simulator {
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	Seed *seed;
	Fractal *fractal;

public:
	Simulator(SDL_Window *window, SDL_Renderer *renderer);
	~Simulator();
	void Run();
	
	void SetSeed(Seed *seed);
};

