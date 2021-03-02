#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "global.h"
#include "Image.h"

class GameObject: public Image
{
public:
    Point position;
    GameObject(int x_val, int y_val, const std::string path) : Image(path) 
    {
        position.x = x_val;
        position.y = y_val;
    }
    GameObject(int x_val, int y_val) 
    {
        position.x = x_val;
        position.y = y_val;
    }
    bool is_wall = false;
    bool is_danger = false;
    bool is_door = false;

    void Draw_obj(Image &screen);
};

#endif