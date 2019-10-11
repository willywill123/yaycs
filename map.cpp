#include "map.h"
#include "globals.h"
#include "graphics.h"
unsigned int BUCKETS = 250;
/**
 * Map is backed by a hashTable. adds all the environment
 * criteria from Sprite.h and graphics.h to program
 * the background and screen design
 */
 
/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static Map maze;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // TODO: Fix me!
    return (X + (Y)*map_width());
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    // TODO: Fix me! 
    //mod buckets (like key mod 4),
    unsigned hash_answer = key % 250;
    return hash_answer;
}

void maps_init()
{
    // TODO: Implement!    
    // Initialize hash table (with malloc)
    // Set width & height
    //similar to createhashtableentry
    //making a second map - map
    map.items = createHashTable(map_hash,BUCKETS);
    map.w = 50;
    map.h = 50;   
    maze.items = createHashTable(map_hash, BUCKETS);
    maze.w = 15;
    maze.h = 9;
}
    

Map* get_active_map()
{
    // There's only one map
    if(!active_map)
    return &map;
    //making a second map
    else
    return &maze;
}
//setting active map for maze and main
Map* set_active_map(int m)
{
    active_map = m;
    if(!active_map)
    return &map;
    else
    return &maze;
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    //add heart into here maybe as 'H'
    char lookup[] = {'W', 'P', 'H', 'N', 'D', 'L', 'G', 'P', 'G', 'K', 'B', 'B', 'C', 'W'};
    //wall, plant, heart, npc, door, lava, girl, player, gate, gatekey, boulder, broken boulder, chest, water
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}
//setting the width of the main and maze map
int map_width()
{
    if(!active_map) { 
    Map *map = get_active_map();
    return map->w;
    }
    else
    return maze.w;
}
//setting the height of the main and the maze map
int map_height()
{
    if(!active_map) {
    Map* map = get_active_map();
    return map->h;
    }
    else
    return maze.h;
}
//returns the area of the map and maze
int map_area()
{
    if (!active_map) {
    Map* map = get_active_map();
    return map->h * map->w;
    }
    else
    return maze.h * maze.w;
}
//implemented maze
MapItem* get_north(int x, int y)
{    
    Map* map = get_active_map();
    int key = XY_KEY(x, y - 1); //inverted y axis so down is north
    if(!active_map)
    return (MapItem*) getItem(map -> items, key);
    else
    return (MapItem*) getItem(maze.items,key);    
}
//implemented second map
MapItem* get_south(int x, int y)
{
    Map* map = get_active_map();
    int key = XY_KEY(x, y + 1); // inverted y axis so up is south (neg y- axis)
    if(!active_map)
    return (MapItem*) getItem(map -> items, key);
    else
    return (MapItem*) getItem(maze.items, key);
}
//implemented second map
MapItem* get_east(int x, int y)
{
    Map* map = get_active_map();
    int key = XY_KEY(x + 1, y); //x axis is not inverted
    if(!active_map)
    return (MapItem*) getItem(map -> items, key);
    else
    return (MapItem*) getItem(maze.items, key);
}
//implemented second map
MapItem* get_west(int x, int y)
{
    Map* map = get_active_map();
    int key = XY_KEY(x - 1, y); //x axis is not inverted
    if(!active_map)
    return (MapItem*) getItem(map -> items, key);
    else
    return (MapItem*) getItem(maze.items, key);
}
//implemented second map
MapItem* get_here(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y); //current location
    if(!active_map)
    return (MapItem*) getItem(map -> items, key);
    else
    return (MapItem*) getItem(maze.items, key);
}

//implemented second map
void map_erase(int x, int y)
{
    if(!active_map)
    deleteItem(map.items, map_hash(XY_KEY(x, y)));
    else
    deleteItem(maze.items, map_hash(XY_KEY(x, y)));
}
//add walls for the game border
void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}
//for maze environment
void add_lava(int x, int y, int dir, int len)
{
    for (int i = 0; i < len; i++)
    {
    MapItem* lava = (MapItem*) malloc(sizeof(MapItem));
    lava->type = LAVA;
    lava->draw = draw_lava;
    lava->walkable = false;
    lava->data = NULL;
    unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
    void* val = insertItem(get_active_map()->items, key, lava);
    if (val) free(val); // If something is already there, free it
    }
}



void add_plant(int x, int y)
{
    MapItem* plant = (MapItem*) malloc(sizeof(MapItem));
    plant->type = PLANT;
    plant->draw = draw_plant;
    plant->walkable = false;
    plant->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), plant);
    if (val) free(val); // If something is already there, free it
}

void add_heart(int x, int y)
{
    MapItem* heart = (MapItem*) malloc(sizeof(MapItem));
    heart->type = HEART;
    heart->draw = draw_heart;
    heart->walkable = false;
    heart->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), heart);
    if (val) free(val); // If something is already there, free it
}
void add_NPC(int x, int y)
{
    MapItem* npc = (MapItem*) malloc(sizeof(MapItem));
    npc->type = NPC;
    npc->draw = draw_NPC;
    npc->walkable = false;
    npc->data = 0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), npc);
    if (val) free(val); // If something is already there, free it
}
void add_door(int x, int y)
{
    MapItem* door = (MapItem*) malloc(sizeof(MapItem));
    door->type = DOOR;
    door->draw = draw_door;
    door->walkable = false;
    door->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), door);
    if (val) free(val); // If something is already there, free it
}
void add_fire(int x, int y)
{
    MapItem* fire = (MapItem*) malloc(sizeof(MapItem));
    fire->type = FIRE;
    fire->draw = draw_fire;
    fire->walkable = true; //to take damage from get.here()
    fire->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), fire);
    if (val) free(val); // If something is already there, free it
}
void add_girl(int x, int y)
{
    MapItem* girl = (MapItem*) malloc(sizeof(MapItem));
    girl->type = GIRL;
    girl->draw = draw_girl;
    girl->walkable = false;
    girl->data = 0;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), girl);
    if (val) free(val); // If something is already there, free it
}

void add_gate(int x, int y)
{
    MapItem* gate = (MapItem*) malloc(sizeof(MapItem));
    gate->type = GATE;
    gate->draw = draw_gate;
    gate->walkable = false;
    gate->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), gate);
    if (val) free(val); // If something is already there, free it
}
void add_gate_key(int x, int y)
{
    MapItem* gatekey = (MapItem*) malloc(sizeof(MapItem));
    gatekey->type = GATEKEY;
    gatekey->draw = draw_gate_key;
    gatekey->walkable = false;
    gatekey->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), gatekey);
    if (val) free(val); // If something is already there, free it
}
void add_boulder(int x, int y)
{
    MapItem* boulder = (MapItem*) malloc(sizeof(MapItem));
    boulder->type = BOULDER;
    boulder->draw = draw_boulder;
    boulder->walkable = false;
    boulder->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), boulder);
    if (val) free(val); // If something is already there, free it
}
void add_broken_boulder(int x, int y)
{
    MapItem* brokenBoulder = (MapItem*) malloc(sizeof(MapItem));
    brokenBoulder->type = BBOULDER;
    brokenBoulder->draw = draw_broken_boulder;
    brokenBoulder->walkable = true;
    brokenBoulder->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), brokenBoulder);
    if (val) free(val); // If something is already there, free it
}
void add_crown(int x, int y)
{
    MapItem* crown = (MapItem*) malloc(sizeof(MapItem));
    crown->type = CROWN;
    crown->draw = draw_crown;
    crown->walkable = false;
    crown->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), crown);
    if (val) free(val); // If something is already there, free it
}
void add_water(int x, int y)
{
    MapItem* water = (MapItem*) malloc(sizeof(MapItem));
    water->type = WATER;
    water->draw = draw_water;
    water->walkable = true; //to take damage from get.here()
    water->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), water);
    if (val) free(val); // If something is already there, free it
}









    