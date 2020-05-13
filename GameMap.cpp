#include "GameMap.h"
#include "CommonFunction.h"


void GameMap::LoadMap(const char* name)
{
	FILE* fp = NULL;
	fopen_s(&fp, name, "rb");
	if (fp == NULL) return;

	game_map.max_x = 0;
	game_map.max_y = 0;
	for (int i = 0; i < MAX_MAP_Y; i++)
	{
		for (int j = 0; j < MAX_MAP_X; j++)
		{
			fscanf_s(fp, "%d", &game_map.tile[i][j]);             // tile[][] : mảng 2 chiều lưu trữ trạng thái từng ô
			int val = game_map.tile[i][j];
			if (val > 0)
			{
				if (j > game_map.max_x)
					game_map.max_x = j;

				if (i > game_map.max_y)
					game_map.max_y = i;
			}
		}
	}

	game_map.max_x = (game_map.max_x + 1) * TILE_SIZE;
	game_map.max_y = (game_map.max_y + 1) * TILE_SIZE;

	game_map.start_x = 0;
	game_map.start_y = 0;
	game_map.file_name = name;

	fclose(fp);

}


void GameMap::LoadTiles(SDL_Renderer* screen)
{
	char file_img[MAX_TILES];
	FILE* fp = NULL;
	for (int i = 0; i < MAX_TILES; i++)
	{
		sprintf_s(file_img, "tube/%d.png", i);      // chuyển đổi file_img   %d ~ i
		fopen_s(&fp, file_img, "rb");     // rb : chế độ đọc
		if (fp == NULL)
			continue;
		fclose(fp);

		tile_map[i].LoadImg(file_img, screen);
	}
} 

void GameMap::DrawMap(SDL_Renderer* screen)
{
	int x1 = 0, x2 = 0;      // điểm đầu, cuối của chiều rộng màn hình

	x1 = (game_map.start_x % TILE_SIZE) * -1;
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);



	int y1 = 0, y2 = 0;      // điểm đầu, cuối của chiều cao màn hình

	y1 = (game_map.start_y % TILE_SIZE) * -1;
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);


	int map_x = 0, map_y = 0;           // chỉ sổ của ô 

	map_x = game_map.start_x / TILE_SIZE;
	map_y = game_map.start_y / TILE_SIZE;

	for (int i = y1; i < y2; i += TILE_SIZE)
	{
		map_x = game_map.start_x / TILE_SIZE;
		for (int j = x1; j < x2; j += TILE_SIZE)
		{
			int val = game_map.tile[map_y][map_x];
			if (val > 0)
			{
				tile_map[val].SetRect(j, i);
				tile_map[val].Render(screen);
			}
			map_x++;
		}

		map_y++;
	}
}
