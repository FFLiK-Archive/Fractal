#pragma once
#include <SDL.h>
#include <vector>
#include "Fractal.h"
using namespace std;
class Manipulate {
private:
	static SDL_Window *window;
	static SDL_Renderer *renderer;

	static double size, dimension;
	static long long int level, total_structure, calc_structure, rendering_cnt;
	static Seed *seed;
	static SDL_Texture *size_t, *level_t, *total_structure_t, *calc_structure_t, *rendering_cnt_t, *dimension_t, *seed_t;
	static double a, b;

public:
	static void Init();
	static void SetSize(double s);
	static void SetLevel(long long int l);
	static void SetStructure(long long int s, vector<Line> v);
	static void SetCalc(long long int c);
	static void SetRenderingCnt(long long int c);
	static void SetDimension(double d, Seed *s);
	static void SetSeed(Seed s);
	static void Rendering();
	static void Quit();

	static void RenderText(SDL_Renderer *renderer);
};

