#pragma once
#include <SDL.h>
#include "Fractal.h"
class AutoFractal {
private:
	SDL_Window *window;
	SDL_Renderer *renderer;

	vector<Seed*> seed;
	vector<Fractal*> fractal;

	bool save;
	
	void SetFractal();

public:
	AutoFractal(SDL_Window *window, SDL_Renderer *renderer, bool save);
	~AutoFractal();
	void Run();

	void PushSeed(Seed *seed);
	void RandomSeed(int cnt);
	void AllSeed();
	void ClearSeed();
};

