#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<ctime>
#include<sstream>
#include<SDL_mixer.h>
using namespace std;

#ifndef BLOCK_H
#define BLOCK_H
#define block_time 500 // 方塊下落時間間隔
#define initial_coordinate_x 80 + buffer_distance // 左上角方塊的X座標
#define initial_coordinate_y 10 // 左上角方塊的Y座標
#define player_distance 600 // 兩個玩家方塊間的距離
#define block_side 29 // ˊ一個格子的邊長大小 調整這個數字可以改變螢幕大小
#define block_side_hold 10 // HOLD方塊邊長大小
#define bar_width 2 * buffer_distance // BAR寬度
#define buffer_distance 4 // HOLD 和 BLOCK 和 BAR 和 NEXT 在螢幕上的間隔 看起來比較不會擠在一起
#define Surface_height 20 * block_side + 2 * initial_coordinate_y // 螢幕高
#define Surface_width 10 * block_side + 2 * initial_coordinate_x + player_distance // 螢幕寬
#define button_side 80

class menu{
    public:
        void start_menu();
};

class block{
    public :
        block(int); //constructor
        block(int, int, int);
        block& changeorientation(int); // 轉變方塊方向
        block& movecenter(int); // 移動方塊中心位置
        friend void four_of_block_position(block&, int[][4]);// 給定方塊Class(其中包含其中心位置, 轉向, 型態) 傳回 四個方塊的X Y 座標
        friend void holdpresent(); // 會利用重心位置呈現
        friend void nextblockpresent(); // 會利用種新位置呈現
    private :
        int type;//型態
        int center_x, center_y;//方塊中心位置 可以參考資料夾中的 block definition 檔
        int orientation;//旋轉方向
};

class player{
    public :
        player(int, int);
        void showbackblock();
        friend void stuck(player&, bool&);
        friend void run_block(player&, player&, clock_t&);
        friend void showbackground(player&, player&);
        friend void blockfall(clock_t&, player&, player&);
        void showblock(); // 方塊呈現
        void giveblock();
        void holdpresent();
        void nextblockpresent();
        void breakblock();
        bool gamerunning;
        void resetblock();
        int nextblocks[6];
        void reset_ary(int);
        void destroy();
        void showshadow();
        void attack(player&);
        int score;
    private :
        int block_ary[21][10];// 地圖陣列
        int name;
        int record[7];
        int hold_type;
        bool shifttimes;
        block NOW;
        int B2B4;
        int combo;
        int garbage;
        bool running;
};

string get_form(string, int, string);
void LoadTextures(int);
void LoadMixs();
void LoadButtons();
SDL_Texture *LoadTexture(string);

void four_of_block_position(block&, int[][4]);

void showbackground(player&, player&);
void breakblock(player&, player&);
void stuck(player&, bool&);

void run_block(player&, player&, clock_t&);
//void run_your_block(player&, player&);
void blockfall(clock_t&, player&, player&);

extern SDL_Window *window;
extern SDL_Surface *windowsurface;
extern SDL_Texture *image_main_background;
extern SDL_Texture *image_background;
extern SDL_Texture *image_I;
extern SDL_Texture *image_J;
extern SDL_Texture *image_L;
extern SDL_Texture *image_O;
extern SDL_Texture *image_S;
extern SDL_Texture *image_T;
extern SDL_Texture *image_Z;
extern SDL_Texture *image_garbage;
extern SDL_Texture *image_hold_None;
extern SDL_Texture *image_shadow;
extern SDL_Texture* startTexture;
extern SDL_Texture* player1win;
extern SDL_Texture* player2win;
extern SDL_Texture* start;
extern SDL_Texture* rule;
extern SDL_Texture* map1;
extern SDL_Texture* map2;
extern SDL_Texture* map3;
extern SDL_Texture* back1;
extern SDL_Texture* back2;
extern SDL_Texture* map1_1;
extern SDL_Texture* map2_1;
extern SDL_Texture* map3_1;
extern SDL_Texture* back1_1;
extern SDL_Texture* back2_1;
extern SDL_Texture* rule_key;
extern SDL_Texture* rule_score;
extern SDL_Texture* music1;
extern SDL_Texture* music2;
extern SDL_Texture* music3;

extern SDL_Texture* rule_score;
extern SDL_Renderer *rendertarget;
extern int map_type;
extern int form_type;
extern Mix_Music *bgm1;
extern Mix_Music *bgm2;
extern Mix_Music *bgm3;
extern Mix_Chunk *effect1;
extern Mix_Chunk *effect2;
extern Mix_Chunk *effect3;
extern Mix_Chunk *effect4;
extern Mix_Chunk *effect5;
extern Mix_Chunk *effect6;
extern Mix_Chunk *effect7;

extern stringstream timeText;
extern bool ESCpress;
enum background_type{Original, Tetris, Lines};
enum mainbackground_type{Bright = 1, Dark};
enum block_type{I_t, J_t, L_t, O_t, S_t, T_t, Z_t, garbage_t};
extern int spin;
#endif
