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
  bool x_coll = coords.x + Width() >= obj.position.x && obj.position.x + obj.width >= coords.x;
  bool y_coll = coords.y + Width() >= obj.position.y && obj.position.y + obj.height >= coords.y;

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

void Player::CheckPlayer(GameMap &map)
{
  std::vector<GameObject> chosen;

  for (GameObject &obj : map.Objects)
    {
      if (CheckCollisons(obj))
      {
        if (obj.is_wall)
          chosen.push_back(obj);
        if (obj.is_danger && coords.y >= obj.position.y && coords.y <= obj.position.y + 3 * obj.width/4 &&
            (coords.x >= obj.position.x || coords.x + Width()/2 >= obj.position.x) && 
            (coords.x <= obj.position.x + obj.width/2 || coords.x + Width()/2 <= obj.position.x + obj.width))
          map.screen_dead = true;
        if (obj.is_door && coords.x >= obj.position.x && coords.x <= obj.position.x + obj.width && 
            coords.y >= obj.position.y && coords.y <= obj.position.y + obj.width)
          map.screen_next_lvl = true;
      }
    }
  if (chosen.size())
  {
    CheckAround(chosen);
    chosen.clear();
  }
}

void Player::ProcessInput(MovementDir dir, GameMap &map)
{
  int move_dist = move_speed * 1;
  
  CheckPlayer(map);
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

void Player::Draw(Image &screen, Image &backup)
{
  bool mv_cond = Moved();
  if(mv_cond)
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
  if (num_anim == 4)
    num_anim = 0;

  
  for(int y = coords.y; y < coords.y + Height(); ++y)
  {
    for(int x = coords.x; x < coords.x + Width(); ++x)
    {
      if (look[3] || ((look[0] || look[1]) && look[4]))
        if (mv_cond)
          screen.PutPixel(x, y, mix(screen.GetPixel(x, y), player_run[num_anim].GetPixel(x - old_coords.x, Height() - (y - old_coords.y) - 1 )));
        else
        {
          screen.PutPixel(x, y, backup.GetPixel(x, y));
          screen.PutPixel(x, y, mix(screen.GetPixel(x, y), player_idle[num_anim].GetPixel(x - old_coords.x, Height() - (y - old_coords.y) - 1 )));
        }
      else if (look[2] || (look[0] || look[1] && !look[4]))
        if (mv_cond)
          screen.PutPixel(x, y, mix(screen.GetPixel(x, y), player_run[num_anim].GetPixel(Width() - (x - old_coords.x) - 1, Height() - (y - old_coords.y) - 1 )));
        else
        {
          screen.PutPixel(x, y, backup.GetPixel(x, y));
          screen.PutPixel(x, y, mix(screen.GetPixel(x, y), player_idle[num_anim].GetPixel(Width() - (x - old_coords.x) - 1, Height() - (y - old_coords.y) - 1 )));
        }
    }
  }  
}
