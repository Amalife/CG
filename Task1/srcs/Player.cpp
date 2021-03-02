#include "global.h"
#include "Player.h"

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::OtherLook(int n)
{
  for (int i = 0; i < 4; i++)
    if (i != n)
      look[i] = false;
}

bool Player::CheckCollisons(GameObject &obj)
{
  bool x_coll = coords.x + Width() >= obj.position.x && obj.position.x + obj.Width() >= coords.x;
  bool y_coll = coords.y + Width() >= obj.position.y && obj.position.y + obj.Height() >= coords.y;

  return x_coll && y_coll;
}

void Player::CheckAround(std::vector<GameObject> &chosen)
{
  for (GameObject &obj : chosen)
  {
    if (obj.position.y >= coords.y && obj.position.y <= coords.y + Width() && 
        coords.x >= obj.position.x - obj.Width()/2 && coords.x <= obj.position.x + obj.Width()/2)
      obstacle[0] = true;
    if (coords.y >= obj.position.y && coords.y <= obj.position.y + obj.Height() && 
        coords.x >= obj.position.x - obj.Width()/2 && coords.x <= obj.position.x + obj.Width()/2)
      obstacle[1] = true;
    if (coords.x >= obj.position.x && coords.x <= obj.position.x + obj.Width() && 
        coords.y >= obj.position.y - obj.Height()/2 && coords.y <= obj.position.y + obj.Height()/2)
      obstacle[2] = true;
    if (obj.position.x >= coords.x && obj.position.x <= coords.x + Width() && 
        coords.y >= obj.position.y - obj.Height()/2 && coords.y <= obj.position.y + obj.Height()/2)
      obstacle[3] = true;
  }
}

void Player::ProcessInput(MovementDir dir, GameMap &map)
{
  int move_dist = move_speed * 1;
  std::vector<GameObject> chosen;

  for (GameObject &obj : map.Objects)
    {
      if (CheckCollisons(obj))
      {
        if (obj.is_wall)
          chosen.push_back(obj);
        if (obj.is_danger && coords.y >= obj.position.y && coords.y <= obj.position.y + 3 * obj.Width()/4 &&
            (coords.x >= obj.position.x || coords.x + Width()/2 >= obj.position.x) && 
            (coords.x <= obj.position.x + obj.Width()/2 || coords.x + Width()/2 <= obj.position.x + obj.Width()))
          map.screen_dead = true;
        if (obj.is_door && coords.x >= obj.position.x && coords.x <= obj.position.x + Width() && 
            coords.y >= obj.position.y && coords.y <= obj.position.y + Width())
          map.screen_next_lvl = true;
      }
    }
  if (chosen.size())
  {
    CheckAround(chosen);
    chosen.clear();
  }

  switch(dir)
  {
    case MovementDir::UP:
      if (!obstacle[0])
      {
        old_coords.y = coords.y;
        coords.y += move_dist;
        look[0] = true;
        OtherLook(0);
      }
      break;
    case MovementDir::DOWN:
      if (!obstacle[1])
      {
        old_coords.y = coords.y;
        coords.y -= move_dist;
        look[1] = true;
        OtherLook(1);
      }
      break;
    case MovementDir::LEFT:
      if (!obstacle[2])
      {
        old_coords.x = coords.x;
        coords.x -= move_dist;
        look[2] = true;
        look[4] = false;
        OtherLook(2);
      }
      break;
    case MovementDir::RIGHT:
      if (!obstacle[3])
      {
        old_coords.x = coords.x;
        coords.x += move_dist;
        look[3] = true;
        look[4] = true;
        OtherLook(3);
      }
      break;
    default:
      break;
  }
  obstacle[0] = false;
  obstacle[1] = false;
  obstacle[2] = false;
  obstacle[3] = false;
  
}

Pixel mix(Pixel bufPixel, Pixel picPixel)
{
  picPixel.r = picPixel.a / 255.0 * (picPixel.r - bufPixel.r) + bufPixel.r;
  picPixel.g = picPixel.a / 255.0 * (picPixel.g - bufPixel.g) + bufPixel.g;
  picPixel.b = picPixel.a / 255.0 * (picPixel.b - bufPixel.b) + bufPixel.b;
  picPixel.a = 255;

  return picPixel;
}

void Player::Draw(Image &screen, Image &backup)
{
  if(Moved())
  {
    for(int y = old_coords.y; y < old_coords.y + Height(); ++y)
    {
      for(int x = old_coords.x; x < old_coords.x + Width(); ++x)
      {
        screen.PutPixel(x, y, backup.GetPixel(x, y));
      }
    }
    old_coords = coords;
  }

  
  for(int y = coords.y; y < coords.y + Height(); ++y)
  {
    for(int x = coords.x; x < coords.x + Width(); ++x)
    {
      if (look[3] || ((look[0] || look[1]) && look[4]))
        screen.PutPixel(x, y, mix(screen.GetPixel(x, y), GetPixel(x - old_coords.x, Height() - (y - old_coords.y) - 1 )));
      else if (look[2] || (look[0] || look[1] && !look[4]))
        screen.PutPixel(x, y, mix(screen.GetPixel(x, y), GetPixel(Width() - (x - old_coords.x), Height() - (y - old_coords.y) - 1 )));
    }
  }
}
