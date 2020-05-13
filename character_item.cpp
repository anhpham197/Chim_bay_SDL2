#include "character_item.h"

character_item::character_item()
{
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
	item_type_ = BIRD_TYPE;
}

character_item::~character_item() {}
void character_item::LoadImgItem(SDL_Renderer* des)
{
	if (item_type_ == BULLET_TYPE)
	{
		LoadImg("Images//bird_item2.png", des);
	}
	else 
	{
		LoadImg("Images//bird_item.png", des);
	}
}
void character_item::HandleMove(const int& x_border, const int& y_border)
{
	rect_.x += x_val_;
	if (rect_.x > x_border)
		is_move_ = false;
}

void character_item::HandleMove_Enemy(const int& x_border, const int& y_border)
{
	rect_.x -= x_val_;
	if (rect_.x < 0)
		is_move_ = false;
}