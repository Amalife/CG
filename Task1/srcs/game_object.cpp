#include "global.h"
#include "Image.h"
#include "game_object.h"


void GameObject::Draw_obj(Image &screen)
{
    for (int y = position.y; y < position.y + Height(); y++)
        for(int x = position.x; x < position.x + Width(); x++)
            screen.PutPixel(x, y, GetPixel(x - position.x, Height() - std::abs(y - position.y) - 1) );   
}