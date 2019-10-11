//////////////////////////////////////////////////////////
//Graphics
//includes all the drawing tools for environment layout
//background, start screen, end screen, status bars
//and items in the hashmap
//////////////////////////////////////////////////////////

#include "graphics.h"
#include "Sprites.h" //this has all the piskel data
#include "globals.h"
#include <stdint.h>

void draw_player(int u, int v, int key)
{
    if(!key)
    uLCD.BLIT(u, v, 11, 11, player_pixel);
    else
    uLCD.BLIT(u, v, 11, 11, player_with_heart_pixel);
}
    
    //put in key found / not found
    /*if (!key)
    //player with key = array;
    uLCD.filled_rectangle(u, v, u+11, v+11, player with key)*/
    

#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define DIRT   BROWN
#define LIGHTGREEN  0x7AE996
#define GRASS  GREEN
void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = DIRT;
        else if (img[i] == '5') colors[i] = LGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_wall(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BROWN);
}

void draw_plant(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}
void draw_heart(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, heart_pixel);
}
void draw_NPC(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, NPC_pixel);
}
void draw_door(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, YELLOW);
}
void draw_fire(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, fire_pixel);
}
void draw_lava(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, lava_pixel);
}
void draw_girl(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, girl_pixel);
}
void draw_gate(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, gate_pixel);
}
void draw_gate_key(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, gate_key_pixel);
}
void draw_boulder(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, boulder_pixel);
}
void draw_broken_boulder(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, broken_boulder_pixel);
}
void draw_crown(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, crown_pixel);
}
void draw_water(int u, int v)
{
    uLCD.BLIT(u, v, 11, 11, water_pixel);
}
void become_boss()
{
    uLCD.BLIT(0, 122, 8, 10, snapchat_pixel);
}
void clear_boss()
{
    uLCD.filled_rectangle(0, 122, 10, 128, BLACK);
}
void draw_upper_status(int health, int player_x, int player_y)
{
    // Draw bottom border of status bar
    uLCD.line(0, 9, 128, 9, GREEN);
    
    // Add other status info drawing code here
    //add coordinate tracking here
    char healthbar[15];
    sprintf(healthbar, "HP:%d ", health);
    uLCD.text_string(healthbar, 0, 0, FONT_5X7, WHITE);
    char coordinates[8];
    sprintf(coordinates, "XY: %d, %d ", player_x, player_y);
    uLCD.text_string(coordinates, 8, 0, FONT_5X7, BLUE);
}

void draw_lower_status()
{
    // Draw top border of status bar
    uLCD.line(0, 118, 128, 118, GREEN);
    //add omni mode indicator here
    
}

void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}

//making the start menu, game over, and victory screens
void StartMenu() {
    uLCD.filled_rectangle(10, 20, 118, 108, BLACK);
    uLCD.locate(3, 3);
    uLCD.printf("THE LEGEND OF\n");
    uLCD.locate(3, 5);
    uLCD.printf("THE AXOLOTL");
 
    uLCD.locate(4, 9);
    uLCD.printf("Hold button 2"); 
    uLCD.locate(4, 10);
    uLCD.printf("to proceed.");
    for (int i = 0; i < 1; i++) {
        speaker.period(1.0 / 440.0); // C4
        speaker = 0.005;
        wait(0.7);
        speaker.period(1.0 / 532.2); // C5
        speaker = 0.005;
        wait(0.7);
        speaker.period(1.0 / 659.2); // E4
        speaker = 0.005;
        wait(0.7);
        speaker = 0;
        }
  
}
//this will load the game over screen 
//and show a GAME OVER text. 
void GameOver() {
    uLCD.filled_rectangle(10, 20, 118, 108, BLACK);
    uLCD.locate(4, 7);
    uLCD.printf("GAME OVER!");
    uLCD.locate(4, 3);
    uLCD.printf("You burned");
    uLCD.locate(4, 5);
    uLCD.printf("in the fire!");
    wait(3);
}
//this will load the victory screen
//and play victorious music through
//the speaker on the circuit
void Victory() {
    uLCD.filled_rectangle(10, 20, 118, 108, BLACK);
    uLCD.locate(2, 4);
    uLCD.printf("You're a HERO!");
    uLCD.locate(5, 7);
    uLCD.printf("You WIN.");
    //play the C major appregio  
    for (int i = 0; i < 1; i++) {
        speaker.period(1.0 / 261.6);
        speaker = 0.005;
        wait(0.6);
        speaker.period(1.0 / 329.6);
        speaker = 0.005;
        wait(0.6);
        speaker.period(1.0 / 392);
        speaker = 0.005;
        wait(0.6);
        speaker.period(1.0 / 440);
        speaker = 0.005;
        wait(0.6);
        speaker.period(1.0 / 392);
        speaker = 0.005;
        wait(0.6);
        speaker.period(1.0 / 329.6);
        speaker = 0.005;
        wait(0.6);
        speaker.period(1.0 / 261.6);
        speaker = 0.005;
        wait(0.6);
        speaker = 0;
    }
}

//this method takes in the number of the quest
//and outputs the correct speech bubble through
//a switch statement
void questMenu(int numQuest) {    
    uLCD.cls();    
    switch(numQuest) {
        case 0: uLCD.locate(2, 2);
                uLCD.printf("MAIN QUEST:");
                uLCD.locate(4, 4);
                uLCD.printf("Find the blue dragon.");
                break;
        case 1: uLCD.locate(2, 2);
                uLCD.printf("MAIN QUEST:");
                uLCD.locate(4, 4);
                uLCD.printf("Find the door. (25, 25)"); //finding the door
                break;
        case 2: uLCD.locate(2, 2);
                uLCD.printf("MAIN QUEST:");
                uLCD.locate(4, 4);
                uLCD.printf("Return to dragon."); //use the gate
                break;
        case 3: uLCD.locate(2, 2);
                uLCD.printf("MAIN QUEST:");
                uLCD.locate(4, 4);
                uLCD.printf("Find the gate."); //once u find the gate, find crown to win
                break;  
        case 4: uLCD.locate(2, 2);
                uLCD.printf("MAIN QUEST:");
                uLCD.locate(4, 4);
                uLCD.printf("Find the crown.");
                break;
        case 5: uLCD.locate(2, 2);
                uLCD.printf("SIDE QUEST:");
                uLCD.locate(4, 4);
                uLCD.printf("Collect 3 plants.");
                break;
            }
            wait(1.5);
        }