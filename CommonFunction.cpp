#include "CommonFunction.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "TextObject.h"



int SDLCommonFunction::ShowMenu(SDL_Renderer* des, TTF_Font* font)
{
	BaseObject g_menu;
	bool ret = g_menu.LoadImg("Images/intro.png", des);
	if (ret == NULL)
	{
		return 1;
	}

	const int num_of_items = 2;            // số lượng phím (play game, exit)
	SDL_Rect pos_arr[num_of_items];
	pos_arr[0].x = 195;
	pos_arr[0].y = 550;
	pos_arr[1].x = 820;
	pos_arr[1].y = 550;

	TextObject text_menu[num_of_items];

	text_menu[0].SetText("_PLAY GAME_");
	text_menu[0].SetColor(TextObject::BLACK_TEXT);     // thiet lap mau ban dau
	text_menu[0].SetRect(pos_arr[0].x, pos_arr[0].y);

	text_menu[1].SetText("...EXIT...");
	text_menu[1].SetColor(TextObject::BLACK_TEXT);
	text_menu[1].SetRect(pos_arr[1].x, pos_arr[1].y);


	bool selected[num_of_items] = { 0,0 };        // luu trang thai chuot khi di chuyen, (co the bo di)

	int xm = 0, ym = 0;     // vi tri con chuot

	SDL_Event mouse_event;

	while (true)
	{
		for (int i = 0; i < num_of_items; i++)
		{
			text_menu[i].LoadFromRenderText(font, des);
		}

		while (SDL_PollEvent(&mouse_event))
		{
			switch (mouse_event.type)
			{
			case SDL_QUIT:
				return 1;
			case SDL_MOUSEMOTION:
			{
				xm = mouse_event.motion.x;
				ym = mouse_event.motion.y;

				for (int i = 0; i < num_of_items; i++)
				{
					if (CheckFocus(xm, ym, text_menu[i].GetRect()))
					{
						if (selected[i] == false)
						{
							//selected[i] = 1;
							text_menu[i].SetColor(TextObject::WHITE_TEXT);
						}
					}
					else
					{
						if (selected[i] == true)
						{
							//selected[i] = 0;
							text_menu[i].SetColor(TextObject::BLACK_TEXT);
						}
					}
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				xm = mouse_event.button.x;
				ym = mouse_event.button.y;

				for (int i = 0; i < num_of_items; i++)
				{
					if (CheckFocus(xm, ym, text_menu[i].GetRect()))
					{
						return i;
					}
				}
			}
			break;

			case SDL_KEYDOWN:
				if (mouse_event.key.keysym.sym == SDLK_ESCAPE)
				{
					return 1;
				}

			default:
				break;
			}
		}

		g_menu.Render(des);

		for (int i = 0; i < num_of_items; ++i)
		{
			text_menu[i].RenderText(des, pos_arr[i].x, pos_arr[i].y);
			pos_arr[i].w = text_menu[i].GetWidth();
			pos_arr[i].h = text_menu[i].GetHeight();
		}


		SDL_RenderPresent(des);          // update man hinh hien tai
	}

	return 1;             // exit
}

// SOURCE : PHATTRIENPHANMEM123AZ
bool SDLCommonFunction::CheckCollison(const SDL_Rect& object1, const SDL_Rect& object2)
{
	int left_a = object1.x;
	int right_a = object1.x + object1.w;
	int top_a = object1.y;
	int bottom_a = object1.y + object1.h;

	int left_b = object2.x;
	int right_b = object2.x + object2.w;
	int top_b = object2.y;
	int bottom_b = object2.y + object2.h;

	// Case 1: size object 1 < size object 2
	if (left_a > left_b && left_a < right_b)
	{
		if (top_a > top_b && top_a < bottom_b)
		{
			return true;
		}
	}

	if (left_a > left_b && left_a < right_b)
	{
		if (bottom_a > top_b && bottom_a < bottom_b)
		{
			return true;
		}
	}

	if (right_a > left_b && right_a < right_b)
	{
		if (top_a > top_b && top_a < bottom_b)
		{
			return true;
		}
	}

	if (right_a > left_b && right_a < right_b)
	{
		if (bottom_a > top_b && bottom_a < bottom_b)
		{
			return true;
		}
	}

	// Case 2: size object 1 > size object 2
	if (left_b > left_a && left_b < right_a)
	{
		if (top_b > top_a && top_b < bottom_a)
		{
			return true;
		}
	}

	if (left_b > left_a && left_b < right_a)
	{
		if (bottom_b > top_a && bottom_b < bottom_a)
		{
			return true;
		}
	}

	if (right_b > left_a && right_b < right_a)
	{
		if (top_b > top_a && top_b < bottom_a)
		{
			return true;
		}
	}

	if (right_b > left_a && right_b < right_a)
	{
		if (bottom_b > top_a && bottom_b < bottom_a)
		{
			return true;
		}
	}

	// Case 3: size object 1 = size object 2
	if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
	{
		return true;
	}

	return false;
}