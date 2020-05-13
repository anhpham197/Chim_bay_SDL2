#pragma once

#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "CommonFunction.h"
#include "BaseObject.h"

class TextObject : public BaseObject  // ke thua BaseObject
{
public:
	TextObject();
	~TextObject();

	enum TextColor
	{
		WHITE_TEXT = 0,
		BLACK_TEXT = 1,
	};

	bool LoadFromFile(std::string path);
	bool LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen);   // font chu
	void Free();             // giai phong texture
	
	void SetColor(Uint8 red , Uint8 yellow);       // truyen truc tiep gia tri mau
	void SetColor(int type);   // xet mau co ban ma NSD da thiet lap
    
	void RenderText(SDL_Renderer* screen, int xp, int yp, SDL_Rect* clip = NULL, double angle = 0.0,
		            SDL_Point* center = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);   

	int GetWidth() const{ return width_; }
	int GetHeight() const { return height_; }

	void SetText(const std::string& text) { content = text; }       
	std::string GetText() const { return content; }      // lay ra noi dung van ban
	

private:
	std::string content;              // luu noi dung hien thi text
	SDL_Color   text_color_;
	SDL_Texture* texture_;            // quan li text
	
	int width_, height_;
};

#endif // !THREATOBJECT_H_
