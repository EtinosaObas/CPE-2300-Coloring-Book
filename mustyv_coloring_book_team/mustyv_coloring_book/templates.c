#include "coloring.h"
/* Only write pixels within the 192x144 framebuffer. */
void set_pixel(int x, int y, int color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        FB[y * WIDTH + x] = (uint8_t)color;
}
/* Clear the screen and draw one of the original six outlines. */
void load_template(int number) {
    int x, y, i, dx, dy, value, width;
    clear_screen(0);
    if (number == 1) { /* flag/rectangle */
        for (x = 50; x < 140; x++) { set_pixel(x, 30, 1); set_pixel(x, 90, 1); }
        for (y = 30; y <= 90; y++) { set_pixel(50, y, 1); set_pixel(140, y, 1); }
    } else if (number == 2) { /* house */
        for (x = 70; x < 130; x++) set_pixel(x, 100, 1);
        for (y = 60; y <= 100; y++) { set_pixel(70, y, 1); set_pixel(130, y, 1); }
        for (i = 0; i <= 30; i++) { set_pixel(70+i, 60-i, 1); set_pixel(130-i, 60-i, 1); }
    } else if (number == 3) { /* circle: integer-only test */
        for (y = 0; y < HEIGHT; y++) for (x = 0; x < WIDTH; x++) {
            dx = x - 96; dy = y - 72;
            value = dx*dx + dy*dy - 35*35;
            if (value < 0) value = -value;
            if (value < 35) set_pixel(x, y, 1);
        }
    } else if (number == 4) { /* diamond */
        for (i = 0; i < 40; i++) {
            set_pixel(96-i,72-i,1); set_pixel(96+i,72-i,1);
            set_pixel(96-i,72+i,1); set_pixel(96+i,72+i,1);
        }
    } else if (number == 5) { /* window */
        for (x = 40; x < 152; x++) {
            set_pixel(x,20,1); set_pixel(x,124,1); set_pixel(x,72,1);
        }
        for (y = 20; y <= 124; y++) {
            set_pixel(40,y,1); set_pixel(151,y,1); set_pixel(96,y,1);
        }
    } else if (number == 6) { /* tree */
        for (y = 90; y < 130; y++) for (x = 92; x < 101; x++) set_pixel(x,y,1);
        for (i = 0; i < 40; i++) {
            width = 30 - i/2;
            for (x = 96-width; x < 96+width; x++) set_pixel(x,50+i,1);
        }
    }
}
