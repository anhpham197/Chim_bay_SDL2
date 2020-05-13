#pragma once

#include "BaseObject.h"
#include "CommonFunction.h"
#include "character_item.h"
#include <vector>


class Enemy : public BaseObject
{
public:
	Enemy();
	~Enemy();


	void set_x_val(const float xVal) { x_val_ = xVal; }
	void set_y_val(const float yVal) { y_val_ = yVal; }

	void set_x_pos(const float& xp) { x_pos_ = xp; }
	void set_y_pos(const float& yp) { y_pos_ = yp; }
	float get_x_pos() const { return x_pos_; }
	float get_y_pos() const { return y_pos_; }

	void SetMapXY(const int& mp_x, const int& mp_y) { map_x_ = mp_x; map_y_ = mp_y; }
	
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);

	void DoPlayer(Map& gMap);
	void CheckToMap(Map& gMap);
	void ImpMove();

	
	void set_input_down(const int& ipDown) { input_type_.down_ = ipDown; } 

	std::vector<character_item*> get_item_list() const { return item_list; }
	void set_item_list(const std::vector<character_item*>& it_list) { item_list = it_list; }
	void InitItem(character_item* p_item, SDL_Renderer* screen);
	void MakeItem(SDL_Renderer* screen, const int& x_limit, const int& y_limit);


	void RemoveBullet(const int& index);   // index: chi so cua vien dan ( vien dan thu bao nhieu)

private:
	SDL_Rect frame_clip_;
	float x_pos_, y_pos_;
	float x_val_, y_val_;
	int map_x_, map_y_;

	Input input_type_;

	std::vector<character_item*> item_list;
};