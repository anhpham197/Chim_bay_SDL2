#pragma once

#include <Windows.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h> // âm thanh
#include <SDL_ttf.h>   // text

// Định nghĩa biến
static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Renderer* g_intro = NULL;
static SDL_Event g_event;


// Audio
static Mix_Music* g_sound = NULL;
static Mix_Music* g_sound_menu = NULL;

// Định nghĩa tham số fps : chất lượng của đồ họa
const int frame_per_second = 15;      // số frame trong 1s


// Định nghĩa thông số môi trường
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 704;
const int SCREEN_BPP = 32; // chỉ số pixels

// Định nghĩa thông số màu sắc
const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;
const int RENDER_DRAW_COLOR = 255;


// Xu li trang thai di chuyen cua nhan vat
typedef struct Input
{
	int right_;
	int up_;
	int down_;
	int left_;

};

// TILE MAP
#define TILE_SIZE 64      // kích thước mắt lưới
#define MAX_MAP_X 28
#define MAX_MAP_Y 11
 
typedef struct Map
{
	int start_x;
	int start_y;

	// chỉ số của ô
	int max_x;
	int max_y;

	int tile[MAX_MAP_Y][MAX_MAP_X];

	const char* file_name;

}; 

namespace SDLCommonFunction
{
	int ShowMenu(SDL_Renderer* des, TTF_Font* font);
	bool CheckFocus(const int& x, const int& y, const SDL_Rect& rect);    // kiểm tra chuột di chuyển
	bool CheckCollison(const SDL_Rect& object1 , const SDL_Rect& object2);    // kiểm tra va chạm
}




