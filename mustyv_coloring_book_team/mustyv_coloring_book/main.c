#include "coloring.h"
/* The queued key register returns 0 when no new key is available. */
void handle_key(int key, int *in_menu, int *current_color) {
    if (key == 0) return;
    if (key == 27) { /* ESC returns to menu */
        *in_menu = 1;
        draw_menu();
    } else if (*in_menu) {
        if (key >= '1' && key <= '6') {
            load_template(key - '0');
            *in_menu = 0;
        }
    } else if (key >= '0' && key <= '9') {
        *current_color = (key == '0') ? 10 : key - '0';
    }
}
/* Mouse coordinates are already framebuffer coordinates in MustyV. */
void handle_mouse(int current_color) {
    int buttons = (int)REG32(MOUSE_BUTTONS_ADDR);
    int x, y, index;
    if ((buttons & 1) == 0) return;
    x = (int)REG32(MOUSE_X_ADDR);
    y = (int)REG32(MOUSE_Y_ADDR);
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    index = y * WIDTH + x;
    if (FB[index] != 1) FB[index] = (uint8_t)current_color;
}
int main(void) {
    int in_menu = 1;
    int current_color = 2;
    int key;
    init_palette();
    draw_menu();
    present();
    while (1) {
        key = (int)REG32(KEY_QUEUE_ADDR);
        handle_key(key, &in_menu, &current_color);
        if (!in_menu) handle_mouse(current_color);
        present();
    }
    return 0;
}
