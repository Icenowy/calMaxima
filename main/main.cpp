#include <SDL/SDL.h>
#include <iostream>

#include "main/main.h"
#include "main/event_loop.h"
#include "term/term.h"
#include "config.h"

SDL_Surface *mainScreen;

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cerr << "Failed to initialize SDL!" << std::endl;
		return 1;
	}

	mainScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_LOGIC_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	if (!mainScreen) {
		std::cerr << "Failed to create SDL main screen!" << std::endl;
		return 1;
	}

	clear_screen();
	init_term();

	event_loop();

	deinit_term();
	SDL_Quit();
}
