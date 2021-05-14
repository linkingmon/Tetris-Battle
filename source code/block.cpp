#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include"block.h"
#include"timer.h"
#include<sstream>
#include<SDL_mixer.h>
using namespace std;
SDL_Window *window = NULL;
SDL_Surface *windowsurface = NULL;
SDL_Texture *image_main_background = NULL;
SDL_Texture *image_background = NULL;
SDL_Texture *image_I = NULL;
SDL_Texture *image_J = NULL;
SDL_Texture *image_L = NULL;
SDL_Texture *image_O = NULL;
SDL_Texture *image_S = NULL;
SDL_Texture *image_T = NULL;
SDL_Texture *image_Z = NULL;
SDL_Texture *image_garbage = NULL;
SDL_Texture *image_hold_None = NULL;
SDL_Texture *image_shadow = NULL;
SDL_Renderer *rendertarget = NULL;
SDL_Texture* startTexture  = NULL;
SDL_Texture* player1win  = NULL;
SDL_Texture* player2win = NULL;
SDL_Texture* start  = NULL;
SDL_Texture* rule  = NULL;
SDL_Texture* map1  = NULL;
SDL_Texture* map2  = NULL;
SDL_Texture* map3  = NULL;
SDL_Texture* back1  = NULL;
SDL_Texture* back2  = NULL;
SDL_Texture* map1_1  = NULL;
SDL_Texture* map2_1  = NULL;
SDL_Texture* map3_1  = NULL;
SDL_Texture* back1_1  = NULL;
SDL_Texture* back2_1  = NULL;
SDL_Texture* rule_key = NULL;
SDL_Texture* rule_score = NULL;
SDL_Texture* music1 = NULL;
SDL_Texture* music2 = NULL;
SDL_Texture* music3 = NULL;
Mix_Music *bgm1 = NULL;
Mix_Music *bgm2 = NULL;
Mix_Music *bgm3 = NULL;
Mix_Chunk *effect1 = NULL;
Mix_Chunk *effect2 = NULL;
Mix_Chunk *effect3 = NULL;
Mix_Chunk *effect4 = NULL;
Mix_Chunk *effect5 = NULL;
Mix_Chunk *effect6 = NULL;
Mix_Chunk *effect7 = NULL;

enum direction{LEFT, DOWN, RIGHT, UP};
bool gamerunning = true;
stringstream timeText;
LTexture gTimeTextTexture;
LTimer timer;
SDL_Color textColor ;
int map_type = Original;
int form_type = Dark;
bool ESCpress = false;
int spin;

string get_form(string filePath, int form, string image_type){// load different forms
    string temp = filePath + char(form + '0') + image_type;
    return temp;
}

SDL_Texture *LoadTexture(string filePath){//load images
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    texture = SDL_CreateTextureFromSurface(rendertarget, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void LoadTextures(int form){
    image_main_background = LoadTexture(get_form("image/background", form, ".jpg"));
    image_background = LoadTexture(get_form("image/block", form, ".png"));
    image_I = LoadTexture(get_form("image/block_I", form, ".png"));
    image_J = LoadTexture(get_form("image/block_J", form, ".png"));
    image_L = LoadTexture(get_form("image/block_L", form, ".png"));
    image_O = LoadTexture(get_form("image/block_O", form, ".png"));
    image_S = LoadTexture(get_form("image/block_S", form, ".png"));
    image_T = LoadTexture(get_form("image/block_T", form, ".png"));
    image_Z = LoadTexture(get_form("image/block_Z", form, ".png"));
    image_garbage = LoadTexture(get_form("image/garbage", form, ".png"));
    image_hold_None = LoadTexture("image/None.png");
    image_shadow = LoadTexture("image/shadow.png");
}

block::block(int type_temp){//constructor
    type = type_temp;
    orientation = 0;
    center_x = 4;
    center_y = 1;
}

block::block(int type_temp, int x, int y){
    type = type_temp;
    orientation = 0;
    center_x = x;
    center_y = y;

}

player::player(int type_temp, int name_temp) : NOW(0){// set maps
    int type = type_temp;
    name = name_temp;
    for(int i = 0 ; i < 7 ; i++)
        record[i] = 0;
    hold_type = -1;
    for(int i = 0 ; i < 6 ; i++)
        nextblocks[i] = -1;
    gamerunning = true;
    score = 0;
    B2B4 =-1;
    combo=-1;
    /*operator
    cout << "Player" << name_temp << "Please input form" << endl;
    cin >> type;
    */
    switch(type){
        case Original :
            for(int i = 0 ; i < 20 ; i++)
                for(int j = 0 ; j < 10 ; j++)
                    block_ary[i][j] = -1;
            for(int i = 0 ; i < 10 ; i++)
                block_ary[20][i] = 0;
            break;
        case Tetris :
            for(int i = 0 ; i < 20 ; i++)
                for(int j = 0 ; j < 10 ; j++)
                    block_ary[i][j] = -1;
            block_ary[14][0] = block_ary[14][1] = block_ary[14][2] = block_ary[15][1] = block_ary[16][1] = 0;
            block_ary[15][2] = block_ary[15][3] = block_ary[16][2] = block_ary[17][2] = block_ary[17][3] = block_ary[18][2] = block_ary[19][2] = block_ary[19][3] = 1;
            block_ary[14][3] = block_ary[14][4] = block_ary[14][5] = block_ary[15][4] = block_ary[16][4] = 2;
            block_ary[16][4] = block_ary[16][5] = block_ary[16][6] = block_ary[17][4] = block_ary[17][6] = block_ary[18][4] = block_ary[18][5] = block_ary[19][4] = block_ary[19][6] = 3;
            block_ary[15][7] = block_ary[16][7] = block_ary[17][7] = block_ary[18][7] = 4;
            block_ary[15][8] = block_ary[15][9] = block_ary[16][8] = block_ary[17][8] = block_ary[17][9] = block_ary[18][9] = block_ary[19][8] = block_ary[19][9] = 5;
            for(int i = 0 ; i < 10 ; i++)
                block_ary[20][i] = 0;
            break;
        case Lines :
            for(int i = 0 ; i < 20 ; i++)
                for(int j = 0 ; j < 10 ; j++)
                    block_ary[i][j] = -1;
            for(int i = 13 ; i < 20 ; i++)
                for(int j = 0 ; j < 10 ; j++){
                    if((i + j) % 7 == 0)
                        block_ary[i][j] = 2;
                    if((i + j) % 7 == 2)
                        block_ary[i][j] = 5;
                    if((i + j) % 7 == 4)
                        block_ary[i][j] = 6;
                }
            for(int i = 0 ; i < 10 ; i++)
                block_ary[20][i] = 0;
            break;
    }
}

void player::showblock(){
    int position[2][4];
    SDL_Rect Rect_block;
    Rect_block.h = Rect_block.w = block_side;
    four_of_block_position(NOW, position);
    for(int j = 0 ; j < 4 ; j++){
        Rect_block.x = position[0][j] * block_side + initial_coordinate_x + player_distance * name; // + player
        Rect_block.y = position[1][j] * block_side + initial_coordinate_y;
        switch(NOW.type){
            case I_t :
                SDL_RenderCopy(rendertarget, image_I, NULL, &Rect_block);
                break ;
            case J_t :
                SDL_RenderCopy(rendertarget, image_J, NULL, &Rect_block);
                break ;
            case L_t :
                SDL_RenderCopy(rendertarget, image_L, NULL, &Rect_block);
                break ;
            case O_t :
                SDL_RenderCopy(rendertarget, image_O, NULL, &Rect_block);
                break ;
            case S_t :
                SDL_RenderCopy(rendertarget, image_S, NULL, &Rect_block);
                break ;
            case T_t :
                SDL_RenderCopy(rendertarget, image_T, NULL, &Rect_block);
                break ;
            case Z_t :
                SDL_RenderCopy(rendertarget, image_Z, NULL, &Rect_block);
                break;
        }
    }
    SDL_RenderPresent(rendertarget);
}

void breakblock(player& P1, player& P2){//also clear the screen
    SDL_RenderClear(rendertarget);
    showbackground(P1, P2);
}

block& block::changeorientation(int sign){//sign = 1 clockwise
    orientation += sign;
    if(orientation < 0)
        orientation += 4;
    orientation %= 4;
    return *this;
}

block& block::movecenter(int direction){
    if(direction == LEFT)
        center_x--;
    else if(direction == RIGHT)
        center_x++;
    else if(direction == DOWN)
        center_y++;
    else
        center_y--;
    return *this;
}

void four_of_block_position(block& X, int position_temp[][4]){// give center and orientation and get four position of the block
    switch(X.type){
        case I_t :
            switch(X.orientation){
                case 0 :
                    position_temp[0][0] = X.center_x - 1; position_temp[1][0] = X.center_y;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x + 1; position_temp[1][2] = X.center_y ;
                    position_temp[0][3] = X.center_x + 2; position_temp[1][3] = X.center_y;
                    break;
                case 1 :
                    position_temp[0][0] = X.center_x + 1; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x + 1; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x + 1; position_temp[1][2] = X.center_y + 1;
                    position_temp[0][3] = X.center_x + 1; position_temp[1][3] = X.center_y + 2;
                    break;
                case 2 :
                    position_temp[0][0] = X.center_x - 1; position_temp[1][0] = X.center_y + 1;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y + 1;
                    position_temp[0][2] = X.center_x + 1; position_temp[1][2] = X.center_y + 1;
                    position_temp[0][3] = X.center_x + 2; position_temp[1][3] = X.center_y + 1;
                    break;
                case 3 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y + 1;
                    position_temp[0][3] = X.center_x; position_temp[1][3] = X.center_y + 2;
                    break;
            }
            break;
        case J_t :
            switch(X.orientation){
                case 0 :
                    position_temp[0][0] = X.center_x - 1; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x - 1; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x + 1; position_temp[1][3] = X.center_y;
                    break;
                case 1 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x + 1; position_temp[1][1] = X.center_y - 1;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x; position_temp[1][3] = X.center_y + 1;
                    break;
                case 2 :
                    position_temp[0][0] = X.center_x - 1; position_temp[1][0] = X.center_y;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x + 1; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x + 1; position_temp[1][3] = X.center_y + 1;
                    break;
                case 3 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y + 1;
                    position_temp[0][3] = X.center_x - 1; position_temp[1][3] = X.center_y + 1;
                    break;
            }
            break;
        case L_t :
            switch(X.orientation){
                case 0 :
                    position_temp[0][0] = X.center_x + 1; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x - 1; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x + 1; position_temp[1][3] = X.center_y;
                    break;
                case 1 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y + 1;
                    position_temp[0][3] = X.center_x + 1; position_temp[1][3] = X.center_y + 1;
                    break;
                case 2 :
                    position_temp[0][0] = X.center_x - 1; position_temp[1][0] = X.center_y;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x + 1; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x - 1; position_temp[1][3] = X.center_y + 1;
                    break;
                case 3 :
                    position_temp[0][0] = X.center_x - 1; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y - 1;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x; position_temp[1][3] = X.center_y + 1;
                    break;
            }
            break;
        case O_t :
            switch(X.orientation){
                default :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x + 1; position_temp[1][1] = X.center_y - 1;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x + 1; position_temp[1][3] = X.center_y;
                    break;
            }
            break;
        case S_t :
            switch(X.orientation){
                case 0 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x + 1; position_temp[1][1] = X.center_y - 1;
                    position_temp[0][2] = X.center_x - 1; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x; position_temp[1][3] = X.center_y;
                    break;
                case 1 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x + 1; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x + 1; position_temp[1][3] = X.center_y + 1;
                    break;
                case 2 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y;
                    position_temp[0][1] = X.center_x + 1; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x - 1; position_temp[1][2] = X.center_y + 1;
                    position_temp[0][3] = X.center_x; position_temp[1][3] = X.center_y + 1;
                    break;
                case 3 :
                    position_temp[0][0] = X.center_x - 1; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x - 1; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x; position_temp[1][3] = X.center_y + 1;
                    break;
            }
            break;
        case T_t :
            switch(X.orientation){
                case 0 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x - 1; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x + 1; position_temp[1][3] = X.center_y;
                    break;
                case 1 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x + 1; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x; position_temp[1][3] = X.center_y + 1;
                    break;
                case 2 :
                    position_temp[0][0] = X.center_x - 1; position_temp[1][0] = X.center_y;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x + 1; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x; position_temp[1][3] = X.center_y + 1;
                    break;
                case 3 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x - 1; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x; position_temp[1][3] = X.center_y + 1;
                    break;
            }
            break;
        case Z_t :
            switch(X.orientation){
                case 0 :
                    position_temp[0][0] = X.center_x - 1; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y - 1;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x + 1; position_temp[1][3] = X.center_y;
                    break;
                case 1 :
                    position_temp[0][0] = X.center_x + 1; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x + 1; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x; position_temp[1][3] = X.center_y + 1;
                    break;
                case 2 :
                    position_temp[0][0] = X.center_x - 1; position_temp[1][0] = X.center_y;
                    position_temp[0][1] = X.center_x; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y + 1;
                    position_temp[0][3] = X.center_x + 1; position_temp[1][3] = X.center_y + 1;
                    break;
                case 3 :
                    position_temp[0][0] = X.center_x; position_temp[1][0] = X.center_y - 1;
                    position_temp[0][1] = X.center_x - 1; position_temp[1][1] = X.center_y;
                    position_temp[0][2] = X.center_x; position_temp[1][2] = X.center_y;
                    position_temp[0][3] = X.center_x - 1; position_temp[1][3] = X.center_y + 1;
                    break;
            }
            break;
    }

}

void player::holdpresent(){
    block I(I_t, 0, 0), J(J_t, 0, 0), L(L_t, 0, 0), O(O_t, 0, 0), S(S_t, 0, 0), T(T_t, 0, 0), Z(Z_t, 0, 0);
    SDL_Rect Rect_hold;
    Rect_hold.x = initial_coordinate_x - 2 * block_side - buffer_distance + player_distance * name;
    Rect_hold.y = initial_coordinate_y;
    Rect_hold.h = Rect_hold.w =  2 * block_side;
    SDL_RenderCopy(rendertarget, image_hold_None, NULL, &Rect_hold);
    int position[2][4];
    switch(hold_type){
        case I_t :
            four_of_block_position(I, position);
            for(int i = 0 ; i < 4 ; i++){
                Rect_hold.x = initial_coordinate_x - block_side - buffer_distance - block_side_hold + position[0][i] * block_side_hold + player_distance * name;
                Rect_hold.y = initial_coordinate_y + block_side - block_side_hold / 2 + position[1][i] * block_side_hold;
                Rect_hold.h = Rect_hold.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_I, NULL, &Rect_hold);
            }
            break;
        case J_t :
            four_of_block_position(J, position);
            for(int i = 0 ; i < 4 ; i++){
                Rect_hold.x = initial_coordinate_x - block_side - buffer_distance - block_side_hold / 2 + position[0][i] * block_side_hold + player_distance * name;
                Rect_hold.y = initial_coordinate_y + block_side + position[1][i] * block_side_hold;
                Rect_hold.h = Rect_hold.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_J, NULL, &Rect_hold);
            }
            break;
        case L_t :
            four_of_block_position(L, position);
            for(int i = 0 ; i < 4 ; i++){
                Rect_hold.x = initial_coordinate_x - block_side - buffer_distance - block_side_hold / 2 + position[0][i] * block_side_hold + player_distance * name;
                Rect_hold.y = initial_coordinate_y + block_side + position[1][i] * block_side_hold;
                Rect_hold.h = Rect_hold.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_L, NULL, &Rect_hold);
            }
            break;
        case O_t :
            four_of_block_position(O, position);
            for(int i = 0 ; i < 4 ; i++){
                Rect_hold.x = initial_coordinate_x - 2 * block_side - buffer_distance + block_side - block_side_hold + position[0][i] * block_side_hold + player_distance * name;
                Rect_hold.y = initial_coordinate_y + block_side + position[1][i] * block_side_hold;
                Rect_hold.h = Rect_hold.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_O, NULL, &Rect_hold);
            }
            break;
        case S_t :
            four_of_block_position(S, position);
            for(int i = 0 ; i < 4 ; i++){
                Rect_hold.x = initial_coordinate_x - 2 * block_side - buffer_distance + block_side - block_side_hold / 2 + position[0][i] * block_side_hold + player_distance * name;
                Rect_hold.y = initial_coordinate_y + block_side + position[1][i] * block_side_hold;
                Rect_hold.h = Rect_hold.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_S, NULL, &Rect_hold);
            }
            break;
        case T_t :
            four_of_block_position(T, position);
            for(int i = 0 ; i < 4 ; i++){
                Rect_hold.x = initial_coordinate_x - block_side - buffer_distance - block_side_hold / 2 + position[0][i] * block_side_hold + player_distance * name;
                Rect_hold.y = initial_coordinate_y + block_side + position[1][i] * block_side_hold;
                Rect_hold.h = Rect_hold.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_T, NULL, &Rect_hold);
            }
            break;
        case Z_t :
            four_of_block_position(Z, position);
            for(int i = 0 ; i < 4 ; i++){
                Rect_hold.x = initial_coordinate_x - 2 * block_side - buffer_distance + block_side - block_side_hold / 2 + position[0][i] * block_side_hold + player_distance * name;
                Rect_hold.y = initial_coordinate_y + block_side + position[1][i] * block_side_hold;
                Rect_hold.h = Rect_hold.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_Z, NULL, &Rect_hold);
            }
            break;
    }
}

void player::nextblockpresent(){
    block I(I_t, 0, 0), J(J_t, 0, 0), L(L_t, 0, 0), O(O_t, 0, 0), S(S_t, 0, 0), T(T_t, 0, 0), Z(Z_t, 0, 0);
    SDL_Rect Rect_nextblocks;
    int position[2][4];
    for(int i = 1 ; i < 6 ; i++){
        Rect_nextblocks.x = 10 * block_side + initial_coordinate_x + bar_width + player_distance * name;
        Rect_nextblocks.y = initial_coordinate_y + 2 * block_side * (i - 1);
        Rect_nextblocks.h = Rect_nextblocks.w = 2 * block_side;
        SDL_RenderCopy(rendertarget, image_hold_None, NULL, &Rect_nextblocks);
        switch(nextblocks[i]){
        case I_t :
            four_of_block_position(I, position);
            for(int j = 0 ; j < 4 ; j++){
                Rect_nextblocks.x = 10 * block_side + initial_coordinate_x + bar_width + block_side - block_side_hold + position[0][j] * block_side_hold + player_distance * name;
                Rect_nextblocks.y = initial_coordinate_y + 2 * block_side * (i - 1) + block_side - block_side_hold / 2 + position[1][j] * block_side_hold;
                Rect_nextblocks.h = Rect_nextblocks.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_I, NULL, &Rect_nextblocks);
            }
            break;
        case J_t :
            four_of_block_position(J, position);
            for(int j = 0 ; j < 4 ; j++){
                Rect_nextblocks.x = 10 * block_side + initial_coordinate_x + bar_width + block_side - block_side_hold / 2 + position[0][j] * block_side_hold + player_distance * name;
                Rect_nextblocks.y = initial_coordinate_y + 2 * block_side * (i - 1) + block_side + position[1][j] * block_side_hold;
                Rect_nextblocks.h = Rect_nextblocks.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_J, NULL, &Rect_nextblocks);
            }
            break;
        case L_t :
            four_of_block_position(L, position);
            for(int j = 0 ; j < 4 ; j++){
                Rect_nextblocks.x = 10 * block_side + initial_coordinate_x + bar_width + block_side - block_side_hold / 2 + position[0][j] * block_side_hold + player_distance * name;
                Rect_nextblocks.y = initial_coordinate_y + 2 * block_side * (i - 1) + block_side + position[1][j] * block_side_hold;
                Rect_nextblocks.h = Rect_nextblocks.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_L, NULL, &Rect_nextblocks);
            }
            break;
        case O_t :
            four_of_block_position(O, position);
            for(int j = 0 ; j < 4 ; j++){
                Rect_nextblocks.x = 10 * block_side + initial_coordinate_x + bar_width + block_side - block_side_hold + position[0][j] * block_side_hold + player_distance * name;
                Rect_nextblocks.y = initial_coordinate_y + 2 * block_side * (i - 1) + block_side + position[1][j] * block_side_hold;
                Rect_nextblocks.h = Rect_nextblocks.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_O, NULL, &Rect_nextblocks);
            }
            break;
        case S_t :
            four_of_block_position(S, position);
            for(int j = 0 ; j < 4 ; j++){
                Rect_nextblocks.x = 10 * block_side + initial_coordinate_x + bar_width + block_side - block_side_hold / 2 + position[0][j] * block_side_hold + player_distance * name;
                Rect_nextblocks.y = initial_coordinate_y + 2 * block_side * (i - 1) + block_side + position[1][j] * block_side_hold;
                Rect_nextblocks.h = Rect_nextblocks.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_S, NULL, &Rect_nextblocks);
            }
            break;
        case T_t :
            four_of_block_position(T, position);
            for(int j = 0 ; j < 4 ; j++){
                Rect_nextblocks.x = 10 * block_side + initial_coordinate_x + bar_width + block_side - block_side_hold / 2 + position[0][j] * block_side_hold + player_distance * name;
                Rect_nextblocks.y = initial_coordinate_y + 2 * block_side * (i - 1) + block_side + position[1][j] * block_side_hold;
                Rect_nextblocks.h = Rect_nextblocks.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_T, NULL, &Rect_nextblocks);
            }
            break;
        case Z_t :
            four_of_block_position(Z, position);
            for(int j = 0 ; j < 4 ; j++){
                Rect_nextblocks.x = 10 * block_side + initial_coordinate_x + bar_width + block_side - block_side_hold / 2 + position[0][j] * block_side_hold + player_distance * name;
                Rect_nextblocks.y = initial_coordinate_y + 2 * block_side * (i - 1) + block_side + position[1][j] * block_side_hold;
                Rect_nextblocks.h = Rect_nextblocks.w = block_side_hold;
                SDL_RenderCopy(rendertarget, image_Z, NULL, &Rect_nextblocks);
            }
            break;
        }
    }
}

void player::giveblock(){// rand
    if(record[0] + record[1] + record[2] + record[3] + record[4] + record[5] + record[6] == 7){
        for(int i = 0 ; i < 7 ; i++)
            record[i] = 0;
    }
    srand(time(0));
    here :
    int temp = rand() % 7;
    if(record[temp] == 0){
        record[temp] = 1;
        for(int i = 0 ; i < 5 ; i++){
            nextblocks[i] = nextblocks[i + 1];
        }
        nextblocks[5] = temp;
    }
    else{
        goto here;
    }
}

void player::showbackblock(){
    // 消排 (可另外寫函數處理)
    for(int i = 0 ; i < 10 ; i++){
        if(block_ary[0][i] != -1)
            gamerunning = false;//lose
    }
    // 背景方塊呈現
    SDL_Rect Rect_background;
    Rect_background.h = Rect_background.w = block_side;
    for(int i = 0 ; i < 10 ; i++){
        for(int j = 0 ; j < 20 ; j++){
            Rect_background.x = i * block_side + initial_coordinate_x + player_distance * name;
            Rect_background.y = j * block_side + initial_coordinate_y;
            switch(block_ary[j][i]){
                case -1 :
                    SDL_RenderCopy(rendertarget, image_background, NULL, &Rect_background);
                    break;
                case I_t :
                    SDL_RenderCopy(rendertarget, image_I, NULL, &Rect_background);
                    break;
                case J_t :
                    SDL_RenderCopy(rendertarget, image_J, NULL, &Rect_background);
                    break;
                case L_t :
                    SDL_RenderCopy(rendertarget, image_L, NULL, &Rect_background);
                    break;
                case O_t :
                    SDL_RenderCopy(rendertarget, image_O, NULL, &Rect_background);
                    break;
                case S_t :
                    SDL_RenderCopy(rendertarget, image_S, NULL, &Rect_background);
                    break;
                case T_t :
                    SDL_RenderCopy(rendertarget, image_T, NULL, &Rect_background);
                    break;
                case Z_t :
                    SDL_RenderCopy(rendertarget, image_Z, NULL, &Rect_background);
                    break;
                case garbage_t :
                    SDL_RenderCopy(rendertarget, image_garbage, NULL, &Rect_background);
                    break;
            }
        }
    }
    // HOLD 和 BAR 和 NEXT 和背景一起呈現
    this->holdpresent();
    this->nextblockpresent();
}

void player::destroy(){//消排function
    int rol = 19;
    int des_row = 0;
    int temp = 0;
    garbage = 0;
    if(running == false){
        while(rol > 0){
            int sum = 0;
            for(int j = 0 ; j < 10 ; j++){
                if(block_ary[rol][j] != -1)
                    sum++;
                else
                    break;
            }
            if(sum == 10){
                for(int i = rol ; i > 0 ; i--)
                    for(int j = 0 ; j < 10 ; j++)
                        block_ary[i][j] = block_ary[i - 1][j];
                des_row += 1;
            }
            else   rol--;
        }
        switch(des_row){
            case 0:combo = -1;break;
            case 1:combo+=1;break;
            case 2:temp = 1;combo+=1;garbage+=1;break;
            case 3:temp = 2;combo+=1;garbage+=1;break;
            case 4:temp = 4;combo+=1;B2B4+=1;garbage+=2;break;
        }score += temp;

        if(des_row > 0){
            switch(combo){
                case 0: break;
                case 1: score += 1; garbage+=1;Mix_VolumeMusic(MIX_MAX_VOLUME);Mix_PlayChannel(7, effect1, 0);break;
                case 2: score += 1; garbage+=1;Mix_VolumeMusic(MIX_MAX_VOLUME);Mix_PlayChannel(2, effect2, 0);break;
                case 3: score += 2; garbage+=1;Mix_VolumeMusic(MIX_MAX_VOLUME);Mix_PlayChannel(3, effect3, 0);break;
                case 4: score += 2; garbage+=1;Mix_VolumeMusic(MIX_MAX_VOLUME);Mix_PlayChannel(4, effect4, 0);break;
                case 5: score += 3; garbage+=2;Mix_VolumeMusic(MIX_MAX_VOLUME);Mix_PlayChannel(5, effect5, 0);break;
                case 6: score += 3; garbage+=2;Mix_VolumeMusic(MIX_MAX_VOLUME);Mix_PlayChannel(6, effect6, 0);break;
                default: score += 4; garbage+=3;Mix_VolumeMusic(MIX_MAX_VOLUME);Mix_PlayChannel(1, effect7, 0);break;
            }
        }
        if(B2B4 > 0){score+=2; B2B4 = 0;}
        Mix_VolumeMusic(MIX_MAX_VOLUME/6);
        //if(P1.score != 0 || P2.score != 0)  cout<<"P1.score = "<< P1.score<<", P2.score = "<< P2.score<<endl;
    }
}

void player::attack(player& P){
    int number = P.garbage;
    int rol = 0 ;
    bool running = true;
    while(running){
        for(int j = 0 ; j < 10 ; j++)
            if(block_ary[rol][j] != -1)
                running = false;
        rol++;
    }
    srand(time(0));
    int temp = rand() % 10;
    for(int i = 0 ; i <= 19 - number; i++)
        for(int j = 0 ; j < 10 ; j++)
            block_ary[i][j] = block_ary[i + number][j];
    for(int i = 0 ; i < number; i++){
        block_ary[19 - i][temp] = -1;
        for(int j = 0 ; j < 10 ; j++)
            if(j != temp)
                block_ary[19 - i][j] = 7;//change
    }
}

void player::reset_ary(int type){
    switch(type){
        case Original :
            for(int i = 0 ; i < 20 ; i++)
                for(int j = 0 ; j < 10 ; j++)
                    block_ary[i][j] = -1;
            for(int i = 0 ; i < 10 ; i++)
                block_ary[20][i] = 0;
            break;
        case Tetris :
            for(int i = 0 ; i < 20 ; i++)
                for(int j = 0 ; j < 10 ; j++)
                    block_ary[i][j] = -1;
            block_ary[14][0] = block_ary[14][1] = block_ary[14][2] = block_ary[15][1] = block_ary[16][1] = 0;
            block_ary[15][2] = block_ary[15][3] = block_ary[16][2] = block_ary[17][2] = block_ary[17][3] = block_ary[18][2] = block_ary[19][2] = block_ary[19][3] = 1;
            block_ary[14][3] = block_ary[14][4] = block_ary[14][5] = block_ary[15][4] = block_ary[16][4] = 2;
            block_ary[16][4] = block_ary[16][5] = block_ary[16][6] = block_ary[17][4] = block_ary[17][6] = block_ary[18][4] = block_ary[18][5] = block_ary[19][4] = block_ary[19][6] = 3;
            block_ary[15][7] = block_ary[16][7] = block_ary[17][7] = block_ary[18][7] = 4;
            block_ary[15][8] = block_ary[15][9] = block_ary[16][8] = block_ary[17][8] = block_ary[17][9] = block_ary[18][9] = block_ary[19][8] = block_ary[19][9] = 5;
            for(int i = 0 ; i < 10 ; i++)
                block_ary[20][i] = 0;
            break;
        case Lines :
            for(int i = 0 ; i < 20 ; i++)
                for(int j = 0 ; j < 10 ; j++)
                    block_ary[i][j] = -1;
            for(int i = 13 ; i < 20 ; i++)
                for(int j = 0 ; j < 10 ; j++){
                    if((i + j) % 7 == 0)
                        block_ary[i][j] = 2;
                    if((i + j) % 7 == 2)
                        block_ary[i][j] = 5;
                    if((i + j) % 7 == 4)
                        block_ary[i][j] = 6;
                }
            for(int i = 0 ; i < 10 ; i++)
                block_ary[20][i] = 0;
            break;
    }
}

void showbackground(player& P1, player& P2){
    SDL_Rect Rect_main_background;
    Rect_main_background.x = Rect_main_background.y = 0;
    Rect_main_background.h = Surface_height;
    Rect_main_background.w = Surface_width;
    SDL_RenderCopy(rendertarget, image_main_background, NULL, &Rect_main_background);
    P1.showbackblock();
    P2.showbackblock();

    timeText.str("");
    timeText << "0" << int(120-timer.getTicks() / 1000.f ) / 60 << ":"
        << int(120-timer.getTicks() / 1000.f) %60 / 10 << int(120 - timer.getTicks() / 1000.f ) % 60 % 10;
    if( int(timer.getTicks() / 1000.f) == 120 )/************/
        P1.gamerunning = P2.gamerunning = false;

    gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), {255, 255 , 0, 255});
    SDL_Rect Rect_Text;
    Rect_Text.x = (Surface_width - gTimeTextTexture.mWidth) / 2;
    Rect_Text.y = (Surface_height - gTimeTextTexture.mHeight) / 2;
    Rect_Text.h = gTimeTextTexture.mHeight;
    Rect_Text.w = gTimeTextTexture.mWidth;
    SDL_RenderCopy(rendertarget, gTimeTextTexture.mTexture, NULL, &Rect_Text);

    timeText.str("");
    timeText << P1.score;
    Rect_Text.h = gTimeTextTexture.mHeight*3;
    Rect_Text.w = gTimeTextTexture.mWidth/2;
    gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), {255, 0 , 0, 255});
    Rect_Text.x = (Surface_width - gTimeTextTexture.mWidth) / 2 - 4.5 * block_side;
    Rect_Text.y = Surface_height - gTimeTextTexture.mHeight / 2 - 6*block_side;
    SDL_RenderCopy(rendertarget, gTimeTextTexture.mTexture, NULL, &Rect_Text);

    timeText.str("");
    timeText << P2.score;
    gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), {0, 0 , 255, 255});
    Rect_Text.x = (Surface_width - gTimeTextTexture.mWidth) / 2 + block_side * 3;
    SDL_RenderCopy(rendertarget, gTimeTextTexture.mTexture, NULL, &Rect_Text);
    SDL_RenderPresent(rendertarget);
}

void stuck(player& P, bool& moving){//deal with stuck problems, there are some bugs
    int position[2][4];
    int cnt = 0;
    moving = false;
    while(!moving){
        four_of_block_position(P.NOW, position);
        for(int i = 0 ; i < 4 ; i++){
            if(position[0][i] >= 10){//cout <<"A";
                P.NOW.movecenter(LEFT);
            break;
            }
            if(position[0][i] <= -1){//cout <<"B";
                P.NOW.movecenter(RIGHT);
                break;
            }
            if(position[1][i] >= 20){//cout <<"C";
                P.NOW.movecenter(UP);
                break;
            }
            if(P.block_ary[ position[1][i] ][ position[0][i] ] != -1){//cout <<"D"<<i;
                for(int j = 0 ; j < 10 ; j++)
                    if(P.block_ary[ 0 ][ position[0][j] ] != -1){
                        P.gamerunning = false;//cout <<"Y";
                        moving = true;
                    }
                if(P.gamerunning == true && spin == true){
                    switch(cnt){
                        case 0:
                            P.NOW.movecenter(LEFT);
                            break;
                        case 1:
                            P.NOW.movecenter(RIGHT);
                            P.NOW.movecenter(RIGHT);
                            break;
                        case 2:
                            P.NOW.movecenter(LEFT);
                            P.NOW.movecenter(DOWN);
                            break;
                        case 3:
                            P.NOW.movecenter(UP);
                            P.NOW.movecenter(UP);
                            break;
                        default :
                            P.NOW.movecenter(UP);
                            break;
                    }
                    cnt++;
                }
                else if(P.gamerunning == true && spin == false){
                    P.NOW.movecenter(UP);
                }
                break;
            }
            if(i == 3 && P.block_ary[ position[1][i] ][ position[0][i] ] == -1){
                moving = true;
            }
        }
    }
}

void run_block(player& P1, player& P2, clock_t& t){//important function
    P1.running = true;
    P2.running = true;
    bool moving = true;
    P1.shifttimes = P2.shifttimes = false;
    int position[2][4];
    SDL_Event e;
    here :
    breakblock(P1, P2);
    while(P1.running && P2.running && P1.gamerunning && P2.gamerunning){
        P1.showblock();
        P2.showblock();
        blockfall(t, P1, P2);
        P1.showshadow();
        P2.showshadow();

        while(SDL_PollEvent(&e)){
            if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_UP :
                    case SDLK_x :
                        P1.NOW.changeorientation(1);
                        spin = true;
                        stuck(P1, moving);
                        spin = false;
                        breakblock(P1, P2);
                        P1.showblock();
                        break;
                    case SDLK_z :
                        P1.NOW.changeorientation(-1);
                        spin = true;
                        stuck(P1, moving);
                        spin = false;
                        breakblock(P1, P2);
                        P1.showblock();
                        break;
                    case SDLK_LEFT :
                        P1.NOW.movecenter(LEFT);
                        stuck(P1, moving);
                        breakblock(P1, P2);
                        P1.showblock();
                        break;
                    case SDLK_RIGHT :
                        P1.NOW.movecenter(RIGHT);
                        stuck(P1, moving);
                        breakblock(P1, P2);
                        P1.showblock();
                        break;
                    case SDLK_DOWN :
                        P1.NOW.movecenter(DOWN);
                        stuck(P1, moving);
                        breakblock(P1, P2);
                        P1.showblock();
                        break;
                    case SDLK_LSHIFT :
                    case SDLK_RSHIFT :
                    case SDLK_c :
                        {
                            if(P1.shifttimes == false){
                                if(P1.hold_type == -1){
                                    P1.hold_type = P1.NOW.type;
                                    P1.giveblock();
                                    P1.resetblock();
                                    P1.shifttimes = true;
                                }
                                else{
                                    int temp = P1.hold_type;
                                    P1.hold_type = P1.NOW.type;
                                    P1.NOW.type = temp;
                                    P1.NOW.center_x = 4;
                                    P1.NOW.center_y = 2;
                                    P1.NOW.orientation = 0;
                                    P1.shifttimes = true;
                                    goto here;
                                }
                            }
                        }
                        break;
                    case SDLK_SPACE ://跑回圈一直往下 直到撞到為止 再向上一格
                        while(moving){
                            P1.NOW.movecenter(DOWN);
                            four_of_block_position(P1.NOW, position);
                            for(int i = 0 ; i < 4 ; i++){
                                if(P1.block_ary[ position[1][i] ][ position[0][i] ] != -1){
                                    moving = false;
                                    P1.NOW.movecenter(UP);
                                    for(int i = 0 ; i < 4 ; i++){
                                        P1.block_ary[ position[1][i] - 1 ][ position[0][i] ] = P1.NOW.type;
                                    };
                                    break;
                                }
                            }
                        }
                        moving = true;
                        P1.giveblock();
                        P1.resetblock();
                        breakblock(P1, P2);
                        P1.showblock();
                        P1.running = false;
                        break;
                    case SDLK_p :
                    case SDLK_e :
                        P2.NOW.changeorientation(1);
                        spin = true;
                        stuck(P1, moving);
                        spin = false;
                        breakblock(P1, P2);
                        P2.showblock();
                        break;
                    case SDLK_w :
                        P2.NOW.changeorientation(-1);
                        spin = true;
                        stuck(P1, moving);
                        spin = false;
                        breakblock(P1, P2);
                        P2.showblock();
                        break;
                    case SDLK_l :
                        P2.NOW.movecenter(LEFT);
                        stuck(P2, moving);
                        breakblock(P1, P2);
                        P2.showblock();
                        break;
                    case SDLK_QUOTE :
                        P2.NOW.movecenter(RIGHT);
                        stuck(P2, moving);
                        breakblock(P1, P2);
                        P2.showblock();
                        break;
                    case SDLK_SEMICOLON :
                        P2.NOW.movecenter(DOWN);
                        stuck(P2, moving);
                        breakblock(P1, P2);
                        P2.showblock();
                        break;
                    case SDLK_q:
                        {
                            if(P2.shifttimes == false){
                                if(P2.hold_type == -1){
                                    P2.hold_type = P2.NOW.type;
                                    P2.giveblock();
                                    P2.resetblock();
                                    P2.shifttimes = true;
                                }
                                else{
                                    int temp = P2.hold_type;
                                    P2.hold_type = P2.NOW.type;
                                    P2.NOW.type = temp;
                                    P2.NOW.center_x = 4;
                                    P2.NOW.center_y = 2;
                                    P2.NOW.orientation = 0;
                                    P2.shifttimes = true;
                                    goto here;
                                }
                            }
                        }
                        break;
                    case SDLK_d ://跑回圈一直往下 直到撞到為止 再向上一格
                        while(moving){
                            P2.NOW.movecenter(DOWN);
                            four_of_block_position(P2.NOW, position);
                            for(int i = 0 ; i < 4 ; i++){
                                if(P2.block_ary[ position[1][i] ][ position[0][i] ] != -1){
                                    moving = false;
                                    P2.NOW.movecenter(UP);
                                    for(int i = 0 ; i < 4 ; i++){
                                        P2.block_ary[ position[1][i] - 1 ][ position[0][i] ] = P2.NOW.type;
                                    };
                                    break;
                                }
                            }
                        }
                        moving = true;
                        P2.giveblock();
                        P2.resetblock();
                        breakblock(P1, P2);
                        P2.showblock();
                        P2.running = false;
                        break;
                    case SDLK_ESCAPE :
                        ESCpress = true;
                        P1.running = P2.running = false;
                        break;
                }
            }
        }
    }
    P1.destroy();
    P2.destroy();
}

void player::resetblock(){
    NOW.type = nextblocks[0];
    NOW.center_x = 4;
    NOW.center_y = 1;
    NOW.orientation = 0;
}

void player::showshadow(){
    int position[2][4];
    bool moving = true;
    four_of_block_position(NOW, position);
    while(1){
        for(int i = 0 ; i < 4 ; i++){
            if(block_ary[ position[1][i] ][ position[0][i] ] != -1)
                moving = false;
        }
        if(moving == false){
            for(int i = 0 ; i < 4 ; i++)
                position[1][i]--;
            for(int i = 0 ; i < 4 ; i++){
                SDL_Rect Rect_shadow;
                Rect_shadow.h = Rect_shadow.w = block_side;
                Rect_shadow.x = position[0][i] * block_side + initial_coordinate_x + player_distance * name;
                Rect_shadow.y = position[1][i] * block_side + initial_coordinate_y;
                SDL_RenderCopy(rendertarget, image_shadow, NULL, &Rect_shadow);
            }
            break;
        }
        else{
            for(int i = 0 ; i < 4 ; i++)
                position[1][i]++;
        }
    }
}

void blockfall(clock_t& t, player& P1, player& P2){
    int position[2][4];
    if(clock() - t >= block_time){
        t = clock();
        P1.NOW.movecenter(DOWN);
        P2.NOW.movecenter(DOWN);
        four_of_block_position(P1.NOW, position);
        for(int i = 0 ; i < 4 ; i++){
            if(P1.block_ary[ position[1][i] ][ position[0][i] ] != -1){
                P1.NOW.movecenter(UP);
                for(int i = 0 ; i < 4 ; i++){
                    P1.block_ary[ position[1][i] - 1 ][ position[0][i] ] = P1.NOW.type;
                };
                P1.running = false;
                P1.giveblock();
                P1.resetblock();
                break;
            }
        }
        four_of_block_position(P2.NOW, position);
        for(int i = 0 ; i < 4 ; i++){
            if(P2.block_ary[ position[1][i] ][ position[0][i] ] != -1){
                P2.NOW.movecenter(UP);
                for(int i = 0 ; i < 4 ; i++){
                    P2.block_ary[ position[1][i] - 1 ][ position[0][i] ] = P2.NOW.type;
                };
                P2.running = false;
                P2.giveblock();
                P2.resetblock();
                break;
            }
        }
        breakblock(P1, P2);
        P1.showblock();
        P2.showblock();
    }
}

void menu::start_menu(){// all menu operations
    SDL_RenderClear(rendertarget);
    static int music_type = 0;

    here :
    SDL_Rect startTexture_data;
    startTexture_data.x = player_distance / 2 + block_side * 2;
    startTexture_data.y =0;
    startTexture_data.h = 781/1.28;
    startTexture_data.w = 814/1.28;
    SDL_RenderCopy(rendertarget, startTexture, NULL, &startTexture_data);

    SDL_Rect button;
    button.x = button_side; button.y = button_side;
    button.h = button_side; button.w = button_side;
    SDL_RenderCopy(rendertarget, rule, NULL, &button);

    button.x += 2 * button_side;
    if(music_type == 0) SDL_RenderCopy(rendertarget, music1, NULL, &button);
    else if(music_type == 1) SDL_RenderCopy(rendertarget, music2, NULL, &button);
    else SDL_RenderCopy(rendertarget, music3, NULL, &button);

    button.x += 2 * button_side;
    SDL_RenderCopy(rendertarget, start, NULL, &button);

    button.x = button_side;button.y += 2 * button_side;
    form_type == Dark ? SDL_RenderCopy(rendertarget, back1_1, NULL, &button) : SDL_RenderCopy(rendertarget, back1, NULL, &button);

    button.x += 2 * button_side;
    form_type == Bright ? SDL_RenderCopy(rendertarget, back2_1, NULL, &button) : SDL_RenderCopy(rendertarget, back2, NULL, &button);

    button.x = button_side; button.y += 2 * button_side;
    map_type == Original ? SDL_RenderCopy(rendertarget, map1_1, NULL, &button) : SDL_RenderCopy(rendertarget, map1, NULL, &button);

    button.x += 2 * button_side;
    map_type == Tetris ? SDL_RenderCopy(rendertarget, map2_1, NULL, &button) : SDL_RenderCopy(rendertarget, map2, NULL, &button);

    button.x += 2 * button_side;
    map_type == Lines ? SDL_RenderCopy(rendertarget, map3_1, NULL, &button) : SDL_RenderCopy(rendertarget, map3, NULL, &button);


    SDL_Event e;
    bool running = true;
    while(running){
        SDL_RenderPresent(rendertarget);
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_MOUSEBUTTONDOWN){
                int x, y;
                SDL_GetMouseState(&x, &y);
                if(x >= button_side && x <= 2 * button_side && y >= 3 * button_side && y <= 4 * button_side){
                    form_type = Dark;
                    SDL_RenderClear(rendertarget);
                    goto here;
                }
                else if(x >= 3 * button_side && x <= 4 * button_side && y >= 3 * button_side && y <= 4 * button_side){
                    form_type = Bright;
                    SDL_RenderClear(rendertarget);
                    goto here;
                }
                else if(x >= button_side && x <= 2 * button_side && y >= 5 * button_side && y <= 6 * button_side){
                    map_type = Original;
                    SDL_RenderClear(rendertarget);
                    goto here;
                }
                else if(x >= 3 * button_side && x <= 4 * button_side && y >= 5 * button_side && y <= 6 * button_side){
                    map_type = Tetris;
                    SDL_RenderClear(rendertarget);
                    goto here;
                }
                else if(x >= 5 * button_side && x <= 6 * button_side && y >= 5 * button_side && y <= 6 * button_side){
                    map_type = Lines;
                    SDL_RenderClear(rendertarget);
                    goto here;
                }
                else if(x >= button_side && x <= 2 * button_side && y >= button_side && y <= 2 * button_side){
                    bool out = false;
                    SDL_Rect rule;
                    rule.x = initial_coordinate_x; rule.y = initial_coordinate_y;
                    rule.h = 600 / 1.2; rule.w = 500 / 1.2;
                    SDL_SetRenderDrawColor(rendertarget, 255, 255, 255, 255);
                    SDL_RenderClear(rendertarget);
                    SDL_RenderCopy(rendertarget, rule_key, NULL, &rule);
                    rule.x += initial_coordinate_x + 500 / 1.28 + block_side;
                    rule.y += block_side;
                    SDL_RenderCopy(rendertarget, rule_score, NULL, &rule);
                    SDL_RenderPresent(rendertarget);
                    SDL_Event e_temp;
                    while(!out){
                        while(SDL_PollEvent(&e_temp)){
                            if(e_temp.type == SDL_MOUSEBUTTONDOWN){
                                out = true;
                            }
                        }
                    }
                    SDL_SetRenderDrawColor(rendertarget, 0, 0, 0, 255);
                    SDL_RenderClear(rendertarget);
                    goto here;
                }
                else if(x >= 5*button_side && x <= 6 * button_side && y >= button_side && y <= 2 * button_side){
                    running = false;
                }
                else if(x >= 3 * button_side && x <= 4 * button_side && y >= button_side && y <= 2 * button_side){
                    SDL_Rect music;
                    music.x = 3 *button_side;
                    music.y = button_side;
                    music.h = music.w = button_side;
                    music_type++;
                    music_type %= 3;
                    SDL_RenderClear(rendertarget);
                    Mix_HaltMusic();
                    if(music_type == 0) Mix_PlayMusic(bgm1, -1);
                    else if(music_type == 1) Mix_PlayMusic(bgm2, -1);
                    else Mix_PlayMusic(bgm3, -1);
                    goto here;
                }
            }
            else if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    running = false;
                    ESCpress = true;
                }
            }
        }
    }
}

void LoadButtons(){
    startTexture = LoadTexture("menu/start1.png");
    player1win = LoadTexture("menu/player1win.png");
    player2win = LoadTexture("menu/player2win.png");
    start = LoadTexture("menu/start.png");
    rule = LoadTexture("menu/rule.png");
    map1 = LoadTexture("menu/map1.png");
    map2 = LoadTexture("menu/map2.png");
    map3 = LoadTexture("menu/map3.png");
    back1 = LoadTexture("menu/back1.png");
    back2 = LoadTexture("menu/back2.png");
    map1_1 = LoadTexture("menu/map1_1.png");
    map2_1 = LoadTexture("menu/map2_1.png");
    map3_1 = LoadTexture("menu/map3_1.png");
    back1_1 = LoadTexture("menu/back1_1.png");
    back2_1 = LoadTexture("menu/back2_1.png");
    rule_key = LoadTexture("menu/key.jpg");
    rule_score = LoadTexture("menu/score.jpg");
    music1 = LoadTexture("menu/music1.png");
    music2 = LoadTexture("menu/music2.png");
    music3 = LoadTexture("menu/music3.png");
}

void LoadMixs(){
    bgm1 = Mix_LoadMUS("sound/Daisy_Dukes.mp3");
    bgm2 = Mix_LoadMUS("sound/StarWars.mp3");
    bgm3 = Mix_LoadMUS("sound/Heavy.mp3");
    effect1 = Mix_LoadWAV("sound/effect/1.mp3");
    effect2 = Mix_LoadWAV("sound/effect/2.mp3");
    effect3 = Mix_LoadWAV("sound/effect/3.mp3");
    effect4 = Mix_LoadWAV("sound/effect/4.mp3");
    effect5 = Mix_LoadWAV("sound/effect/5.mp3");
    effect6 = Mix_LoadWAV("sound/effect/6.mp3");
    effect7 = Mix_LoadWAV("sound/effect/7.mp3");
}

