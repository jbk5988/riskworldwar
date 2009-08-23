#include <string>
using namespace std;

#define NUM_OF_TERRITORIES    42


#include "SDL.h"
#include "GraphicsLoader.h"
#include "Territory.h"
#include "playerClass.h"
bool load_territories(Territory[]);

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;
static const int SCREEN_BPP = 32;

#include "InfoHandler.h"

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

	IH.resetPlayers(IH.numOfPlayers);
	
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