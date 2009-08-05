#pragma once

#define MAX_CONNECTS    10


class Territory
{
private:
	int troops, owner, index;
	int connected_to[MAX_CONNECTS];
	string name;
	bool capitol, highlighted;
	SDL_Rect rect;
public:
	Territory();

	SDL_Rect getRect() {return rect;}
	string getName() {return name;}
	int getTroops() {return troops;}
	int getOwner() {return owner;}
	bool isCapitol() {return capitol;}
	bool isHighlighted() {return highlighted;}
	int * getConTo() {return connected_to;}

	void setOwner(int newOwner) {owner = newOwner;}	
	void setAsCapitol() {capitol = true;}
	void highlight() {highlighted = true;}
	void dehighlight() {highlighted = false;}
	void setXY(int X, int Y) {rect.x = X; rect.y = Y;}

	void modTroops(int);
	void connectTo(int);
	void setData(string, int, int, int, bool, bool, int[], SDL_Rect);
	bool isConnTo(int);
	void draw();
};

Territory::Territory()
{
	name = "NOT SET";
	troops = 0;
	owner = index = -1;
	for(int i = 0; i < MAX_CONNECTS; ++i)
	{
		connected_to[i] = -1;
	}
	capitol = highlighted = false;
	rect.h = 20;
	rect.w = 20;
	rect.x = 0;
	rect.y = 0;
}

void Territory::modTroops(int modifier)
{
	troops += modifier;
	if(troops < 0)
		troops = 0;
}

void Territory::connectTo(int to)
{
	for(int i = 0; i < MAX_CONNECTS; ++i)
	{
		if(connected_to[i] == -1)
		{
			connected_to[i] = to;
			break;
		}
	}
}

void Territory::setData(string a_name, int a_troops, int a_owner, int a_index, 
						bool a_capitol, bool a_highlighted, int a_connects[], SDL_Rect a_rect)
{
	name = a_name;
	troops = a_troops;
	owner = a_owner;
	index = a_index;
	capitol = a_capitol;
	highlighted = a_highlighted;
	for(int i = 0; i < MAX_CONNECTS; ++i)
		connected_to[i] = a_connects[i];
	rect.w = a_rect.w;
	rect.h = a_rect.h;
	rect.x = a_rect.x;
	rect.y = a_rect.y;
}

bool Territory::isConnTo(int check)
{
	for(int i = 0; i < MAX_CONNECTS; ++i)
	{
		if(connected_to[i] == check)
			return true;
	}
	return false;
}

void Territory::draw()
{

}