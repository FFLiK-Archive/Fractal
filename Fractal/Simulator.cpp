#include "Simulator.h"
#include <iostream>
#include <SDL.h>
#include <vector>
#include <ctime>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "ColorCalc.h"
#include "Fractal.h"
#include "Manipulate.h"
#include "LoadTexture.h"

using namespace Color;

extern int win_w, win_h;
extern bool run;

Simulator::Simulator(SDL_Window * window, SDL_Renderer * renderer) {
	this->window = window;
	this->renderer = renderer;
	this->fractal = nullptr;
	this->seed = nullptr;
}

Simulator::~Simulator() {
	delete this->fractal;
	delete this->seed;
}

void Simulator::Run() {
	SDL_Point focus = { 0,0 };

	SDL_Event event;

	double size = 1;

	double move_size = 1;

	double speed = 0.97;
	int level = 0;

	Manipulate::SetSize(size);
	Manipulate::SetLevel(level);
	Manipulate::SetRenderingCnt(0);
	Manipulate::SetDimension(0, seed);

	bool focus_moving = false;

	bool win_focused = false;

	fractal->SetScreenTexture(renderer, size);

	int f_clock = clock();

	int save = 0;

	while (run) {
		while (SDL_PollEvent(&event)) {
			if (event.window.windowID == SDL_GetWindowID(window)) {
				if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
					run = false;
				}
				else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
					win_focused = true;
				}
				else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
					win_focused = false;
				}
			}
			if (win_focused) {
				switch (event.type) {
				case SDL_MOUSEBUTTONDOWN:
					focus_moving = true;
					break;
				case SDL_MOUSEBUTTONUP:
					focus_moving = false;
					break;
				case SDL_MOUSEMOTION:
					if (focus_moving) {
						focus.x += event.motion.xrel;
						focus.y += event.motion.yrel;
					}
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_SPACE) {
						level++;
						Manipulate::SetLevel(level);
						fractal->NextFractal();
					}
					move_size = 1;
					fractal->SetScreenTexture(renderer, size);
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_UP) {
						size /= speed;
						move_size /= speed;
					}
					else if (event.key.keysym.sym == SDLK_DOWN) {
						size *= speed;
						move_size *= speed;
					}
					else if (event.key.keysym.sym == SDLK_RETURN) {
						SDL_RenderClear(renderer);
						SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
						fractal->Rendering(renderer, { 0,0 }, move_size);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
						SaveScreen("img//" + to_string(save) + "_screen.png", window, renderer);
						Manipulate::RenderText(renderer);
						SaveScreen("img//" + to_string(save) + "_status.png", window, renderer);
						save++;
					}
					Manipulate::SetSize(size);
					break;
				}
			}
		}

		if (clock() - f_clock > 16) {
			SDL_RenderClear(renderer);
			fractal->Rendering(renderer, focus, move_size);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderPresent(renderer);
			Manipulate::Rendering();
			f_clock = clock();
		}
		SDL_GetWindowSize(window, &win_w, &win_h);
		SDL_Delay(1);
	}
}

void Simulator::SetSeed(Seed * seed) {
	if (this->seed)
		delete this->seed;
	
	this->seed = new Seed();

	*this->seed = *seed;
	
	if (this->fractal)
		delete this->fractal;

	HSV_Color hsv;
	hsv.h = rand() % 360;
	hsv.s = 50;
	hsv.v = 100;
	RGB_Color rgb1, rgb2;
	rgb1 = HSVtoRGB(hsv);
	hsv.h += 60;
	if (hsv.h >= 360)
		hsv.h -= 360;
	rgb2 = HSVtoRGB(hsv);
	this->fractal = new Fractal(seed, rgb1, rgb2);
}
