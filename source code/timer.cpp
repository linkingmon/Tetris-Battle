#include"timer.h"
#include"block.h"
#include<iostream>
#include<SDL_image.h>
using namespace std;

TTF_Font* gFont = NULL;

void LTexture::loadFromRenderedText(string textureText, SDL_Color textColor){
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	mTexture = SDL_CreateTextureFromSurface(rendertarget, textSurface);
    mWidth = textSurface->w;
    mHeight = textSurface->h;
    SDL_FreeSurface(textSurface);
}

void LTexture::free(){
    SDL_DestroyTexture( mTexture );
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue){
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void loadMedia(){
	gFont = TTF_OpenFont( "image/Pacifico.ttf", 50 );
}

void close(){
	gTimeTextTexture.free();
	TTF_CloseFont(gFont);
	gFont = NULL;
	SDL_DestroyRenderer(rendertarget);
	SDL_DestroyWindow(window);
	window = NULL;
	rendertarget = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}

Uint32 LTimer::getTicks(){
	Uint32 time = 0;
    time = SDL_GetTicks() - mStartTicks;
    return time;
}
