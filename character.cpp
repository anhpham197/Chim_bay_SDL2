#include "character.h"


character::character()
{
	x_pos_ = 0;
	y_pos_ = SCREEN_HEIGHT/2;
	x_val_ = 0;
	y_val_ = 0;

	input_type_.right_ = 0;
	input_type_.down_ = 0;
	input_type_.up_ = 0;
	input_type_.left_ = 0;

	map_x_ = 0;
	map_y_ = 0;
}

character::~character(){}

bool character::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true)
	{
		frame_clip_.w = rect_.w;
		frame_clip_.h = rect_.h;
	} 

	return ret;
}



void character::Show(SDL_Renderer* des)
{
	//LoadImg("Images//bird.png", des);
	rect_.x = x_pos_ - map_x_;      // khi nvat chạy => màn hình bị cuốn theo (x_pos_ : tính từ điểm mốc ban đầu đến điểm hiện tại), 
	rect_.y = y_pos_ - map_y_;      // trong khi màn hình hiển thị từ map_x_

	SDL_Rect renderquad = { rect_.x,rect_.y, frame_clip_.w, frame_clip_.h };  
	SDL_RenderCopy(des, p_object_, &frame_clip_, &renderquad);
}


void character::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_DOWN:
		{
			input_type_.down_ = 1;
			input_type_.up_ = 0;
		}
		break;
		case SDLK_UP:
		{
			input_type_.up_ = 1;
			input_type_.down_ = 0;
		}
		break;
		case SDLK_LEFT:
		{
			input_type_.left_ = 1;
			input_type_.right_ = 0;
		}
		break;
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_UP:
		{
			input_type_.up_ = 0;
		}
		break;
		case SDLK_DOWN:
		{
			input_type_.down_ = 0;
		}
		break;
		case SDLK_LEFT:
		{
			input_type_.left_ = 0;
		}
		break;
		}
	}
	if (events.type == SDL_MOUSEBUTTONDOWN)
	{
		if (events.button.button == SDL_BUTTON_LEFT)
		{
			character_item* p_item = new character_item();
			p_item->set_item_type(character_item::BIRD_TYPE);
			p_item->LoadImgItem(screen);
			p_item->SetRect(this->rect_.x + frame_clip_.w - 20, rect_.y + frame_clip_.h * 0.3);

			p_item->set_x_val(player_speed * 2);
			p_item->set_is_move (true);

			p_item_list.push_back(p_item);

			Mix_Chunk* beep_sound = Mix_LoadWAV("sound/punch.wav");
			if (beep_sound != NULL)
				Mix_PlayChannel(-1, beep_sound, 0);
		}
		if (events.button.button == SDL_BUTTON_RIGHT)
		{
			character_item* p_item = new character_item();
			p_item->set_item_type(character_item::BULLET_TYPE);
			p_item->LoadImgItem(screen);
			p_item->SetRect(this->rect_.x + frame_clip_.w - 20, rect_.y + frame_clip_.h * 0.3);

			p_item->set_x_val(player_speed * 2);
			p_item->set_is_move(true);

			p_item_list.push_back(p_item);

			Mix_Chunk* beep_sound = Mix_LoadWAV("sound/punch.wav");
			if (beep_sound != NULL)
				Mix_PlayChannel(-1, beep_sound, 0);
		}
	}
}

// Xử lí đạn bắn
void character::HandleItem(SDL_Renderer* des)
{
	for (int i = 0; i < p_item_list.size(); i++)
	{
		character_item* p_item = p_item_list[i];
		if (p_item != NULL)
		{
			if (p_item->get_is_move() == true )
			{
				p_item->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_item->Render(des);
			}
			else
			{
				p_item_list.erase(p_item_list.begin() + i);
				if (p_item!=NULL)
				{
					delete p_item;
					p_item = NULL;
				}
			}
		}
	}
}



void character::RemoveBullet(const int& index)
{
	int size = p_item_list.size();
	if (size > 0 && index < size) {
		character_item* p_bullet = p_item_list[index];
		p_item_list.erase(p_item_list.begin() + index);

		if (p_bullet) {
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}



int character::DoPlayer(Map& map_data)
{
	int ret = 0;
	x_val_ = 0;
	y_val_ = player_speed/2.5;     // lien tuc cong len luong player_speed/3 (toc do roi) 
	

	if (input_type_.up_ == 1)
	{
		x_val_ += player_speed;
		y_val_ -= player_speed;
	}
	if (input_type_.down_ == 1)
	{
		//x_val_ += player_speed;
		y_val_ += player_speed/2;
	}
	if (input_type_.left_ == 1)
	{
		x_val_ -= player_speed/2;
		//y_val_ += player_speed;
	}

	ret = CheckMap(map_data);
	MapParameter(map_data);

	return ret;
}



void character::MapParameter(Map& map_data)
{
	map_data.start_x = x_pos_ - TILE_SIZE;    // khi nvat di chuyen den 1 nua ban do thi ban do bat dau cuon theo (SCREEN_WIDTH / 2)
	if (map_data.start_x < 0)
	{
		map_data.start_x = 0;
	}
	else if (map_data.start_x + SCREEN_WIDTH >= map_data.max_x)     // max.x = 64*334
	{
		map_data.start_x = map_data.max_x - SCREEN_WIDTH;
	}

	/*map_data.start_y = y_pos_ - (SCREEN_HEIGHT / 2);
	if (map_data.start_y < 0)
	{
		map_data.start_y = 0;
	}
	else if (map_data.start_y + SCREEN_HEIGHT >= map_data.max_y)
	{
		map_data.start_y = map_data.max_y - SCREEN_HEIGHT;
	}*/
}

int character::CheckMap(Map& map_data)
{
	int bRet = 0;
	int x1 = 0;   // gioi han kiem tra theo chieu x
	int x2 = 0;   // o thu bao nhieu 

    int y1 = 0;
	int y2 = 0;

	// Kiem tra theo chieu Ox
	int height_min = frame_clip_.h < TILE_SIZE ? frame_clip_.h : TILE_SIZE;
	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + frame_clip_.w - 5) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 10) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)      // nvat nam trong MAP
	{
		if (x_val_ > 0)  // nvat di chuyen sang phai
		{
			if ((map_data.tile[y1][x2] != 0 && map_data.tile[y1][x2] != 13) ||
				(map_data.tile[y2][x2] != 0 && map_data.tile[y2][x2] != 13) ||
				(map_data.tile[y1][x1] != 0 && map_data.tile[y1][x1] != 13) ||
				(map_data.tile[y2][x1] != 0 && map_data.tile[y2][x1] != 13))
			{
				//x_pos_ = x2 * TILE_SIZE;
				//x_pos_ -= frame_clip_.w;
				//x_val_ = 0;
				bRet = 1;
				return bRet;
			}
			else if (map_data.tile[y1][x2] == 13 || map_data.tile[y2][x2] == 13 ||
				map_data.tile[y1][x1] == 13 || map_data.tile[y2][x1] == 13)
			{
				bRet = -1;
				return bRet;
			}
		}



			                                                                              //  x1,y1.........x2,y1
		                                                                                  //   .              .
		/*
		else if (x_val_ < 0)                                                              //   .              .  
		{                                                                                 //  x1,y2.........x2,y2
			if ((map_data.tile[y1][x1] != 0 && map_data.tile[y1][x1] != 13 )|| 
				(map_data.tile[y2][x1] != 0 && map_data.tile[y2][x1] != 13))
				
			{
				//x_pos_ = x1 * TILE_SIZE;
				//x_val_ = 0;
				bRet = 1;
			}
			else if (map_data.tile[y1][x1] == 13 || map_data.tile[y2][x1] == 13)
			{
				bRet = -1;
			}
		}
		*/
	}


	// Kiem tra theo chieu Oy
	int width_min = frame_clip_.w < TILE_SIZE ? frame_clip_.w : TILE_SIZE;
	x1 = (x_pos_)/ TILE_SIZE;
	x2 = (x_pos_ + width_min - 5) / TILE_SIZE;
	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + frame_clip_.h - 10) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_val_ > 0)
		{
			if ((map_data.tile[y2][x1] != 0 && map_data.tile[y2][x1] != 13 ) || 
				(map_data.tile[y2][x2] != 0 && map_data.tile[y2][x2] != 13))
			{
				//y_pos_ = y2 * TILE_SIZE;
				//y_pos_ -= frame_clip_.h;
				//y_val_ = 0;
				bRet = 1;
				return bRet;
			}
			if (map_data.tile[y2][x1] == 13 || map_data.tile[y2][x2] == 13)
			{
				bRet = -1;
				return bRet;
		    }
		}
		else if (y_val_ < 0)
		{
			if ((map_data.tile[y1][x1] != 0 && map_data.tile[y1][x1] != 13) || 
				(map_data.tile[y1][x2] != 0 && map_data.tile[y1][x2] != 13))
			{
				//y_pos_ = y1 * TILE_SIZE;
				//y_val_ = 0;
				bRet = 1;
				return bRet;
			}

			else if (map_data.tile[y1][x2] == 13 || map_data.tile[y1][x1] == 13)
			{
				bRet = -1;
				return bRet;
			}
		}
	}

	if (bRet == 0)
	{
		x_pos_ += x_val_;
		y_pos_ += y_val_;
	}

	if (x_pos_ < 0 || x_pos_ + frame_clip_.w > map_data.max_x 
		|| y_pos_ < 0 || y_pos_ + frame_clip_.h > map_data.max_y)
	{
		bRet = 1;
	}

	return bRet;
}
		

	






