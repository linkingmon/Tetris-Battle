#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<ctime>
#include<sstream>
#include<SDL_mixer.h>
using namespace std;

#ifndef BLOCK_H
#define BLOCK_H
#define block_time 500 // ����U���ɶ����j
#define initial_coordinate_x 80 + buffer_distance // ���W�������X�y��
#define initial_coordinate_y 10 // ���W�������Y�y��
#define player_distance 600 // ��Ӫ��a��������Z��
#define block_side 29 // ���@�Ӯ�l������j�p �վ�o�ӼƦr�i�H���ܿù��j�p
#define block_side_hold 10 // HOLD�������j�p
#define bar_width 2 * buffer_distance // BAR�e��
#define buffer_distance 4 // HOLD �M BLOCK �M BAR �M NEXT �b�ù��W�����j �ݰ_�Ӥ�����|���b�@�_
#define Surface_height 20 * block_side + 2 * initial_coordinate_y // �ù���
#define Surface_width 10 * block_side + 2 * initial_coordinate_x + player_distance // �ù��e
#define button_side 80

class menu{
    public:
        void start_menu();
};

class block{
    public :
        block(int); //constructor
        block(int, int, int);
        block& changeorientation(int); // ���ܤ����V
        block& movecenter(int); // ���ʤ�����ߦ�m
        friend void four_of_block_position(block&, int[][4]);// ���w���Class(�䤤�]�t�䤤�ߦ�m, ��V, ���A) �Ǧ^ �|�Ӥ����X Y �y��
        friend void holdpresent(); // �|�Q�έ��ߦ�m�e�{
        friend void nextblockpresent(); // �|�Q�κطs��m�e�{
    private :
        int type;//���A
        int center_x, center_y;//������ߦ�m �i�H�ѦҸ�Ƨ����� block definition ��
        int orientation;//�����V
};

class player{
    public :
        player(int, int);
        void showbackblock();
        friend void stuck(player&, bool&);
        friend void run_block(player&, player&, clock_t&);
        friend void showbackground(player&, player&);
        friend void blockfall(clock_t&, player&, player&);
        void showblock(); // ����e�{
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
        int block_ary[21][10];// �a�ϰ}�C
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
