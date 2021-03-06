#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "game_map.h"
#include "global.h"


enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

struct Player : public Image
{
  explicit Player(Point pos = {.x = 10, .y = 10}) : Image("resources/frames/knight_m_idle_anim_f0.png"),
                 coords(pos), old_coords(coords) {};

  bool Moved() const;
  bool CheckCollisons(GameObject &obj);
  bool obstacle[4] = {false, false, false, false};
  bool look[5] = {false, false, false, true, true};
  void ProcessInput(MovementDir dir, GameMap &map);
  void Draw(Image &screen, Image &backup);
  void CheckPlayer(GameMap &map);
  void CheckAround(std::vector<GameObject> &chosen);
  void OtherLook(int n);
  std::vector<Image> player_run;
  std::vector<Image> player_idle;
  Point coords {.x = 10, .y = 10};
  //std::vector<std::string> mv_path;
  int num_anim = 0;
  //int time = 0;

private:
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 255, .b = 255, .a = 0};
  int move_speed = 2;

};

#endif //MAIN_PLAYER_H
