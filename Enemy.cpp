#include "Enemy.h"

Enemy::Enemy()
{
	x_val_ = 0; y_val_ = 0;
	x_pos_ = 0; y_pos_ = 0;
	y_val_ = 20;
	input_type_.down_ = 1;
	input_type_.up_ = 0;
}

Enemy::~Enemy() {}

bool Enemy::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		frame_clip_.w = rect_.w ;
		frame_clip_.h = rect_.h;
	}
	return ret;
}

void Enemy::Show(SDL_Renderer* des)
{
	rect_.x = x_pos_ - map_x_;      
	rect_.y = y_pos_ - map_y_;

	SDL_Rect renderquad = { rect_.x,rect_.y, frame_clip_.w, frame_clip_.h };
	SDL_RenderCopy(des, p_object_, &frame_clip_, &renderquad);
}

void Enemy::DoPlayer(Map& gMap)
{
	x_val_ = 0;
	ImpMove();
}

void Enemy::ImpMove()
{
	if (y_pos_ >= 600)
	{
		y_val_ = -(rand() % 11 + 20);
	}
	else if (y_pos_ <= 10)
	{
		y_val_ = rand() % 11 + 20;           // random trong khoảng [20;30] 
	}
	y_pos_ += y_val_;

}


void Enemy::RemoveBullet(const int& index)
{
	int size = item_list.size();
	if (size > 0 && index < size) {
		character_item* p_bullet = item_list[index];
		item_list.erase(item_list.begin() + index);

		if (p_bullet) {
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

void Enemy::CheckToMap(Map& gMap)
{
	int x1 = 0;   // gioi han kiem tra theo chieu x
	int x2 = 0;   // o thu bao nhieu 

	int y1 = 0;
	int y2 = 0;

	
	// Kiem tra theo chieu Oy
	int width_min = frame_clip_.w < TILE_SIZE ? frame_clip_.w : TILE_SIZE;
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;
	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + frame_clip_.h) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_val_ > 0)
		{
			y_pos_ = y2 * TILE_SIZE;
			y_pos_ -= frame_clip_.h;
			y_val_ = 0;
		}
		else if (y_val_ < 0)
		{
			y_pos_ = y1 * TILE_SIZE;
			y_val_ = 0;
		}
	}
		
	y_pos_ += y_val_;

	if (y_pos_ <= 0)
	{
		input_type_.down_ = 1;
		input_type_.up_ = 0;
	}
	if (y_pos_ + frame_clip_.h >= gMap.max_y)
	{
		input_type_.down_ = 0;
		input_type_.up_ = 1;
	}

}


// THIẾT LẬP BULLET CHO BOSS
void Enemy::InitItem(character_item* p_item, SDL_Renderer* screen)
{
	if (p_item != NULL)
	{
		p_item->LoadImg("Images//enemy_item.png", screen);
		p_item->set_is_move(true);
		p_item->SetRect(x_pos_ + 20, y_pos_ + 10);
		p_item->set_x_val(rand()% 20 + 150);
		item_list.push_back(p_item);
	}  
}

void Enemy::MakeItem(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
	for (int i = 0; i < item_list.size(); i++)
	{
		character_item* p_item = item_list[i];
		if (p_item != NULL)
		{
			if (p_item->get_is_move())
			{
				p_item->HandleMove_Enemy(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_item->Render(screen);
			} 
			else              // khi viên đạn đi quá giới hạn màn hình
			{
				p_item->set_is_move(true);
				p_item->SetRect(x_pos_ + 20, y_pos_ + 10);
			}
		}
	}
}  

