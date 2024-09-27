#pragma once
#include <vector>
#include <string>
#include <SDL.h>
using namespace std;

#define FONT_SIZE 30

void LoadTextureInit();
void LoadTextureQuit();
SDL_Texture* LoadImage(const char* filename, SDL_Renderer *renderer);
SDL_Texture * LoadText(const char * str, SDL_Renderer * renderer, int r, int g, int b);

void SaveScreen(string file_name, SDL_Window *window, SDL_Renderer * renderer);