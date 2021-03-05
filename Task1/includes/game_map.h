#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "global.h"
#include "game_object.h"
#include "Image.h"

class GameMap
{
public:

    std::vector<GameObject> Objects;
    Point player_start;
    bool screen_next_lvl = false;
    bool screen_end_lvl = false;
    bool screen_dead = false;
    Image backup;

    GameMap() {}

    void loadFile(const char *path, Point *start, std::vector<Image> &anima);
    
    void initMap(std::vector<std::vector<char>> tile_data, Point *start, std::vector<Image> &anima);

};

class Game
{
public:
    std::vector<GameMap> Maps;
    bool screen_load = true;
    int level = 0;
    int timer = 0;

    Game() {}
    
    void Render(GameMap &map, Image &screen);
    void Update(GameMap &map, Image &screen, int i);
};

#endif