#include "DependNotResponding.h"
#include <SDL.h>

bool DependNotResponding() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}
	}
	return false;
}
