#include "Fractal.h"
#include "DependNotResponding.h"
#include "Manipulate.h"
#include <Windows.h>
#include <ctime>
#include <dos.h>

using namespace Color;

extern int win_w;
extern int win_h;
extern bool run;

vector<double> sin_memory;
vector<double> cos_memory;
vector<double> tan_memory;

#define pi 3.1415926

#define MIN(x, y) (x < y ? x : y);

int Degree(int degree) {
	while (degree >= 360) {
		degree -= 360;
	}
	while (degree < 0) {
		degree += 360;
	}
	return degree;
}

Point GetPoint(Line l) {
	Point p;
	p = l.point;
	p.x += cos_memory[l.degree] * l.distance;
	p.y += sin_memory[l.degree] * l.distance;
	return p;
}

void Seed::push(Line line) {
	this->seed.push_back(line);
}

void Seed::push(double degree, double distance) {
	Line l;
	if (this->seed.empty()) {
		l.point.x = 0;
		l.point.y = 0;
	}
	else {
		l.point.x = GetPoint(this->seed.back()).x;
		l.point.y = GetPoint(this->seed.back()).y;
	}
	l.degree = Degree(degree);
	l.distance = distance;
	this->seed.push_back(l);
}

Seed::~Seed() {
	this->seed.clear();
}

vector<Line> Seed::NewFractal(Line l) {
	vector<Line> fractal;
	Point p1 = this->seed.front().point;
	Point p2 = GetPoint(this->seed.back());
	double seed_length = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	double rate = l.distance / seed_length;
	Point point = l.point;
	for (int i = 0; i < this->seed.size(); i++) {
		Line t;
		t.degree = Degree(this->seed[i].degree + l.degree);
		t.point = point;
		t.distance = this->seed[i].distance * rate;
		fractal.push_back(t);
		point = GetPoint(t);
	}
	return fractal;
}

int Seed::GetSize() {
	return seed.size();
}

string Seed::GetData() {
	string str;
	for (int i = 0; i < this->seed.size(); i++) {
		str += to_string(this->seed[i].degree) + " ";
	}
	return str;
}

Fractal::Fractal(Seed * seed, RGB_Color begin_color, RGB_Color end_color) {
	this->seed = seed;
	Line l;
	l.point = { 0,0 };
	l.degree = 0;
	l.distance = win_w / 2;
	this->fractal.push_back(l);
	Manipulate::SetStructure(this->fractal.size(), this->fractal);
	Manipulate::SetCalc(this->fractal.size());

	this->begin_color = begin_color;
	this->end_color = end_color;
	/*this->render_music = false;
	this->playing_pos = -1;
	this->music.clear();*/
}

Fractal::~Fractal() {
	this->seed = nullptr;
	this->fractal.clear();
}

void Fractal::Rendering(SDL_Renderer * renderer, SDL_Point focus, double size) {
	SDL_Rect dst;
	SDL_QueryTexture(this->screen, NULL, NULL, &dst.w, &dst.h);

	dst.w *= size;
	dst.h *= size;
	dst.x = focus.x + win_w / 2 - dst.w / 2;
	dst.y = focus.y + win_h / 2 - dst.h / 2;
	SDL_RenderCopy(renderer, this->screen, NULL, &dst);

	dst.x--;
	dst.y--;
	dst.w += 2;
	dst.h += 2;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &dst);
}

void Fractal::NextFractal() {
	vector<Line> f;
	f.resize(this->fractal.size() * this->seed->GetSize());

	int rander_t = clock();

	Manipulate::SetStructure(this->fractal.size() * this->seed->GetSize(), this->fractal);
	for (int i = 0; i < this->fractal.size(); i++) {
		vector<Line> v = this->seed->NewFractal(fractal[i]);
		for (int j = 0; j < v.size(); j++) {
			f[i * this->seed->GetSize() + j] = v[j];
		}
		if (DependNotResponding()) {
			run = false;
			break;
		}
		if (clock() - rander_t > 33) {
			Manipulate::SetCalc(i * this->seed->GetSize());
			Manipulate::Rendering();
			rander_t = clock();
		}
	}

	double d;

	d = this->fractal.front().distance / f.front().distance;
	Manipulate::SetDimension(d, this->seed);

	this->fractal = f;
	Manipulate::SetCalc(this->fractal.size());
	Manipulate::Rendering();
}

void Fractal::SetScreenTexture(SDL_Renderer *renderer, double size) {
	int w, h;
	long long int x_min = 0, x_max = 0, y_min = 0, y_max = 0;

	for (int i = 0; i < this->fractal.size(); i++) {
		Point point;
		SDL_Point p;
		point = this->fractal[i].point;
		p.x = size * point.x;
		p.y = size * point.y;
		if (p.x < x_min)
			x_min = p.x;
		else if (p.x > x_max)
			x_max = p.x;
		if (p.y < y_min)
			y_min = p.y;
		else if (p.y > y_max)
			y_max = p.y;
	}

	Point point;
	SDL_Point p;
	point = GetPoint(this->fractal.back());
	p.x = size * point.x;
	p.y = size * point.y;
	if (p.x < x_min)
		x_min = p.x;
	else if (p.x > x_max)
		x_max = p.x;
	if (p.y < y_min)
		y_min = p.y;
	else if (p.y > y_max)
		y_max = p.y;

	w = x_max - x_min + 1;
	h = y_max - y_min + 1;

	if (this->screen)
		SDL_DestroyTexture(this->screen);

	this->screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, w, h);

	SDL_QueryTexture(this->screen, NULL, NULL, &w, &h);

	SDL_SetRenderTarget(renderer, this->screen);

	SDL_Point add;
	add.x = -x_min;
	add.y = -y_min;

	int cnt = 0;
	for (int i = 0; i < this->fractal.size(); i++) {
		Point begin, end;
		SDL_Point b, e;
		begin = this->fractal[i].point;
		end = GetPoint(this->fractal[i]);
		b.x = size * begin.x + add.x;
		b.y = size * begin.y + add.y;
		e.x = size * end.x + add.x;
		e.y = size * end.y + add.y;

		int rc, gc, bc;

		rc = ((long long int)this->begin_color.r * (long long int)(this->fractal.size() - i) + (long long int)this->end_color.r * (long long int)i) / (long long int)this->fractal.size();
		gc = ((long long int)this->begin_color.g * (long long int)(this->fractal.size() - i) + (long long int)this->end_color.g * (long long int)i) / (long long int)this->fractal.size();
		bc = ((long long int)this->begin_color.b * (long long int)(this->fractal.size() - i) + (long long int)this->end_color.b * (long long int)i) / (long long int)this->fractal.size();
		SDL_SetRenderDrawColor(renderer, rc, gc, bc, 255);
		SDL_RenderDrawLine(renderer, b.x, b.y, e.x, e.y);
		cnt++;
	}
	Manipulate::SetRenderingCnt(cnt);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRenderTarget(renderer, NULL);
}

double Fractal::GetModerateSize(int margin) {
	double w, h;
	long long int x_min = 0, x_max = 0, y_min = 0, y_max = 0;
	for (int i = 0; i < this->fractal.size(); i++) {
		Point point;
		SDL_Point p;
		point = this->fractal[i].point;
		p.x = point.x;
		p.y = point.y;
		if (p.x < x_min)
			x_min = p.x;
		else if (p.x > x_max)
			x_max = p.x;
		if (p.y < y_min)
			y_min = p.y;
		else if (p.y > y_max)
			y_max = p.y;
	}

	Point point;
	SDL_Point p;
	point = GetPoint(this->fractal.back());
	p.x = point.x;
	p.y = point.y;
	if (p.x < x_min)
		x_min = p.x;
	else if (p.x > x_max)
		x_max = p.x;
	if (p.y < y_min)
		y_min = p.y;
	else if (p.y > y_max)
		y_max = p.y;

	w = x_max - x_min + 1;
	h = y_max - y_min + 1;
	return MIN(((double)win_w - (double)margin * 2) / w, ((double)win_h - (double)margin * 2) / h);
}

void Fractal::ChangeFractalSize(double size) {
	for (int i = 0; i < this->fractal.size(); i++) {
		this->fractal[i].point.x *= size;
		this->fractal[i].point.y *= size;
		this->fractal[i].distance *= size;
	}
}

void SinCosTanInit() {
	for (int i = 0; i < 360; i++) {
		sin_memory.push_back(sin((double)i * pi / 180.0));
		cos_memory.push_back(cos((double)i * pi / 180.0));
		tan_memory.push_back(tan((double)i * pi / 180.0));
	}
}
