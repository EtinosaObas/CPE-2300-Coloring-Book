MUSTYV COLORING BOOK -- C PSEUDOCODE FOR RISC-V TRANSLATION

IMPORTANT: This is not an SDL application or an executable for Windows.
The C source models memory-mapped MustyV registers described in the professor's guide.
It is intended as a reference for manual translation into RISC-V assembly.
It has not been executed inside MustyV.

Person 1: templates.c -- set_pixel, load_template
Person 2: graphics.c -- init_palette, clear_screen, draw_digit, draw_menu, present
Person 3: main.c -- handle_key, handle_mouse, main
Shared: coloring.h -- constants, register addresses, prototypes

MustyV device map (from professor's guide):
0x20000000 framebuffer (192*144 bytes)
0x21000000 palette (32-bit 0x00RRGGBB entries)
0x21001008 PRESENT
0x4000002C queued keypress (read consumes event)
0x40000030 mouse x
0x40000034 mouse y
0x40000038 mouse buttons (bit 0 = left button)

Controls: In menu press 1..6 to load a template. In canvas press 0..9
for a color; left mouse button paints; ESC returns to menu.
The original menu's text is replaced by six numbered boxes to avoid
SDL_ttf or a complete bitmap font. Pixel dimensions remain 192x144.

For assembly: use .equ for addresses, sb for framebuffer bytes, sw for
palette and PRESENT words, lw for input registers. Use call/ret for
functions and preserve s-registers and ra according to calling convention.
Avoid drawing the menu every frame because it would erase the selection.

Note: An infinite busy loop with PRESENT every iteration may be slow in
MustyV; once functional, consider only presenting when the screen changes.
The original tree template uses filled pixels of palette index 1, which
are protected from painting, just as in the Python original.
