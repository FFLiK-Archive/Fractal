#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>
#include <string>
#include "ColorCalc.h"
using namespace std;

int Degree(int degree);

struct Point {
	double x;
	double y;
};

struct Line {
	Point point;
	int degree;
	double distance;
};

class Seed {
private:
	vector<Line> seed;

public:
	void push(Line line);
	void push(double degree, double distance);
	~Seed();

	vector<Line> NewFractal(Line l);

	int GetSize();

	string GetData();
};

class Fractal {
private:
	Seed *seed;
	vector<Line> fractal;
	Color::RGB_Color begin_color, end_color;

	SDL_Texture* screen;

public:
	Fractal(Seed *seed, Color::RGB_Color begin_color, Color::RGB_Color end_color);
	~Fractal();
	void Rendering(SDL_Renderer *renderer, SDL_Point focus, double size = 1);
	void NextFractal();

	void SetScreenTexture(SDL_Renderer *renderer, double size);
	double GetModerateSize(int margin);

	void ChangeFractalSize(double size);
};

void SinCosTanInit();