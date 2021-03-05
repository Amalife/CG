#include "global.h"
#include "game_map.h"


void GameMap::loadFile(const char *path, Point *start, std::vector<Image> &anima)
{
    char tile_index;
    GameMap map;
    std::string line;
    std::ifstream file(path);
    std::vector<std::vector<char>> tile_data;

    if (file)
    {
        while (std::getline(file, line))
        {
            //std::cout << line;
            std::vector<char> row;
            int i = 0;

            while (line[i])
            {
                row.push_back(line[i]);
                i++;
            }
            tile_data.push_back(row);
        }
        file.close();
    }
    if (tile_data.size() > 0)
    {
        std::reverse(tile_data.begin(), tile_data.end());
        this->initMap(tile_data, start, anima);
    }
}

void GameMap::initMap(std::vector<std::vector<char>> tile_data, Point *start, std::vector<Image> &anima)
{
    int height = tile_data.size();
    int width = tile_data[0].size();
    //std::cout << "width = " << width << " height = " << height;

    for (int y = 0; y < height; y++)
    {
        
        for (int x = 0; x < width; x++)
        {
            if (tile_data[y][x] == ' ')
            {
                GameObject obj(16 * x, 16 * y);
                obj.anim.push_back(Image("resources/frames/hole.png"));
                obj.height = obj.anim[0].Height();
                obj.width = obj.anim[0].Width();
                obj.is_danger = true;
                this->Objects.push_back(obj);
            }
            else if (tile_data[y][x] == '#')
            {
                GameObject obj(16 * x, 16 * y, "resources/frames/wall_mid.png");
                obj.height = obj.Height();
                obj.width = obj.Width();
                obj.is_wall = true;
                this->Objects.push_back(obj);
            }
            else if (tile_data[y][x] == '.')
            {
                GameObject obj(16 * x, 16 * y, "resources/frames/floor_1.png");
                obj.height = obj.Height();
                obj.width = obj.Width();
                this->Objects.push_back(obj);
            }
            else if (tile_data[y][x] == 'x')
            {
                GameObject obj(16 * x, 16 * y, "resources/frames/doors_leaf_closed.png");
                obj.height = obj.Height();
                obj.width = obj.Width();
                obj.is_door = true;
                this->Objects.push_back(obj);
            }
            else if (tile_data[y][x] == 'T')
            {
                GameObject obj(16 * x, 16 * y);
                obj.anim = anima;
                obj.height = obj.anim[0].Height();
                obj.width = obj.anim[0].Width();
                obj.is_danger = false;
                obj.is_spike = true;
                this->Objects.push_back(obj);
            }
            else if (tile_data[y][x] == '$')
            {
                GameObject obj(16 * x, 16 * y);
                obj.height = obj.Height();
                obj.width = obj.Width();
                obj.is_door = true;
                this->Objects.push_back(obj); 
            }
            else if (tile_data[y][x] == '@')
            {
                GameObject obj(16 * x, 16 * y, "resources/frames/floor_1.png");
                this->Objects.push_back(obj);
                start->x = 16 * x;
                start->y = 16 * y;
            }
        }
    }
}

void Game::Render(GameMap &map, Image &screen)
{
    timer = 0;
    for (GameObject &obj : map.Objects)
    {
        obj.Draw_obj(screen);
    }
}

void Game::Update(GameMap &map, Image &screen, int i)
{
    for (GameObject &obj : map.Objects)
    {
        if (obj.is_spike)
            {
                if (i == 3)
                    obj.is_danger = true;
                else if (i != -1)
                    obj.is_danger = false;
                if (obj.glit < 5 && i == -1)
                    obj.glit++;
                else if (i == -1)
                {
                    obj.num_an = 3;
                    obj.Draw_obj(screen);
                    obj.Draw_obj(map.backup);
                    obj.glit = 3;
                }
                else
                    obj.num_an = i;
                obj.Draw_obj(screen);
                obj.Draw_obj(map.backup);
            }
    }
}