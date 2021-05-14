#ifndef TIMER_H
#define YIMER_H
#include<SDL_ttf.h>
#include<iostream>
#include"block.h"
using namespace std;

class LTexture{
	public:
		LTexture(){mTexture = NULL; mWidth = 0; mHeight = 0;};
		~LTexture(){free();};
		void free();
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		friend void showbackground(player&, player&);
		void loadFromRenderedText(string, SDL_Color);
    private :
		SDL_Texture* mTexture;
		int mWidth;
		int mHeight;
};

class LTimer{
    public:
		LTimer(){mStartTicks = 0;};
		void start(){mStartTicks = SDL_GetTicks();};
		Uint32 getTicks();
    private:
		Uint32 mStartTicks;
};

bool init();
void loadMedia();
void close();
extern TTF_Font* gFont;
extern LTexture gTimeTextTexture;
extern LTimer timer;
extern SDL_Color textColor;
#endif
