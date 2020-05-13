#include "BaseObject.h"
#include "CommonFunction.h"
#include "TextObject.h"

// Định nghĩa hàm constructor
BaseObject::BaseObject()
{
	p_object_ = NULL; // tránh lỗi
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;     // width
	rect_.h = 0;     // height

}

BaseObject::~BaseObject()
{
	Free();
}


bool SDLCommonFunction::CheckFocus(const int& x, const int& y, const SDL_Rect& rect)
{
	if (x >= rect.x && x < rect.x + rect.w &&
		y >= rect.y && y < rect.y + rect.h)
	{
		return true;
	}
	return false;
}

// Định nghĩa hàm LoadImg
bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)   
{
	SDL_Texture* new_texture = NULL;
	SDL_Surface* load_surface = IMG_Load(path.c_str());
	if (load_surface != NULL)   // nếu load không lỗi sẽ thực hiện transparent image
	{
		SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_B, COLOR_KEY_G));   // 167 175 180 
		new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
		if (new_texture != NULL)
		{
			rect_.w = load_surface->w;
			rect_.h = load_surface->h;  // khi đó toàn bộ thông số và thuộc tính ảnh nằm trong new_texture
		}
		SDL_FreeSurface(load_surface);  // do đã được chuyển sang new_texture
	}

	p_object_ = new_texture;    // đối tượng p_object_ sẽ gồm texture và rect

	if (p_object_ != NULL) return 1;
	else return 0;
}
 
void BaseObject::Render(SDL_Renderer* des,const SDL_Rect* clip)   // des: điểm đến của ảnh
{
	SDL_Rect renderquad = { rect_.x, rect_.y, rect_.w, rect_.h };  // x,y : vị trí ảnh     w,h: kích thước ảnh
	SDL_RenderCopy(des, p_object_, clip, &renderquad);  // đẩy thông số của renderquad lên p_object_
}

void BaseObject::Free()
{
	if (p_object_ != NULL)
	{
		SDL_DestroyTexture(p_object_);
		p_object_ = NULL;
		rect_.x = 0;
		rect_.y = 0;
	}
}
