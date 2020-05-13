#pragma once
#ifndef GAME_MAP_H

#define GAME_MAP_H
#define MAX_TILES 20

#include "CommonFunction.h"
#include "BaseObject.h"

class TileMap : public BaseObject
{
public:
    TileMap() { ; }
    ~TileMap() { ; }
};


class GameMap 
{
public:
    GameMap() { ; }
    ~GameMap() { ; }

    void LoadMap(const char* name);
    void LoadTiles(SDL_Renderer* screen);
    void DrawMap(SDL_Renderer* screen);
    Map getMap() const { return game_map; };    // do game_map la private nen phai dung getMap de lay thong tin
    void SetMap(Map& map_data) { game_map = map_data; };

private:

    Map game_map;   // chứa toàn bộ thông tin về bản đồ : trạng thái, vị trí, chỉ số
    TileMap tile_map[MAX_TILES];

};



#endif // GAME_MAP_H



