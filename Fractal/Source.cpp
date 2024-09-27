#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <ctime>
#include "Manipulate.h"
#include "LoadTexture.h"
#include "Simulator.h"
#include "AutoFractal.h"
using namespace std;

#define WIN_W 1200
#define WIN_H 675

int win_w = WIN_W;
int win_h = WIN_H;

#define AUTO false

bool run = true;

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	srand((unsigned)time(NULL));

	SinCosTanInit();
	
	SDL_Window *window = SDL_CreateWindow("Fractal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	Manipulate::Init();

	LoadTextureInit();

	SDL_SetWindowResizable(window, SDL_TRUE);

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);

	if (AUTO) {
		AutoFractal auto_fractal(window, renderer, true);
		auto_fractal.RandomSeed(16);
		auto_fractal.Run();
	}
	else {
		Simulator simulator(window, renderer);
		Seed seed;
		seed.push(0, 5);
		seed.push(-60, 5);
		seed.push(60, 5);

		/*int n = 100;
		double d = (180 * ((double)n - 2)) / (double)n;
		for (int i = 0; i < n-1; i++) {
			seed.push(-d + (180 - d) * i, 5);
		}*/
		seed.push(0, 5);
		simulator.SetSeed(&seed);
		simulator.Run();
	}

	LoadTextureQuit();
	Manipulate::Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}