#ifndef COLORING_H
#define COLORING_H
#include <stdint.h>
#define WIDTH 192
#define HEIGHT 144
#define FB_ADDR 0x20000000u
#define PAL_ADDR 0x21000000u
#define PRESENT_ADDR 0x21001008u
#define KEY_QUEUE_ADDR 0x4000002Cu
#define MOUSE_X_ADDR 0x40000030u
#define MOUSE_Y_ADDR 0x40000034u
#define MOUSE_BUTTONS_ADDR 0x40000038u
#define FB ((volatile uint8_t *)(uintptr_t)FB_ADDR)
#define PAL ((volatile uint32_t *)(uintptr_t)PAL_ADDR)
#define REG32(address) (*(volatile uint32_t *)(uintptr_t)(address))
/* Person 1: templates.c */
void set_pixel(int x, int y, int color);
void load_template(int number);
/* Person 2: graphics.c */
void init_palette(void);
void clear_screen(int color);
void draw_digit(int digit, int x, int y, int color);
void draw_menu(void);
void present(void);
/* Person 3: main.c */
void handle_key(int key, int *in_menu, int *current_color);
void handle_mouse(int current_color);
#endif
