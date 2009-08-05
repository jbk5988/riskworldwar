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

bool allTerTaken(Info_Handler & IH)
{
	for(int i = 0; i < NUM_OF_TERRITORIES; ++i)
	{
		if(IH.allTerritories[i].getOwner() == -1)
			return false;
	}
	return true;
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
			looking = false;
		}
	}
}

void drawAll(Info_Handler & IH, char * util)
{
	itoa(SDL_GetTicks() / 1000, util, 10);
	writeText(IH.ascii, &IH.asciiRect, IH.screen, util, IH.screen->w - 400, IH.screen->h - IH.asciiRect.h);
	SDL_FillRect(IH.screen, &IH.consoleRect, 0x000000);
	writeToConsole(IH.ascii, &IH.asciiRect, IH.screen, IH.outputMessage, &IH.consoleRect);
	switch(IH.state)
	{
	case 0:	
		drawATile(IH.buttons, &IH.buttonRect, 0, IH.screen, 0,0);
		break;
	case 1:
		break;
	case 2:
		drawATile(IH.buttons, &IH.buttonRect, 1, IH.screen, 0,0);
		break;
	}
	for(int i = 0; i < NUM_OF_TERRITORIES; ++i)
	{
		drawATile(IH.borders, &IH.borderRect, IH.allTerritories[i].getOwner() + 1, IH.screen, IH.allTerritories[i].getRect().x, IH.allTerritories[i].getRect().y);
		//writeText(IH.ascii, &IH.asciiRect, IH.screen, IH.allTerritories[i].getName().c_str(), IH.allTerritories[i].getRect().x, IH.allTerritories[i].getRect().y);
	}
	for(int i = 0; i < NUM_OF_TERRITORIES; ++i)
	{
		if(IH.allTerritories[i].isHighlighted())
		{
			drawATile(IH.borders, &IH.borderRect, 1, IH.screen, IH.allTerritories[i].getRect().x, IH.allTerritories[i].getRect().y);
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

int checkTerClicked(Info_Handler & IH)
{
	for(int i = 0; i < NUM_OF_TERRITORIES; ++i)
	{
		if( IH.event.motion.x > IH.allTerritories[i].getRect().x && 
			IH.event.motion.x < IH.allTerritories[i].getRect().x + IH.allTerritories[i].getRect().w &&
			IH.event.motion.y > IH.allTerritories[i].getRect().y && 
			IH.event.motion.y < IH.allTerritories[i].getRect().y + IH.allTerritories[i].getRect().h)
			return i;
	}
	return -1;
}

void update(Info_Handler & IH, int msPassed)
{
	IH.displayTimer += msPassed;
	if(IH.displayTimer > IH.displayTimerMax)
	{
		IH.outputMessage = "";
		IH.displayTimer = 0;
	}

	if(IH.state == 1 && !allTerTaken(IH) && IH.playersTurn > 1)
	{
		for(int i = 2; i <= IH.numOfPlayers; ++i)
		{			
			if(!allTerTaken(IH))
				compGrabRandom(IH, IH.playersTurn);	
			IH.playersTurn++;
		}
		IH.playersTurn = 1;
	}
	else if(IH.state == 1 && allTerTaken(IH))
		IH.state++;
}

string putDataInString(Info_Handler & IH, int terr)
{
	char util[256];
	string output = "";
	
	output += IH.allTerritories[terr].getName();
	output += "^nOwner Player: ";
	itoa(IH.allTerritories[terr].getOwner(), util, 10);
	output += util;
	output += "^nTroops: ";
	itoa(IH.allTerritories[terr].getTroops(), util, 10);
	output += util;

	return output;
}

void eventHandler(Info_Handler & IH)
{
	int temp;
	if(IH.event.type == SDL_QUIT)
		IH.keepGoing = false;
	if(IH.event.type == SDL_MOUSEBUTTONDOWN)
	{
		if(IH.event.button.button == SDL_BUTTON_LEFT && IH.playersTurn == 1) 
		{
			for(int i = 0; i < NUM_OF_TERRITORIES; ++i)
				IH.allTerritories[i].dehighlight();
			temp = checkTerClicked(IH);
			if(temp >= 0)
			{
				switch(IH.state)
				{
				case 0:					
					break;
				case 1:
					if(IH.allTerritories[temp].getOwner() == -1)
					{
						IH.allTerritories[temp].setOwner(1);
						IH.allTerritories[temp].modTroops(1);
						IH.playersTurn++;
					}
					break;
				case 2:
					IH.allTerritories[temp].highlight();
					IH.outputMessage = putDataInString(IH, temp);
					IH.displayTimer = 0;
					break;
				case 3:
					break;
				}
				
			}
			else if(IH.event.motion.x < 64 && IH.event.motion.y < 64 && IH.state == 0)
				IH.state++;
		}
	}
}