#include <SDL/SDL.h>
#include <iostream>
#include <cstdlib>

#include "main/main.h"
#include "fonts/font.h"
#include "config.h"

const struct font_desc *term_font_desc;

void init_term()
{
	term_font_desc = find_font(FONT_NAME);
	if (!term_font_desc) {
		std::cerr << "Cannot find suitable font!" << std::endl;
		std::exit(1);
	}
}
