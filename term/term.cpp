#include <SDL/SDL.h>
#include <array>
#include <iostream>
#include <cstdlib>

#include "main/main.h"
#include "fonts/font.h"
#include "term/term.h"
#include "config.h"

static const struct font_desc *term_font_desc;

static std::array<std::array<char, TERM_WIDTH>, TERM_ROLLBACK_LINES> term_buffer;

static std::array<SDL_Surface *, 256> font_surfaces;

static unsigned int term_start_line;

static unsigned int cursor_x, cursor_y;

void clear_screen()
{
	SDL_FillRect(mainScreen, NULL, SDL_MapRGB(mainScreen->format, 0, 0, 0));
	SDL_Flip(mainScreen);
}

static void draw_term_char_noflip(char ch, unsigned int x, unsigned int y)
{
	SDL_Rect dstrect;
	dstrect.w = term_font_desc->width;
	dstrect.h = term_font_desc->height;
	dstrect.x = x * dstrect.w;
	dstrect.y = y * dstrect.h + SCREEN_SHIFT;
	SDL_BlitSurface(font_surfaces[((uint8_t)ch)], NULL, mainScreen, &dstrect);
}

void redraw_term()
{
	clear_screen();
	unsigned int term_end_line = term_start_line + TERM_HEIGHT;
	if (term_end_line > TERM_ROLLBACK_LINES) term_end_line = TERM_ROLLBACK_LINES;
	for (unsigned int y = term_start_line; y < term_end_line; y++)
		for (unsigned int x = 0; x < TERM_WIDTH; x++)
			if (term_buffer[y][x])
				draw_term_char_noflip(term_buffer[y][x], x, y - term_start_line);
	SDL_Flip(mainScreen);
}

static bool normalize_term_disp_area()
{
	if (cursor_y >= term_start_line && cursor_y < term_start_line + TERM_HEIGHT)
		return false;

	if (cursor_y < TERM_HEIGHT)
		term_start_line = 0;
	else
		cursor_y = term_start_line - TERM_HEIGHT + 1;

	return true;
}

static void term_buffer_scroll_one_line()
{
	for (unsigned int y = 0; y < TERM_ROLLBACK_LINES - 1; y++)
		term_buffer[y] = term_buffer[y+1];

	for (auto &ch : term_buffer[TERM_ROLLBACK_LINES - 1])
		ch = '\0';
}

static bool normalize_term_cursor()
{
	bool processed = false;

	while (cursor_x >= TERM_WIDTH) {
		cursor_x -= TERM_WIDTH;
		cursor_y++;
		processed = true;
	}

	while (cursor_y >= TERM_ROLLBACK_LINES) {
		term_buffer_scroll_one_line();
		cursor_y--;
		processed = true;
	}

	return processed;
}

void term_putchar(char ch)
{
	switch(ch) {
	case '\n':
		cursor_x = 0;
		cursor_y++;
		break;
	default:
		term_buffer[cursor_y][cursor_x] = ch;
		cursor_x++;
	}

	normalize_term_cursor();
	normalize_term_disp_area();
	redraw_term();
}

void term_puts(const char *str)
{
	while (*str) {
		term_putchar(*str);
		str++;
	}
}

void init_term()
{
	term_font_desc = find_font(FONT_NAME);
	if (!term_font_desc) {
		std::cerr << "Cannot find suitable font!" << std::endl;
		std::exit(1);
	}

	for (auto &line : term_buffer) {
		for (auto &ch : line) {
			ch = '\0';
		}
	}

	for (int i = 0; i < 256; i++) {
		SDL_Rect curr_rect;
		font_surfaces[i] = SDL_CreateRGBSurface(0, term_font_desc->width, term_font_desc->height,
							16, 0, 0, 0, 0);
		SDL_Surface *curr_surface = font_surfaces[i];
		const uint8_t *font_data = (const uint8_t *)(term_font_desc->data);
		size_t font_data_index = i * ((term_font_desc->width + 7) / 8) * term_font_desc->height;
		curr_rect.w = 1;
		curr_rect.h = 1;
		for (int y = 0; y < term_font_desc->height; y++) {
			curr_rect.y = y;
			for (int x = 0; x < term_font_desc->width; x++) {
				curr_rect.x = term_font_desc->width - 1 - x;
				if (font_data[font_data_index + (x / 8)] & (1 << (x % 8)))
					SDL_FillRect(curr_surface, &curr_rect,
						     SDL_MapRGB(curr_surface->format, 255, 255, 255));
				else
					SDL_FillRect(curr_surface, &curr_rect,
						     SDL_MapRGB(curr_surface->format, 0, 0, 0));
			}
			font_data_index += (term_font_desc->width + 7) / 8;
		}
	}

	term_start_line = 0;

	cursor_x = cursor_y = 0;
}

void deinit_term()
{
	for (int i = 0; i < 256; i++)
		SDL_FreeSurface(font_surfaces[i]);
}
