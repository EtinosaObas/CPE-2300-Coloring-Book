#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 192
#define HEIGHT 144
#define SCALE 4

typedef struct { Uint8 r, g, b; } Color;
static Uint8 framebuffer[WIDTH * HEIGHT];
static const Color palette[11] = {
    {255,255,255}, {20,20,20}, {220,50,50}, {50,120,220},
    {50,200,50}, {240,200,50}, {180,50,220}, {50,220,220},
    {240,120,30}, {120,120,120}, {0,0,0}
};

static void pixel(int x, int y, Uint8 color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        framebuffer[y * WIDTH + x] = color;
}

static void load_template(int number) {
    int x, y, i, w, dx, dy;
    memset(framebuffer, 0, sizeof(framebuffer));
    switch (number) {
    case 1:
        for (x = 50; x < 140; ++x) { pixel(x, 30, 1); pixel(x, 90, 1); }
        for (y = 30; y <= 90; ++y) { pixel(50, y, 1); pixel(140, y, 1); }
        break;
    case 2:
        for (x = 70; x < 130; ++x) pixel(x, 100, 1);
        for (y = 60; y <= 100; ++y) { pixel(70, y, 1); pixel(130, y, 1); }
        for (i = 0; i <= 30; ++i) { pixel(70 + i, 60 - i, 1); pixel(130 - i, 60 - i, 1); }
        break;
    case 3:
        for (y = 0; y < HEIGHT; ++y)
            for (x = 0; x < WIDTH; ++x) {
                dx = x - 96; dy = y - 72;
                if (abs(dx * dx + dy * dy - 35 * 35) < 35) pixel(x, y, 1);
            }
        break;
    case 4:
        for (i = 0; i < 40; ++i) {
            pixel(96 - i, 72 - i, 1); pixel(96 + i, 72 - i, 1);
            pixel(96 - i, 72 + i, 1); pixel(96 + i, 72 + i, 1);
        }
        break;
    case 5:
        for (x = 40; x < 152; ++x) {
            pixel(x, 20, 1); pixel(x, 124, 1); pixel(x, 72, 1);
        }
        for (y = 20; y <= 124; ++y) {
            pixel(40, y, 1); pixel(151, y, 1); pixel(96, y, 1);
        }
        break;
    case 6:
        for (y = 90; y < 130; ++y)
            for (x = 92; x < 101; ++x) pixel(x, y, 1);
        for (i = 0; i < 40; ++i) {
            w = 30 - i / 2;
            for (x = 96 - w; x < 96 + w; ++x) pixel(x, 50 + i, 1);
        }
        break;
    }
}

static void draw_text(SDL_Renderer *renderer, TTF_Font *font,
                      const char *message, SDL_Color color, int x, int y, int center) {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_FRect dst;
    if (!font) return;
    surface = TTF_RenderText_Blended(font, message, 0, color);
    if (!surface) return;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        dst.x = center ? (WIDTH * SCALE - surface->w) / 2 : x;
        dst.y = y;
        dst.w = surface->w;
        dst.h = surface->h;
        SDL_RenderTexture(renderer, texture, NULL, &dst);
        SDL_DestroyTexture(texture);
    }
    SDL_DestroySurface(surface);
}

static void draw_menu(SDL_Renderer *renderer, TTF_Font *large, TTF_Font *small) {
    static const char *options[] = {
        "1: Flag Template", "2: House Template", "3: Circle Template",
        "4: Diamond Template", "5: Window Pane Template", "6: Tree Template"
    };
    SDL_Color dark = {20,20,20,255};
    SDL_Color gray = {60,60,60,255};
    SDL_Color green = {40,120,80,255};
    int i;
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderClear(renderer);
    draw_text(renderer, large, "ETCH-A-SKETCH COLORING BOOK", dark, 0, 80, 1);
    draw_text(renderer, small, "Please select an image to color (Press 1 - 6):", gray, 0, 130, 1);
    for (i = 0; i < 6; ++i)
        draw_text(renderer, small, options[i], green, WIDTH * SCALE / 2 - 100, 180 + i * 35, 0);
}

static void draw_framebuffer(SDL_Renderer *renderer) {
    int x, y;
    SDL_FRect rect = {0, 0, SCALE, SCALE};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    for (y = 0; y < HEIGHT; ++y) {
        for (x = 0; x < WIDTH; ++x) {
            Color c = palette[framebuffer[y * WIDTH + x]];
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
            rect.x = x * SCALE;
            rect.y = y * SCALE;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

int main(int argc, char **argv) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *large, *small;
    SDL_Event event;
    int running = 1, in_menu = 1, current_color = 2;
    float mx, my;
    int fx, fy;
    const char *font_path = argc > 1 ? argv[1] : "C:/Windows/Fonts/arial.ttf";

    if (!SDL_Init(SDL_INIT_VIDEO)) { SDL_Log("SDL_Init: %s", SDL_GetError()); return 1; }
    if (!TTF_Init()) { SDL_Quit(); return 1; }
    if (!SDL_CreateWindowAndRenderer("Etch-a-Sketch Coloring Book - Menu Selection",
                                     WIDTH * SCALE, HEIGHT * SCALE, 0,
                                     &window, &renderer)) {
        SDL_Log("Window/renderer: %s", SDL_GetError());
        TTF_Quit(); SDL_Quit(); return 1;
    }
    large = TTF_OpenFont(font_path, 30);
    small = TTF_OpenFont(font_path, 22);
    if (!large || !small) {
        SDL_Log("Cannot load font: %s (pass a .ttf path as the first argument)", font_path);
        if (large) TTF_CloseFont(large);
        if (small) TTF_CloseFont(small);
        SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
        TTF_Quit(); SDL_Quit(); return 1;
    }

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = 0;
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                SDL_Keycode key = event.key.key;
                if (key == SDLK_ESCAPE) {
                    if (in_menu) running = 0;
                    else in_menu = 1;
                } else if (in_menu && key >= SDLK_1 && key <= SDLK_6) {
                    load_template((int)(key - SDLK_0));
                    in_menu = 0;
                } else if (!in_menu && key >= SDLK_0 && key <= SDLK_9) {
                    current_color = key == SDLK_0 ? 10 : (int)(key - SDLK_0);
                }
            }
        }
        if (in_menu) draw_menu(renderer, large, small);
        else {
            if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON_LMASK) {
                fx = (int)(mx / SCALE); fy = (int)(my / SCALE);
                if (fx >= 0 && fx < WIDTH && fy >= 0 && fy < HEIGHT &&
                    framebuffer[fy * WIDTH + fx] != 1)
                    framebuffer[fy * WIDTH + fx] = (Uint8)current_color;
            }
            draw_framebuffer(renderer);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    TTF_CloseFont(large); TTF_CloseFont(small);
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
    TTF_Quit(); SDL_Quit();
    return 0;
}
