#include "global.h"
#include "Image.h"
#include "game_object.h"

void GameObject::Draw_obj(Image &screen)
{
    if (is_spike || is_danger)
    {
        for (int y = position.y; y < position.y + height; y++)
            for(int x = position.x; x < position.x + width; x++)
                if (num_an == 3 && glit > 3)
                    screen.PutPixel(x, y, mix(screen.GetPixel(x,y), anim[glit].GetPixel(x - position.x, height - std::abs(y - position.y) - 1)) );
                else    
                    screen.PutPixel(x, y, anim[num_an].GetPixel(x - position.x, height - std::abs(y - position.y) - 1) );
    }
    else
    {
        for (int y = position.y; y < position.y + Height(); y++)
            for(int x = position.x; x < position.x + Width(); x++)
                screen.PutPixel(x, y, GetPixel(x - position.x, Height() - std::abs(y - position.y) - 1) ); 
    }
}