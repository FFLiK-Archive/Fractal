#include "Manipulate.h"
#include "LoadTexture.h"
#include "Fractal.h"
#include <iostream>
using namespace std;
SDL_Window *Manipulate::window;
SDL_Renderer *Manipulate::renderer;

Seed *Manipulate::seed;
double Manipulate::size = -1, Manipulate::dimension = -1;
long long int Manipulate::level = -1, Manipulate::total_structure = -1, Manipulate::calc_structure = -1, Manipulate::rendering_cnt;
SDL_Texture *Manipulate::size_t, *Manipulate::level_t, *Manipulate::total_structure_t, *Manipulate::calc_structure_t, *Manipulate::rendering_cnt_t, *Manipulate::dimension_t, *Manipulate::seed_t;
double Manipulate::a, Manipulate::b;

double logN(double x, double n) {
	return log(x) / log(n);
}

void Manipulate::Init() {
	window = SDL_CreateWindow("Manipulate - Fractal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
	SDL_SetWindowResizable(window, SDL_TRUE);
}

void Manipulate::SetSize(double s) {
	if (size != s) {
		size = s;
		if (size_t) {
			SDL_DestroyTexture(size_t);
		}
		size_t = LoadText(("Size : " + to_string(size)).c_str(), renderer, 255, 255, 255);
	}
}

void Manipulate::SetLevel(long long int l) {
	if (level != l) {
		level = l;
		if (level_t) {
			SDL_DestroyTexture(level_t);
		}
		level_t = LoadText(("Level : " + to_string(level)).c_str(), renderer, 255, 255, 255);
	}
}

void Manipulate::SetStructure(long long int s, vector<Line> v) {
	if (total_structure != s) {
		total_structure = s;
		if (total_structure_t) {
			SDL_DestroyTexture(total_structure_t);
		}
		total_structure_t = LoadText(("Structures : " + to_string(total_structure) + " (" + to_string(sizeof(v) * s / 1024) + "KB)").c_str(), renderer, 255, 255, 255);
	}
}

void Manipulate::SetCalc(long long int c) {
	if (calc_structure != c) {
		calc_structure = c;
		if (calc_structure_t) {
			SDL_DestroyTexture(calc_structure_t);
		}
		calc_structure_t = LoadText((to_string(calc_structure) + " / " + to_string(total_structure) + " - " + to_string(calc_structure * 100 / total_structure) + "%").c_str(), renderer, 255, 255, 255);
	}
}

void Manipulate::SetRenderingCnt(long long int c) {
	if (rendering_cnt != c) {
		rendering_cnt = c;
		if (rendering_cnt_t) {
			SDL_DestroyTexture(rendering_cnt_t);
		}
		rendering_cnt_t = LoadText(("Rendered Structures : " + to_string(rendering_cnt)).c_str(), renderer, 255, 255, 255);
	}
}

void Manipulate::SetDimension(double d, Seed *s) {
	double D = logN(s->GetSize(), d);
	a = d;
	b = s->GetSize();
	if (dimension != D) {
		dimension = D;
		if (dimension_t) {
			SDL_DestroyTexture(dimension_t);
		}
		dimension_t = LoadText(("Dimension : " + to_string(dimension) + " - logn" + "(" + to_string(s->GetSize()) + ") , n = " + to_string(d)).c_str(), renderer, 255, 255, 255);
	}
}

void Manipulate::SetSeed(Seed s) {
	if (seed)
		delete seed;
	seed = new Seed();
	*seed = s;
	if (seed_t) {
		SDL_DestroyTexture(seed_t);
	}
	seed_t = LoadText(("Seed (degree) : " + seed->GetData()).c_str(), renderer, 255, 255, 255);
}

void Manipulate::Rendering() {
	SDL_RenderClear(renderer);
	SDL_Rect dst;

	dst.x = 10;
	dst.y = 10;
	
	if (seed) {
		SDL_QueryTexture(seed_t, NULL, NULL, &dst.w, &dst.h);
		SDL_RenderCopy(renderer, seed_t, NULL, &dst);

		dst.x = 10;
		dst.y += dst.h + 10;
	}

	SDL_QueryTexture(level_t, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, level_t, NULL, &dst); 

	dst.x = 10;
	dst.y += dst.h + 10;
	SDL_QueryTexture(size_t, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, size_t, NULL, &dst);

	dst.x = 10;
	dst.y += dst.h + 10;
	SDL_QueryTexture(total_structure_t, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, total_structure_t, NULL, &dst);

	dst.x = 10;
	dst.y += dst.h + 10;
	SDL_QueryTexture(rendering_cnt_t, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, rendering_cnt_t, NULL, &dst);

	dst.x = 10;
	dst.y += dst.h + 10;
	SDL_QueryTexture(dimension_t, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, dimension_t, NULL, &dst);

	dst.x = 10;
	dst.y += dst.h + 30;
	SDL_QueryTexture(calc_structure_t, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, calc_structure_t, NULL, &dst);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

void Manipulate::Quit() {
	SDL_DestroyTexture(size_t);
	SDL_DestroyTexture(level_t);
	SDL_DestroyTexture(total_structure_t);
	SDL_DestroyTexture(calc_structure_t);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Manipulate::RenderText(SDL_Renderer * renderer) {
	SDL_Texture *se = nullptr;
	if (seed) {
		se = LoadText(("Seed (degree) : " + seed->GetData()).c_str(), renderer, 255, 255, 255);
	}
	SDL_Texture *s = LoadText(("Structures : " + to_string(total_structure)).c_str(), renderer, 255, 255, 255);
	SDL_Texture *d = LoadText(("Dimension : " + to_string(dimension) + " - logn" + "(" + to_string(b) + ") , n = " + to_string(a)).c_str(), renderer, 255, 255, 255);
	SDL_Texture *l = LoadText(("Level : " + to_string(level)).c_str(), renderer, 255, 255, 255);
	SDL_Texture *ss = LoadText(("Size : " + to_string(size)).c_str(), renderer, 255, 255, 255);
	
	SDL_Rect dst;
	dst.x = 10;
	dst.y = 10;

	if (seed) {
		SDL_QueryTexture(se, NULL, NULL, &dst.w, &dst.h);
		SDL_RenderCopy(renderer, se, NULL, &dst);

		dst.x = 10;
		dst.y += dst.h + 10;
	}

	SDL_QueryTexture(l, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, l, NULL, &dst);

	dst.x = 10;
	dst.y += dst.h + 10;
	SDL_QueryTexture(ss, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, ss, NULL, &dst);

	dst.x = 10;
	dst.y += dst.h + 10;
	SDL_QueryTexture(s, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, s, NULL, &dst);

	dst.x = 10;
	dst.y += dst.h + 10;
	SDL_QueryTexture(d, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, d, NULL, &dst);

	SDL_DestroyTexture(s);
	SDL_DestroyTexture(l);
	SDL_DestroyTexture(d);
	SDL_DestroyTexture(ss);
	SDL_DestroyTexture(se);
}
