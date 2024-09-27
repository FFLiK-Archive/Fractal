#include "AutoFractal.h"
#include "ColorCalc.h"
#include "Manipulate.h"
#include "LoadTexture.h"
#include "Fractal.h"
#include "DependNotResponding.h"
using namespace Color;

extern vector<double> sin_memory;
extern vector<double> cos_memory;
extern vector<double> tan_memory;

extern int win_w, win_h;
extern bool run;

void AutoFractal::SetFractal() {
	this->fractal.clear();
	for (int i = 0; i < this->seed.size(); i++) {

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

		this->fractal.push_back(new Fractal(this->seed[i], rgb1, rgb2));
	}
}

AutoFractal::AutoFractal(SDL_Window * window, SDL_Renderer * renderer, bool save) {
	this->window = window;
	this->renderer = renderer;
	this->save = save;
	SDL_SetWindowResizable(window, SDL_FALSE);
	SDL_SetWindowSize(window, 1600, 900);
}

AutoFractal::~AutoFractal() {
	this->seed.clear();
	this->fractal.clear();
}

void AutoFractal::Run() {
	SDL_GetWindowSize(window, &win_w, &win_h);
	int num = 0;
	while (!this->seed.empty() && run) {
		Seed *seed = this->seed.back();
		Fractal *fractal = this->fractal.back();
		this->seed.pop_back();
		this->fractal.pop_back();

		double size = 1;
		double speed = 0.99;
		int level = 0;

		fractal->NextFractal();
		level++;

		Manipulate::SetSize(size);
		Manipulate::SetLevel(level);
		Manipulate::SetRenderingCnt(0);
		Manipulate::SetDimension(0, seed);
		Manipulate::SetSeed(*seed);

		int save = 0;
		if (this->save) {
			for (int i = 0; i < 4; i++) {
				if (DependNotResponding()) {
					run = false;
				}
				size = fractal->GetModerateSize(100);

				Manipulate::SetSize(size);
				Manipulate::SetLevel(level);

				SDL_RenderClear(this->renderer);
				fractal->SetScreenTexture(this->renderer, size);

				SDL_RenderClear(this->renderer);
				fractal->Rendering(this->renderer, { 0,0 });
				SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);

				SaveScreen("img//" + to_string(num) + "-" + to_string(save) + "_screen.png", window, renderer);
				Manipulate::RenderText(renderer);
				SaveScreen("img//" + to_string(num) + "-" + to_string(save) + "_status.png", window, renderer);

				SDL_RenderPresent(this->renderer);
				Manipulate::Rendering();
				save++;

				for (int j = 0; j < 5; j++) {
					fractal->NextFractal();
					level++;
					size = fractal->GetModerateSize(100);
					fractal->ChangeFractalSize(size);
				}
			}
		}
		else {
			fractal->NextFractal();
		}
		num++;
		delete seed;
		delete fractal;
		SDL_Delay(1);
	}
}

void AutoFractal::PushSeed(Seed * seed) {
	this->seed.push_back(new Seed());
	*this->seed.back() = *seed;
	SetFractal();
}

void AutoFractal::RandomSeed(int cnt) {
	this->ClearSeed();
	for (int i = 0; i < cnt; i++) {
		Seed s;
		int degree1 = rand() % 178 + 1;
		int degree2 = -(rand() % (179 - degree1) + 1);

		if (rand() % 2) {
			degree1 *= -1;
			degree2 *= -1;
		}

		s.push(degree1, 1.0 / sin_memory[abs(degree1)]);
		s.push(degree2, 1.0 / sin_memory[abs(degree2)]);

		//s.push(45, 5);
		//s.push(-45, 5);
		
		this->seed.push_back(new Seed());
		*this->seed.back() = s;
	}
	SetFractal();
}

void AutoFractal::AllSeed() {
	this->ClearSeed();
	for (int i = 1; i <= 178; i++) {
		for (int j = 1; j <= 179 - i; j++) {
			Seed s1, s2;
			int degree1 = i;
			int degree2 = -j;

			s1.push(degree1, 1.0 / sin_memory[abs(degree1)]);
			s1.push(degree2, 1.0 / sin_memory[abs(degree2)]);

			s2.push(-degree1, 1.0 / sin_memory[abs(degree1)]);
			s2.push(-degree2, 1.0 / sin_memory[abs(degree2)]);

			this->seed.push_back(new Seed());
			*this->seed.back() = s1;

			this->seed.push_back(new Seed());
			*this->seed.back() = s2;
		}
	}
	SetFractal();
}

void AutoFractal::ClearSeed() {
	this->seed.clear();
	this->fractal.clear();
}
