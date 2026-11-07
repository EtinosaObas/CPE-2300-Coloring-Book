#include "coloring.h"
/* Color values are 0x00RRGGBB. */
void init_palette(void) {
    static const uint32_t colors[11] = {
        0xFFFFFF, 0x141414, 0xDC3232, 0x3278DC, 0x32C832,
        0xF0C832, 0xB432DC, 0x32DCDC, 0xF0781E, 0x787878, 0x000000
    };
    int i;
    for (i = 0; i < 11; i++) PAL[i] = colors[i];
}
void clear_screen(int color) {
    int i;
    for (i = 0; i < WIDTH*HEIGHT; i++) FB[i] = (uint8_t)color;
}
/* A tiny 3x5 bitmap font: each row is 3 bits, leftmost bit is 4. */
void draw_digit(int digit, int x, int y, int color) {
    static const uint8_t digits[10][5] = {
        {7,5,5,5,7}, {2,6,2,2,7}, {7,1,7,4,7}, {7,1,7,1,7},
        {5,5,7,1,1}, {7,4,7,1,7}, {7,4,7,5,7}, {7,1,1,1,1},
        {7,5,7,5,7}, {7,5,7,1,7}
    };
    int row, col, bit;
    if (digit < 0 || digit > 9) return;
    for (row = 0; row < 5; row++) for (col = 0; col < 3; col++) {
        bit = 4 >> col;
        if ((digits[digit][row] & bit) != 0)
            set_pixel(x+col, y+row, color);
    }
}
/* Six numbered, selectable boxes. Press 1..6 to choose a template. */
void draw_menu(void) {
    int option, x, y, i;
    clear_screen(0);
    for (option = 1; option <= 6; option++) {
        x = 25 + ((option-1) % 3)*55;
        y = 35 + ((option-1) / 3)*60;
        for (i = 0; i < 38; i++) {
            set_pixel(x+i,y,1); set_pixel(x+i,y+38,1);
            set_pixel(x,y+i,1); set_pixel(x+37,y+i,1);
        }
        draw_digit(option,x+17,y+17,2);
    }
}
void present(void) {
    REG32(PRESENT_ADDR) = 1;
}
