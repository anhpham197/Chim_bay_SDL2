#include "BaseObject.h"
#include "CommonFunction.h"
#include "character.h"
#include "GameMap.h"
#include "ImpTimer.h"
#include "TextObject.h"
#include "Enemy.h"
#include "Explosion.h"

#include <SDL.h>
#include <Windows.h>



BaseObject g_background;
TTF_Font* font_time;      // tao doi tuong font

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);   // thiết lập môi trường ban đầu cho SDL
    if (ret < 0) return false;
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");    // thiết lập chế độ tỉ lệ với chất lượng

    g_window = SDL_CreateWindow("CHIM_BAY - MSV:19021219", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                                           SCREEN_WIDTH, SCREEN_HEIGHT, 
                                                           SDL_WINDOW_SHOWN);   // tạo cửa sổ hiển thị

    if (g_window == NULL) success = false;
    else
    {
        g_screen = SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL) success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgflags = IMG_INIT_PNG;
            if (!(IMG_Init(imgflags) && imgflags)) 
                success = false;
        }
    }

    //AUDIO
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        printf("%s", Mix_GetError());
    }
    g_sound = Mix_LoadMUS("sound/audio.mp3");
    
    if (g_sound == NULL)
    {
        printf("%s", Mix_GetError());
    }

    // TEXT
    if (TTF_Init() == -1)
    {
        success = false;
    }
    font_time = TTF_OpenFont("Font//dlxfont_.ttf", 30);
    if (font_time == NULL)
    {
        printf ("&s",TTF_GetError());
    }

    return success;

    
}


bool LoadBackground()
{
    bool ret = g_background.LoadImg("Images//background3.jpg", g_screen);
    if (ret == false)
        return false;
    return true;
}

void close()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}



int main(int argc, char* argv[])
{
    ImpTimer  fps_time;


    if (InitData() == false)
        return -1;
    if (LoadBackground() == false)
        return -1;


    // INITIALIZE GAME_MAP
    GameMap tube;
    tube.LoadMap("tube/map.txt");
    tube.LoadTiles(g_screen);  


    // INITIALIZE MAIN CHARACTER
    character p_player;
    p_player.LoadImg("Images//bird.png", g_screen);


    // INITIALIZE ENEMY
    Enemy p_enemy;
    p_enemy.LoadImg("Images//enemy.png", g_screen);
    p_enemy.set_x_pos(1664);          
    p_enemy.set_y_pos(0);
    

    // INITIALIZE TEXTOBJECT
    TextObject time_game;                                             // game_time
    time_game.SetColor(TextObject::WHITE_TEXT);
    TextObject num_of_defeat;                                            // số lần bắn trúng đạn và enemy
    num_of_defeat.SetColor(TextObject::WHITE_TEXT);
    // SET UP SURVIVAL INDEX
    int num_of_death = 0;


    // INITIALIZE ITEM FOR ENEMY
    character_item* p_item = new character_item();         
    p_enemy.InitItem(p_item, g_screen);                    


    // INITIALIZE EXPLOSION
    Explosion exp;
    bool tRet = exp.LoadImg("Images//explosion.png", g_screen);
    if (!tRet) return -1;
    exp.set_clip();



    bool is_quit = false;
    

    // INITIALIZE MENU
    int ret_menu = SDLCommonFunction::ShowMenu(g_screen, font_time);
   
    if (ret_menu == 1)
    {
        is_quit = true;
    }


    while (!is_quit)
    {

        if (!Mix_PlayingMusic())
            Mix_PlayMusic(g_sound, 0);     // thay 0 = so am => so lan phat lai nhac la vo tan


        fps_time.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
                is_quit = true;
      
            p_player.HandleInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);

        
        Map map_data = tube.getMap();

        p_player.HandleItem(g_screen);
        p_player.SetMapXY(map_data.start_x, map_data.start_y);     // thiết lập lại bản đồ theo di chuyển của nvat
        int ret = p_player.DoPlayer(map_data);
        p_player.Show(g_screen);

        tube.SetMap(map_data);
        tube.DrawMap(g_screen);

       
        p_enemy.SetMapXY(map_data.start_x, map_data.start_y);
        p_enemy.DoPlayer(map_data);
        p_enemy.MakeItem(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
        p_enemy.Show(g_screen);


        // GET RECT OF EXPLOSION
        int frame_exp_width = exp.get_frame_width();
        int frame_exp_height = exp.get_frame_heigh();


        // CHECK COLLISION BETWEEN BULLET OF ENEMY AND MAIN CHARACTER        
        SDL_Rect pRect;          // player rect
        pRect.x = p_player.GetRect().x;
        pRect.y = p_player.GetRect().y;
        pRect.w = p_player.GetRect().w;
        pRect.h = p_player.GetRect().h;

        bool bCol1 = false;
        std::vector <character_item*> tBulletList = p_enemy.get_item_list();
        for (int j = 0; j < tBulletList.size(); j++) {
            character_item* pt_bullet = tBulletList[j];
            if (pt_bullet) {

                bCol1 = SDLCommonFunction::CheckCollison(pt_bullet->GetRect(), pRect);

                if (bCol1) {   

                    // EXPLOSION BETWEEN BULLET OF ENEMY AND MAIN CHARACTER
                   for (int ex = 0; ex < NUM_OF_FRAMES; ex++)
                    {
                        int x_pos = p_item->GetRect().x - frame_exp_width * 0.5;
                        int y_pos = p_item->GetRect().y - frame_exp_height * 0.5;

                        exp.set_frame(ex);
                        exp.SetRect(x_pos, y_pos);
                        exp.Show(g_screen);
                        SDL_RenderPresent(g_screen);
                    } 

                    p_enemy.RemoveBullet(j);
                    break;
                }
            }
        }

        SDL_Rect tRect;       // threat rect
        tRect.x = p_enemy.GetRect().x;
        tRect.y = p_enemy.GetRect().y;
        tRect.w = p_enemy.GetRect().w;
        tRect.h = p_enemy.GetRect().h;
        bool bCol2 = SDLCommonFunction::CheckCollison(pRect, tRect);     // collision between player and threat 

        if (bCol1 || bCol2) {

            // EXPLOSION BETWEEN ENEMY AND MAIN CHARACTER
            for (int ex = 0; ex < NUM_OF_FRAMES; ex++)
            {
                int x_pos = p_player.GetRect().x + p_player.GetRect().w - frame_exp_width * 0.5;
                int y_pos = p_player.GetRect().y + p_player.GetRect().h- frame_exp_height * 0.5;

                exp.set_frame(ex);
                exp.SetRect(x_pos, y_pos);
                exp.Show(g_screen);
                SDL_RenderPresent(g_screen);
            } 

             Mix_Chunk* beep_sound = Mix_LoadWAV("sound/game_over.wav");
             if (beep_sound != NULL)
                Mix_PlayChannel(-1, beep_sound, 0);
             MessageBox(NULL, L"Oops! You've been hit ! ! !", L"Info", MB_ICONERROR | MB_OK);
             is_quit = true;

        }



        // CHECK COLLISION BETWEEN BULLET OF MAIN CHARACTER AND ENEMY
        std::vector<character_item*> bullet_arr = p_player.get_item_list();
        for (int i = 0; i < bullet_arr.size(); i++)
        {
            character_item* p_bullet = bullet_arr[i];
            if (p_bullet != NULL) {
                SDL_Rect tRect;
                tRect.x = p_enemy.GetRect().x;
                tRect.y = p_enemy.GetRect().y;
                tRect.w = p_enemy.GetRect().w;
                tRect.h = p_enemy.GetRect().h;

                SDL_Rect bRect = p_bullet->GetRect();

                bool bCol = SDLCommonFunction::CheckCollison(bRect, tRect);
                if (bCol) {

                    //EXPLOSION BETWEEN BULLET OF MAIN CHARACTER AND ENEMY
                    for (int ex = 0; ex < NUM_OF_FRAMES; ex++)
                    {
                        int x_pos = p_enemy.GetRect().x - frame_exp_width * 0.5;
                        int y_pos = p_enemy.GetRect().y - frame_exp_height * 0.5;

                        exp.set_frame(ex);
                        exp.SetRect(x_pos, y_pos);
                        exp.Show(g_screen);
                        SDL_RenderPresent(g_screen);
                    }
                    

                    num_of_death++;      

                    if (num_of_death < 3) {
                        p_enemy.SetRect(1664, 0);
                    }
                    else {
                        for (int j = 0; j < tBulletList.size(); j++) {
                            p_enemy.RemoveBullet(j);
                        }
                        p_player.RemoveBullet(i);
                        p_enemy.Free();                      
                    }
                }
            }
        }



        // SHOW GAME STATUS
        if (ret == 1)
        {
            Mix_Chunk* beep_sound = Mix_LoadWAV("sound/game_over.wav");
            if (beep_sound != NULL)
                Mix_PlayChannel(-1, beep_sound, 0);
            MessageBox(NULL, L"GAME OVER !", L"Info", MB_ICONERROR | MB_OK);
            is_quit = true;
        }
        else if (ret == -1 && num_of_death >= 3)
        {
            Mix_Chunk* beep_sound = Mix_LoadWAV("sound/win_game.wav");
            if (beep_sound != NULL)
                Mix_PlayChannel(-1, beep_sound, 0);
            MessageBox(NULL, L"YOU'VE WIN THE GAME !", L"Info", MB_ICONASTERISK | MB_OK);
            is_quit = true;
        }
     

        // SHOW GAME TIME
        std::string str_time = "Time left : ";
        Uint32 time_val = SDL_GetTicks() / 1000;   // doi ve giay
        Uint32 val_time = 500 - time_val;          // dem nguoc;
        if (val_time <= 0)
        {
            Mix_Chunk* beep_sound = Mix_LoadWAV("sound/game_over.wav");
            if (beep_sound != NULL)
                Mix_PlayChannel(-1, beep_sound, 0);
            MessageBox(NULL, L"GAMEOVER !", L"Info", MB_ICONERROR | MB_OK);
            is_quit = true;
        }
        else
        {
            std::string str_val = std::to_string(val_time);
            str_time += str_val;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, 10, 20);
        }


        std::string num_of_Death = std::to_string(num_of_death);
        std::string survival("DEFEATED : ");
        survival += num_of_Death;
        num_of_defeat.SetText(survival);
        num_of_defeat.LoadFromRenderText(font_time, g_screen);
        num_of_defeat.RenderText(g_screen, 800, 20);

        SDL_RenderPresent(g_screen);


        // FPS
        int real_imp_time = fps_time.get_ticks();                       // thời gian thực sự đã trôi qua
        int time_of_one_frame = 1000 / frame_per_second;        // đơn vị : ms
        if (real_imp_time < time_of_one_frame)     // nếu thời gian chạy thực bé hơn (nhanh hơn so với cài đặt) -> phải tạo độ trễ
        {
            int delay_time = time_of_one_frame - real_imp_time;
            SDL_Delay(delay_time);      // nếu là dương thì delay_time tự truyền vào, nếu là âm thì delay_time tự convert về 0

            // delay_time càng lớn <=> frame_per_second (FPS) càng nhỏ => ctrinh càng chậm
            // tăng FPS để ctrinh chạy nhanh, MAX_FPS = 1000 / real_imp_time;
            // phải giảm real_time <=> máy có cấu hình lớn : thời gian thực hiện lệnh ít
        }
    }

    p_player.Free();
    close();
    SDL_Quit();
    return 0;
}


