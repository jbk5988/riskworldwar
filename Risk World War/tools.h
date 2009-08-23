#pragma once
#include <fstream>
#include <iostream>
#include <conio.h>

#define TERRITORY_DATA_FILE        "territory list.txt"

bool load_territories(Territory allTerritories[])
{
	ifstream inFile(TERRITORY_DATA_FILE);
	if(!inFile.is_open())//if the file doesn't open say so
		return false;


	SDL_Rect r = {0,0,0,0};
	string n = "NONE";
	int c[MAX_CONNECTS] = {-1};
	
	for(int i = 0; i < NUM_OF_TERRITORIES; ++i)
	{
		for(int k = 0; k < MAX_CONNECTS; ++k)
			c[k] = -1;
		getline(inFile, n, '#');
		inFile.ignore();
		inFile >> r.x;
		inFile.ignore();
		inFile >> r.y;
		inFile.ignore();
		inFile >> r.w;
		inFile.ignore();
		inFile >> r.h;
		for(int j = 0; j < MAX_CONNECTS; ++j)
		{
			inFile >> c[j];
			if(c[j] == -1)
				break;
		}
		allTerritories[i].setData(n, 0, -1, i, false, false, c, r);
		inFile.ignore(2);
	}
}



void compGrabRandom(Info_Handler & IH, int player)
{
	bool looking = true;
	int random;
	while(looking)
	{
		random = getRandomNum() % NUM_OF_TERRITORIES;
		if(IH.allTerritories[random].getOwner() == -1)
		{
			IH.allTerritories[random].setOwner(player);
			IH.allTerritories[random].modTroops(1);
			IH.allPlayers[player].decrementReinforcements();
			IH.allPlayers[player].addTerritory(random);
			if(IH.allPlayers[player].getNumberOfTerritories() == 1)
			{
				IH.allTerritories[random].setAsCapitol();
				IH.allPlayers[player].setCapitol(random);
			}
			looking = false;
		}
	}
}

void compReenforceRandom(Info_Handler & IH, int player)
{
	int rand = (getRandomNum() % IH.allPlayers[player].getNumberOfTerritories());
	IH.allTerritories[IH.allPlayers[player].getTerritory(rand)].modTroops(1);
	IH.allPlayers[player].decrementReinforcements();
}

void drawAll(Info_Handler & IH, char * util)
{

	itoa(SDL_GetTicks() / 1000, util, 10);
	writeText(IH.ascii, &IH.asciiRect, IH.screen, util, IH.screen->w - 400, IH.screen->h - IH.asciiRect.h);
	drawATile(IH.hud1, &IH.hudRect, 0, IH.screen, IH.hudRect.x, IH.hudRect.y);
	SDL_FillRect(IH.screen, &IH.consoleRect, 0x000000);
	writeToConsole(IH.ascii, &IH.asciiRect, IH.screen, IH.outputMessage, &IH.consoleRect);
	itoa(IH.allPlayers[1].getReinforce(), util, 10);
	writeText(IH.ascii, &IH.asciiRect, IH.screen, util, 0, 200);
	switch(IH.state)
	{
	case 0:	
		drawATile(IH.buttons, &IH.buttonRect, 0, IH.screen, 0,0);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		drawATile(IH.buttons, &IH.buttonRect, 1, IH.screen, 0,0);
		break;
	}
	for(int i = 0; i < NUM_OF_TERRITORIES; ++i)
	{
		drawATile(IH.borders, &IH.borderRect, IH.allTerritories[i].getOwner() + 1, IH.screen, IH.allTerritories[i].getRect().x, IH.allTerritories[i].getRect().y);
		if(IH.allTerritories[i].isCapitol())
			drawATile(IH.borders, &IH.borderRect, 7, IH.screen, IH.allTerritories[i].getRect().x, IH.allTerritories[i].getRect().y);
		//writeText(IH.ascii, &IH.asciiRect, IH.screen, IH.allTerritories[i].getName().c_str(), IH.allTerritories[i].getRect().x, IH.allTerritories[i].getRect().y);
	}
	for(int i = 0; i < NUM_OF_TERRITORIES; ++i)
	{
		if(IH.allTerritories[i].isHighlighted())
		{
			drawATile(IH.borders, &IH.borderRect, 0, IH.screen, IH.allTerritories[i].getRect().x, IH.allTerritories[i].getRect().y);
			for(int j = 0; j < NUM_OF_TERRITORIES; ++j)
			{
				if(IH.allTerritories[i].isConnTo(j) && IH.allTerritories[j].getOwner() == IH.allTerritories[i].getOwner())
				{
					drawATile(IH.borders, &IH.borderRect, 8, IH.screen, IH.allTerritories[j].getRect().x, IH.allTerritories[j].getRect().y);
				}
				else if (IH.allTerritories[i].isConnTo(j))
				{
					drawATile(IH.borders, &IH.borderRect, 6, IH.screen, IH.allTerritories[j].getRect().x, IH.allTerritories[j].getRect().y);
				}
			}
		}
	}
}

void update(Info_Handler & IH, int msPassed)
{
	IH.displayTimer += msPassed;
	if(IH.displayTimer > IH.displayTimerMax)
	{
		IH.outputMessage = "";
		IH.displayTimer = 0;
	}

	if(IH.state == 1 && !IH.allTerTaken() && IH.playersTurn > 0)
	{
		for(int i = 1; i < IH.numOfPlayers; ++i)
		{			
			if(!IH.allTerTaken())
				compGrabRandom(IH, IH.playersTurn);
			IH.playersTurn++;
		}
		IH.playersTurn = 0;
	}
	else if(IH.state == 1 && IH.allTerTaken())
		IH.state++;
	if(IH.state == 2 && IH.playersTurn > 0)
	{
		for(int i = 1; i < IH.numOfPlayers; ++i)
		{
			if(IH.allPlayers[i].getReinforce() > 0)
			{
				compReenforceRandom(IH, i);
			}
		}
		IH.playersTurn = 0;
	}
	else if(IH.state == 2 && IH.allReinforced())
		IH.state++;
}

void eventHandler(Info_Handler & IH)
{
	int temp;
	if(IH.event.type == SDL_QUIT)
		IH.keepGoing = false;
	if(IH.event.type == SDL_KEYDOWN)
	{
		switch(IH.event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			IH.keepGoing = false;
			break;
		case SDLK_F3:
			if(IH.isFullScreen)
			{
				IH.screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_SWSURFACE);
				IH.isFullScreen = false;
			}
			else
			{
				IH.screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN);
				IH.isFullScreen = true;
			}
			break;
		}
	}
	if(IH.event.type == SDL_MOUSEBUTTONDOWN)
	{
		if(IH.event.button.button == SDL_BUTTON_LEFT && IH.playersTurn == 0) 
		{
			for(int i = 0; i < NUM_OF_TERRITORIES; ++i)
				IH.allTerritories[i].dehighlight();
			temp = IH.checkTerClicked();
			if(temp >= 0)
			{
				switch(IH.state)
				{
				case 0:					
					break;
				case 1:
					if(IH.allTerritories[temp].getOwner() == -1)
					{
						IH.allPlayers[0].addTerritory(temp);
						IH.allTerritories[temp].setOwner(0);
						IH.allTerritories[temp].modTroops(1);
						IH.allPlayers[0].decrementReinforcements();
						if(IH.allPlayers[0].getNumberOfTerritories() == 1)
						{
							IH.allPlayers[0].setCapitol(temp);
							IH.allTerritories[temp].setAsCapitol();
						}
						IH.playersTurn++;
					}
					break;
				case 2:
					if(IH.allTerritories[temp].getOwner() == 0)
					{
						IH.allTerritories[temp].modTroops(1);
						IH.allPlayers[0].decrementReinforcements();
						IH.playersTurn++;
						IH.outputMessage = IH.putDataInString(temp);
						IH.displayTimer = 0;
					}
					else
					{
						IH.allTerritories[temp].highlight();
						IH.outputMessage = IH.putDataInString(temp);
						IH.displayTimer = 0;
					}
					break;
				case 3:
					IH.allTerritories[temp].highlight();
					IH.outputMessage = IH.putDataInString(temp);
					IH.displayTimer = 0;
					break;
				case 4:
					break;
				}
				
			}
			else if(IH.event.motion.x < 64 && IH.event.motion.y < 64 && IH.state == 0)
				IH.state++;
		}
	}
}