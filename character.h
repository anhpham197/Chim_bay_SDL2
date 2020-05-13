
#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <vector>  // lưu trữ list item

#include "CommonFunction.h"
#include "BaseObject.h"
#include "character_item.h"

#define player_speed 32

class character : public BaseObject
{
public:
	character();
	~character();


	bool LoadImg(std::string path, SDL_Renderer* screen);     
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);     // ham xu li su kien
	//void set_clip();    // animation
	void Show(SDL_Renderer* des);

    int DoPlayer(Map& map_data);      // cong tru luong cho nvat di chuyen
	int CheckMap(Map& map_data);
	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }    // luu vi tri ban do
	void MapParameter(Map& map_data);    // tinh toan ra thong so cua ban do khi di chuyen nhan vat

	// Hàm xử lí item
	void set_item_list(std::vector <character_item*> item_list)
	{
		p_item_list = item_list;
	}
	std::vector <character_item*> get_item_list() const { return p_item_list; }

	void HandleItem(SDL_Renderer* des);

	void RemoveBullet(const int& index);   // index: chi so cua vien dan ( vien dan thu bao nhieu)
	
private:

	std::vector <character_item*> p_item_list;

	float x_val_, y_val_;       // lượng thay đổi sau mỗi lần ấn
	float x_pos_, y_pos_;       // vị trí hiện tại

	
	SDL_Rect frame_clip_;   // lưu ảnh

	Input input_type_;    // lưu trạng thái di chuyển

	float map_x_, map_y_;
};

#endif // !CHARACTER_H_

