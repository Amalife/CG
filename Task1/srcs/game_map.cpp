#include "global.h"
#include "game_map.h"

void GameMap::loadFile(const char *path, Point *start)
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
        this->initMap(tile_data, start);
    }
}

void GameMap::initMap(std::vector<std::vector<char>> tile_data, Point *start)
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
                GameObject obj(16 * x, 16 * y, "resources/frames/hole.png");
                obj.is_danger = true;
                this->Objects.push_back(obj);
            }
            else if (tile_data[y][x] == '#')
            {
                GameObject obj(16 * x, 16 * y, "resources/frames/wall_mid.png");
                obj.is_wall = true;
                this->Objects.push_back(obj);
            }
            else if (tile_data[y][x] == '.')
            {
                GameObject obj(16 * x, 16 * y, "resources/frames/floor_1.png");
                this->Objects.push_back(obj);
            }
            else if (tile_data[y][x] == 'x')
            {
                GameObject obj(16 * x, 16 * y, "resources/frames/doors_leaf_closed.png");
                obj.is_door = true;
                this->Objects.push_back(obj);
            }
            else if (tile_data[y][x] == 'T')
            {
                GameObject obj(16 * x, 16 * y, "resources/frames/floor_spikes_anim_f3.png");
                obj.is_danger = true;
                this->Objects.push_back(obj);
            }
            else if (tile_data[y][x] == '$')
            {
                GameObject obj(16 * x, 16 * y);
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
    for (GameObject &obj : map.Objects)
        obj.Draw_obj(screen);
}