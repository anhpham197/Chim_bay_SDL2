// Quản lí vấn đề hình ảnh

#pragma once

#include "CommonFunction.h"


static SDL_Renderer* g_menu_ = NULL;

class BaseObject
{
public:
	BaseObject();  //constructor
	~BaseObject(); //destructor
	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; }
	SDL_Rect GetRect() const { return rect_; }    // lấy ra kích thước ảnh
	SDL_Texture* GetObject() const { return p_object_; }

	bool LoadImg(std::string path, SDL_Renderer* screen);     // load ảnh bitmap lên p_object_ -> thêm vào màn hình
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);

	void Free();

	

protected:
	SDL_Texture* p_object_;    // chuyên lưu trữ hình ảnh
	SDL_Rect rect_;            // chuyên lưu trữ kích thước ảnh
};



