#include <SDL/SDL.h>

#include "main/main.h"
#include "main/event_loop.h"
#include "config.h"

static SDL_Event event;

bool event_loop_terminate;

void event_loop()
{
	event_loop_terminate = false;
	while (!event_loop_terminate) {
		while (SDL_PollEvent(&event)) {
		}
	}
}
