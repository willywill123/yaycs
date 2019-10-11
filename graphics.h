#ifndef GRAPHICS_H
#define GRAPHICS_H
/**header file for graphics
 * methods defined in graphics.cpp
/**
 * Takes a string image and draws it to the screen. The string is 121 characters
 * long, and represents an 11x11 tile in row-major ordering (across, then down,
 * like a regular multi-dimensional array). The available colors are:
 *      R = Red
 *      Y = Yellow
 *      G = Green
 *      D = Brown ("dirt")
 *      5 = Light grey (50%)
 *      3 = Dark grey (30%)
 *      Any other character is black
 * More colors can be easily added by following the pattern already given.
 */
void draw_img(int u, int v, const char* img);

/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */
void draw_nothing(int u, int v);
void draw_wall(int u, int v);
void draw_plant(int u, int v);
void draw_heart(int u, int v);
void draw_NPC(int u, int v);
void draw_door(int u, int v);
void draw_fire(int u, int v);
void draw_lava(int u, int v);
void draw_girl(int u, int v);
void draw_player(int u, int v, int key);
void draw_gate(int u, int v);
void draw_gate_key(int u, int v);
void draw_boulder(int u, int v);
void draw_broken_boulder(int u, int v);
void draw_crown(int u, int v);
void draw_water(int u, int v);
void become_boss();
void clear_boss();


/**
 * Draw the upper status bar.
 */
void draw_upper_status(int health, int player_x, int player_y);

/**
 * Draw the lower status bar.
 */ 
void draw_lower_status();

/**
 * Draw the border for the map.
 */
void draw_border();
void StartMenu();
void Victory();
void GameOver();
void questMenu(int numQuest);

#endif // GRAPHICS_H