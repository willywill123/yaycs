/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed. 
 * This will power the whole game that combines all the methods from the other files
 * Programs game events through the auxillary files
 */
 
// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map (); //main world 50x50
void init_maze_map (); //fire world that i created
void dragon_interact(MapItem* item); //main quest
void girl_interact(MapItem* item); //side quest
int Interact(MapItem* item); //action button actions
int main ();



struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key; //for the main quest
    int numPlants; // for the side quest
    bool omni; //for omnipotent button
    int health; //for fire damage
    bool menu; //toggles on quest menu page
    int numQuest; //counter for which quest you are on
    bool sad_girl_blessing; //flag for side quest
    
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below. added in the angle of the 
 * accelerometer tilt for player movement
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define OMNI_BUTTON 7 //for omnipotent mode
MapItem* nextBlock;
int get_action(GameInputs inputs)
{
    if(!inputs.b1)
        return OMNI_BUTTON;
    if(!inputs.b2)
        return ACTION_BUTTON;
    if(!inputs.b3)
        return MENU_BUTTON;
    if (inputs.ax < -0.4) {
        nextBlock = get_north(Player.x, Player.y);
        if (Player.omni || nextBlock -> walkable) {
            return GO_UP;
        }
    }
    if (inputs.ax >= 0.4) {
        nextBlock = get_south(Player.x, Player.y);
        if (Player.omni || nextBlock -> walkable) {
            return GO_DOWN;
        }
    }
    if (inputs.ay < -0.4) {
        nextBlock = get_east(Player.x, Player.y);
        if (Player.omni || nextBlock -> walkable) {
            return GO_RIGHT;
        }
    }
    if (inputs.ay >= 0.4) {
        nextBlock = get_west(Player.x, Player.y);
        if (Player.omni || nextBlock -> walkable) {
            return GO_LEFT;
        }
    }
    return NO_ACTION;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define WIN 2
#define FULL_DRAW 3
//define locations
MapItem* up;
MapItem* left;
MapItem* right;
MapItem* down;
MapItem* here;
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;

    // define player movement here
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    // made methods for player movement through a switch statement
    switch(action) {
        case GO_UP:
            //debug
            pc.printf("Up\r\n");
            Player.y -= 1;
            break;

        case GO_LEFT:
            pc.printf("Left\r\n");
            Player.x -= 1;
            break;

        case GO_DOWN:
            pc.printf("Down\r\n");
            Player.y += 1;
            break;

        case GO_RIGHT:
            pc.printf("Right\r\n");
            Player.x += 1;
            break;

        case ACTION_BUTTON:

            int up = Interact(get_north(Player.x, Player.y));
            if (up == GAME_OVER) {
                return up;
            }
            int left = Interact(get_west(Player.x, Player.y));
            if (left == GAME_OVER) {
                return left;
            }
            int right = Interact(get_east(Player.x, Player.y));
            if (right == GAME_OVER) {
                return right;
            }
            int down = Interact(get_south(Player.x, Player.y));
            if (down == GAME_OVER) {
                return down;
            }
            int here = Interact(get_here(Player.x, Player.y));
            if(NO_ACTION)
                return action;
            return FULL_DRAW;


        case MENU_BUTTON:
            if(!Player.menu) {
                questMenu(Player.numQuest);
                Player.menu = true;
            }
            break;
        case OMNI_BUTTON:
            Player.omni = !Player.omni; //toggle on and off omnipotent mode
            if(Player.omni) {
                become_boss();
            } else
                clear_boss();
            break;

        default:
            break;
    }
    return NO_RESULT;
}
/**
INTERACT
interact with the environment through the action button (b2) that looks for the
item type 
*/
int Interact(MapItem* item)
{
    switch(item -> type) {
        case NPC:
            dragon_interact(item);
            return FULL_DRAW;
        case GIRL:
            girl_interact(item);
            return FULL_DRAW;
        case DOOR:
            init_maze_map();
            set_active_map(1);
            Player.x = 1;
            Player.y = 6;
            draw_game(FULL_DRAW);
            break;
        case HEART:
            Player.has_key = 1;
            map_erase(4,1); //erase the heart because u took it!
            wait(1);
            char* line1;
            char* line2;
            line1 = "You found the";
            line2 = "heart. Press ";
            speech(line1, line2);
            line1 = "the action but";
            line2 = "ton to return.";
            speech(line1, line2);
            wait(1);
            set_active_map(0); //return to the main map
            //map_erase(25, 25); //erase yellow door.
            Player.numQuest = 2;
            Player.x = 20;
            Player.y = 20;
            draw_game(FULL_DRAW);
            return FULL_DRAW;
        case CROWN:
            Victory();
            break;
        //put music here?
        case PLANT:
            Player.numPlants++;
            char* l1;
            char* l2;
            l1 = "Do these even";
            l2 = "taste good?";
            speech(l1, l2);
            speaker.period(1.0 / 392);
            speaker = 0.005;
            wait(0.5);
            speaker = 0;
            //make a sound effect for collecting plants            
            return FULL_DRAW;
        case BOULDER:
            char* li1;
            char* li2;
            if(Player.has_key) {
                add_broken_boulder(7,6);
                draw_game(FULL_DRAW);
                wait(1);
                li1 = "BAM!";
                li2 = "O WOW.";
                speech(li1, li2);
                li1 = "The power of";
                li2 = "the key!";
                speech(li1, li2);
            } else {
                char* line1;
                char* line2;
                line1 = "There's nothing";
                line2 = "but trash here.";
                speech(line1, line2);
            }
            return FULL_DRAW;
        case GATE:
            if(Player.has_key) {
                char* line1;
                char* line2;
                add_gate_key(10, 1);
                draw_game(FULL_DRAW);
                wait(2);
                line1 = "CLUNK!";
                line2 = "The door opened!";
                speech(line1, line2);
                wait(1);
                set_active_map(1);
                Player.x = 1;
                Player. y = 2; //do not start in the wall
                Player.numQuest = 4; //find the crown
                draw_game(FULL_DRAW);
            } else {
                char* line1;
                char* line2;
                line1 = "There is a myst";
                line2 = "erious energy.";
                speech(line1, line2);
                line1 = "The big gate is";
                line2 = "locked. You need";
                speech(line1, line2);
                line1 = "a special key to";
                line2 = "open it....... ";
                speech(line1, line2);
            }
            return FULL_DRAW;

        default :
            return NO_RESULT;
    }
}

void dragon_interact(MapItem* item)
{
    char* line1;
    char* line2;
    switch(item -> data) {
        case 0:
            line1 = "Drake: Hi, please";
            line2 = "fix my broken";
            speech(line1, line2);
            line1 = "heart. Find it ";
            line2 = "in the yellow box.";
            speech(line1, line2);
            Player.numQuest = 1; //for case: write find heart at (25,25)
            Player.has_key = 0;
            
            item -> data++;
            break;
        case 1:
            if(!Player.has_key) {
                line1 = "You do not have";
                line2 = "my beating <3.";
                speech(line1, line2);
                line1 = "Please go to the";
                line2 = "yellow box. Run!";
                speech(line1, line2);
            } else {
                line1 = "Thank you. You";
                line2 = "have saved me.";
                speech(line1, line2);
                line1 = "I no longer feel";
                line2 = "broken anymore.";
                speech(line1, line2);
                line1 = "Take this key. It";
                line2 = "can open a gate.";
                Player.numQuest = 3; //find the locked gate
                speech(line1, line2);
                item -> data++;
                //add key to open the gate
            }
            break;
        default:
            line1 = "My whole village";
            line2 = "thanks you.";
            speech(line1, line2);
            line1 = "How can I ever";
            line2 = "repay you?";
            speech(line1, line2);
            line1 = "Talk to sad girl";
            line2 = "for a side quest.";
            speech(line1, line2);
    }
}
void girl_interact(MapItem* item)
{
    char* l1;
    char* l2;
    switch(item -> data) {
        case 0:
            l1 = "Sad Girl: Can";
            l2 = "you help me?";
            Player.numQuest = 5; //side quest: Collect three plants
            speaker.period(1.0 / 440.0); //C4
            speaker = 0.005;
            wait(0.6);
            speech(l1, l2);
            l1 = "I need you to";
            l2 = "collect 3 plants";
            speech(l1, l2);
            l1 = "I am very hungry.";
            l2 = "Please help me.";
            speech(l1,l2);
            item -> data++;
            Player.numPlants = 0; //initialize count
            break;
        case 1:
            if(Player.numPlants < 3) {
                l1 = "I asked for 3";
                l2 = "plants. I think";
                speech(l1, l2);
                l1 = "you need a little";
                l2 = "more. You can do";
                speech(l1, l2);
                l1 = "it. I believe";
                l2 = "in you...";
                speech(l1, l2);
            } else {
                l1 = "Thank you so much.";
                l2 = "Poof Poof Spoof.";
                speech(l1, l2);
                l1 = "I have given you";
                l2 = "more health.";
                speech(l1, l2);
                Player.sad_girl_blessing = true;
            }
            break;
        default:
            l1 = "These plants are";
            l2 = "delicious. Thanks.";
            speech(l1,l2);
    }
}


/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (i == 0 && j == 0) { // Only draw the player on init
                draw_player(u, v, Player.has_key);
                continue;
            } else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                    if(Player.menu) {
                        Player.menu = false;
                    } //toggle between the two states
                }
            } else if (init) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status(Player.health,Player.x, Player.y);
    draw_lower_status();
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 * Added in a second map for further game development
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39) {
        add_plant(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");

    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
    add_NPC(1, 1);
    add_girl(10, 10);
    add_door(25, 25);
    add_gate(10, 1);
    add_fire(4, 4);
    for(int row = 11; row < 15; row++) {
        for(int col = 11; col < 15; col++) {
            add_water(row, col);
        }
    }
    print_map();
}
void init_maze_map()
{
    Map* map = set_active_map(1);
    //for (int i = 3; i < 9; i++) {
    //    add_lava(1,i);
    //    }
    add_lava(0,              0,              HORIZONTAL, map_width());
    add_lava(0,              map_height()-1, HORIZONTAL, map_width());
    add_lava(0,              0,              VERTICAL,   map_height());
    add_lava(map_width()-1,  0,              VERTICAL,   map_height());
    add_heart(4, 1);
    add_boulder(7, 6);
    add_crown(13, 6);
    for (int row = 1; row < 9; row+=2) {
        for(int col = 1; col < 9; col+=2) {
            add_fire(row, col);
        }
    }
    for (int row = 8; row < 14; row++) {
        for (int col = 1; col < 6; col++) {
            add_fire(row, col);
        }
    }
    for (int row = 8; row < 14; row++) {
        int col = 7;
        add_fire(row, col);
    }

    print_map();
}
/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    //start with the welcomme screen
    int timer = 1;
    StartMenu();
    while(timer) {

        wait(0.5);
        uLCD.text_string("-INSERT COIN-", 4, 13, FONT_7X8, WHITE);
        wait(0.5);
        uLCD.filled_rectangle(0, 90, 127, 90, BLACK);
        if(button2.read() == 0) {
            timer = 0;
        }
    }
    maps_init();
    init_main_map();
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.health = 50;
    Player.numQuest = 0; //find a dragon
    Player.sad_girl_blessing = false;
    // Initialize game state
    //set_active_map(0);
    //Player.x = Player.y = 5;
    //Player.health = 100;
    GameInputs in;
    // Initial drawing
    draw_game(true);

    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t;
        t.start();

        // Actually do the game update:
        // 1. Read inputs
        in = read_inputs();
        // 2. Determine action (get_action)
        int action = get_action(in);
        // 3. Update game (update_game)
        int result = update_game(action);
        //update health each time player walks over fire        
        if((get_here(Player.x, Player.y))-> type == FIRE) {
            wait(0.5);
            Player.health = Player.health - 10.0;
            // 3b. Check for game over
            //switching over to the Game Over screen if health is 0
            //timer for restart game
            if(Player.health < 1) {
                uLCD.cls();
                GameOver();
                uLCD.cls();
                Player.health = 50;
                uLCD.locate(4, 7);
                uLCD.printf("Respawning in");
                uLCD.locate(8, 9);
                uLCD.printf("3");
                wait(1);
                uLCD.locate(8, 9);
                uLCD.printf("2");
                wait(1);
                uLCD.locate(8, 9);
                uLCD.printf("1");
                wait(1);
                uLCD.cls();
                set_active_map(0);
                Player.x = 3;
                Player.y = 3;
            }
        }
    
        //water healing for extra game criteria
        if((get_here(Player.x, Player.y))-> type == WATER) {
            wait(0.75);
            if (Player.health > 100) {
                Player.health = Player.health;
            } else (Player.health = Player.health + 1.0);
        }
        if (Player.sad_girl_blessing) {
            Player.health = Player.health + 50;
            Player.sad_girl_blessing = false;
            }        
        
        // 4. Draw frame (draw_game)
        draw_game(result);

        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}
