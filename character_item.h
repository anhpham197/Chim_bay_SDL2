#pragma once

#include "BaseObject.h"
#include "CommonFunction.h"

class character_item : public BaseObject
{
public:
	character_item();
	~character_item();

	enum ItemType
	{
		BIRD_TYPE = 50,
		BULLET_TYPE = 51,
	};

	void set_x_val(const int& xVal) { x_val_ = xVal; }
	void set_y_val(const int& yVal) { y_val_ = yVal; }
	int get_x_val() const { return x_val_; }
	int get_y_val() const { return y_val_; }

	void set_is_move(const bool& isMove) { is_move_ = isMove; }         
	bool get_is_move() const { return is_move_; }

	void HandleMove(const int& x_border, const int& y_border);       //  xử lí di chuyển ra khỏi màn hình cho item

	void HandleMove_Enemy(const int& x_border, const int& y_border);

	// Xử lí các loại đạn
	void set_item_type(const unsigned int& itemType) { item_type_ = itemType; }
	unsigned int get_item_type() const { return item_type_; }

	void LoadImgItem(SDL_Renderer* des);
private:
	int x_val_, y_val_;
	bool is_move_;      // kiểm tra trạng thái 
	unsigned int item_type_;

};

