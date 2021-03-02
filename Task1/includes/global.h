#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

#define GLFW_DLL
#include <GLFW/glfw3.h>

struct Point
{
  int x;
  int y;
};

struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};
//constexpr GLsizei WINDOW_WIDTH = 512, WINDOW_HEIGHT = 512;
//Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

#endif