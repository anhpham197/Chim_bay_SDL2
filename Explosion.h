#pragma once

#include "CommonFunction.h"
#include "BaseObject.h"

#define NUM_OF_FRAMES 8

class Explosion : public BaseObject
{
public:
	Explosion();
	~Explosion();

	void set_clip();             // load từng frame ảnh vụ nổ
	void set_frame(const int& fr) { frame_ = fr; }
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);
	int get_frame_width() const { return frame_width_; }
	int get_frame_heigh() const { return frame_height_; }

private:
	int frame_width_;
	int frame_height_;

	unsigned int frame_;
	SDL_Rect frame_clip_[8];
};
