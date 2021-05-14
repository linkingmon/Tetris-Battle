#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <SDL_mixer.h>
#include "block.h"
#include"timer.h"
using namespace std;

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("TETRIS BATTLE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Surface_width, Surface_height, SDL_WINDOW_SHOWN);
    windowsurface = SDL_GetWindowSurface(window);
    rendertarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    menu M;
    int menu_type = 1;

    SDL_Rect playerwin;
    playerwin.x = Surface_width / 2 - 300 * 1.6;
    playerwin.y = Surface_height / 2 - 150 * 3;
    playerwin.w = 600;
    playerwin.h = 300;

    TTF_Init();
    loadMedia();

    LoadMixs();
    Mix_VolumeMusic(MIX_MAX_VOLUME/6);
    LoadButtons();
    Mix_PlayMusic(bgm1, -1);


    nextgame :
    M.start_menu();
        if(ESCpress == false){
        LoadTextures(form_type);
        player P1(map_type, 0), P2(map_type, 1);
        SDL_RenderClear(rendertarget);
        showbackground(P1, P2);

        for(int i = 0 ; i < 6 ; i++){
            P1.giveblock();
            P2.giveblock();
        }
        clock_t t= clock();
        SDL_Event e;

        timer.start();
        P1.resetblock();
        P2.resetblock();
        if(player1win == NULL) cout <<"S";
        while(P1.gamerunning && P2.gamerunning){
            run_block(P1, P2, t);
            P1.attack(P2);
            P2.attack(P1);
            if(ESCpress == true)
                break;
        }
        SDL_Delay(1000);
        if(P1.gamerunning == false && P2.gamerunning == false){
            if(P1.score > P2.score)
                SDL_RenderCopy(rendertarget, player1win, NULL, &playerwin);
            else if(P2.score > P1.score)
                SDL_RenderCopy(rendertarget, player2win, NULL, &playerwin);
        }
        else
            P2.gamerunning == false ? SDL_RenderCopy(rendertarget, player1win, NULL, &playerwin) : SDL_RenderCopy(rendertarget, player2win, NULL, &playerwin);
        if(ESCpress == false){
        SDL_RenderPresent(rendertarget);
        SDL_Delay(2500);
        P1.gamerunning = P2.gamerunning = true;
        P1.reset_ary(0);
        P2.reset_ary(0);
        goto nextgame;
        }
    }
    close();
    SDL_DestroyTexture(image_background);
    image_background = NULL;
    rendertarget = NULL;
    return 0;
}



