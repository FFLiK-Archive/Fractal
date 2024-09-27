#include "LoadTexture.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

TTF_Font* font;

void LoadTextureInit() {
	font = TTF_OpenFont("font.ttf", FONT_SIZE);
}

void LoadTextureQuit() {
	TTF_CloseFont(font);
}

SDL_Texture * LoadImage(const char * filename, SDL_Renderer * renderer) {
	SDL_Surface *surface;
	SDL_Texture *texture;
	surface = IMG_Load(filename);
	cout << IMG_GetError();
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

SDL_Texture * LoadText(const char * str, SDL_Renderer * renderer, int r, int g, int b) {
	SDL_Surface *surface;
	SDL_Texture *texture;

	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 255;

	surface = TTF_RenderText_Blended(font, str, color);

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
	return texture;
}

void SaveScreen(string file_name, SDL_Window *window, SDL_Renderer * renderer) {
	SDL_Surface* saveSurface = NULL;
	SDL_Surface* infoSurface = NULL;
	infoSurface = SDL_GetWindowSurface(window);
	unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
	
	SDL_RenderReadPixels(renderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel);
	saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
	
	IMG_SavePNG(saveSurface, file_name.c_str());
	SDL_FreeSurface(saveSurface);
	saveSurface = NULL;
	SDL_FreeSurface(infoSurface);
	infoSurface = NULL;
}
