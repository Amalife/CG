#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

constexpr GLsizei WINDOW_WIDTH = 640, WINDOW_HEIGHT = 640;

Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, sizeof(Pixel))) != nullptr)
  {
    size = width * height * channels;
    //std::cout << "width: " << width << " height " << height <<  std::endl;
  }
  else
    std::cout << "image can't be open" << std::endl;
}

Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * a_height ]{};

  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}

int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}

Pixel Image::ToFromDark(Pixel bufPixel, float k)
{
  Pixel chgPixel;
  chgPixel.r = bufPixel.r * k;
  chgPixel.g = bufPixel.g * k;
  chgPixel.b = bufPixel.b * k;
  chgPixel.a = bufPixel.a * k;

  return chgPixel;
}

void Image::Draw(Image &screen, float k)
{
  for (int y = 0; y < Height(); ++y)
  {
    for (int x = 0; x < Width() ; ++x)
    {
      PutPixel(x, y, ToFromDark(screen.GetPixel(x, y), k));
    }
  }  
}

void Image::Read_img(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, sizeof(Pixel))) != nullptr)
  {
    size = width * height * channels;
    //std::cout << "width: " << width << " height " << height <<  std::endl;
  }
  else
    std::cout << "image can't be open" << std::endl;
}

Pixel Image::mix(Pixel bufPixel, Pixel picPixel)
{
  picPixel.r = picPixel.a / 255.0 * (picPixel.r - bufPixel.r) + bufPixel.r;
  picPixel.g = picPixel.a / 255.0 * (picPixel.g - bufPixel.g) + bufPixel.g;
  picPixel.b = picPixel.a / 255.0 * (picPixel.b - bufPixel.b) + bufPixel.b;
  picPixel.a = 255;

  return picPixel;
}

void Image::TakePix(int x_pos, int y_pos, Image &screen)
{
  if (y_pos - 16 * 10 < 0)
    y_pos = 16 * 10;
  if (x_pos - 16 * 10 < 0)
    x_pos = 16 * 10;
  if (y_pos + 16 * 10 > screen.Height())
    y_pos = screen.Height() - 16 * 10;
  if (x_pos + 16 * 10 > screen.Width())
    x_pos = screen.Width() - 16 * 10;

  for (int y = y_pos - 16 * 10; y < y_pos + 16 * 10; y++)
    for (int x = x_pos - 16 * 10; x < x_pos + 16 * 10; x++)
      PutPixel(x - (x_pos - 16 * 10), y - (y_pos - 16 * 10), screen.GetPixel(x, y));
}

void Image::ChangeSize(int a_width, int a_height, int a_channels)
{
  delete data;
  data = new Pixel[a_width * a_height]{};

  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}

/*Image::~Image()
{
  if(self_allocated)
    delete [] data;
  else
  {
    stbi_image_free(data);
  }
}*/