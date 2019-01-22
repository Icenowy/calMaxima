#ifndef _TERM_H
#define _TERM_H

void clear_screen();
void redraw_term();

void term_putchar(char ch);
void term_puts(const char *str);

void init_term();
void deinit_term();


#endif
