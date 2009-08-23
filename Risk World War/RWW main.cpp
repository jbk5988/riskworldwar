#include <string>
using namespace std;

#define NUM_OF_TERRITORIES    42


#include "SDL.h"
#include "GraphicsLoader.h"
#include "Territory.h"
bool load_territories(Territory[]);

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;
static const int SCREEN_BPP = 32;

class Info_Handler
{
public:
	Territory * allTerritories;
	SDL_Surface * screen;
	SDL_Surface * ascii;
	SDL_Surface * mapIm;
	SDL_Surface * borders;
	SDL_Surface * buttons;
	SDL_Rect asciiRect, mapRect, borderRect, consoleRect, buttonRect;
	SDL_Event event;
	bool keepGoing;
	bool isFullScreen;

	string outputMessage;

	int displayTimer, displayTimerMax, state, playersTurn, numOfPlayers;
	Info_Handler()
		:keepGoing(true), isFullScreen(false), state(0), displayTimer(0), displayTimerMax(5000), playersTurn(1), numOfPlayers(4)
	{
		allTerritories = new Territory[NUM_OF_TERRITORIES];
		screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
		ascii = load_my_image("ascii.bmp");
		mapIm = load_my_image("Risk Map.bmp");
		borders = load_my_image("borders.bmp");
		buttons = load_my_image("buttons.bmp");
		SDL_SetColorKey(borders, SDL_SRCCOLORKEY, 0xffffff);
		asciiRect.x = asciiRect.y = 0;
		asciiRect.h = 12;
		asciiRect.w = 8;
		mapRect.x = mapRect.y = 0;
		mapRect.w = 1280;
		mapRect.h = 720;
		borderRect.x = borderRect.y = 0;
		borderRect.h = borderRect.w = 20;
		consoleRect.h = 280;
		consoleRect.w = 170;
		consoleRect.x = 0;
		consoleRect.y = screen->h - consoleRect.h;

		buttonRect.x = buttonRect.y = 0;
		buttonRect.w = buttonRect.h = 64;
		
		load_territories(allTerritories);
		
	}
	~Info_Handler()
	{
		delete [] allTerritories;
		SDL_FreeSurface(screen);
		SDL_FreeSurface(borders);
		SDL_FreeSurface(ascii);
		SDL_FreeSurface(mapIm);
		SDL_FreeSurface(buttons);
	}
};

#include "tools.h"

int main(int argc, char * argv[])
{

	if(SDL_Init( SDL_INIT_EVERYTHING) == -1)
	{
		return 1;
	}

	Info_Handler IH;
	char util1[32];
	int msPassed = 0;
	Uint32 then = SDL_GetTicks(), now;
	
	while(IH.keepGoing)
	{
		//input
		while(SDL_PollEvent(&IH.event))
			eventHandler(IH);

		//update		
		now = SDL_GetTicks();
		msPassed = now - then;
		then = now;
		update(IH, msPassed);

		//draw
		SDL_FillRect(IH.screen, &IH.screen->clip_rect, 0xffffff);
		drawATile(IH.mapIm, &IH.mapRect, 0, IH.screen, IH.mapRect.x, IH.mapRect.y);
		drawAll(IH, util1);
		if(SDL_Flip(IH.screen) == -1)
			return 1;		
	}

	SDL_Quit();
	
	return 0;
}