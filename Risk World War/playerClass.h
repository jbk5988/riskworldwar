#pragma once

#include <string>

class Player
{
private:
	string playerName;
	int playerNumber;
	int capitol;
	int numberOfTerritories;
public:
	Player(string, int, int, int);
	void setPlayerStats(string, int, int, int);
	void setPlayerName(string);
	void setPlayerNumber(int);
	void setCapitol(int);
	void setNumOfTerritories(int);
	string getPlayerName();
	int getPlayerNumber();
	int getCapitol();
	int getNumberOfTerritories();
};
Player::Player(string name, int number, int cap, int territories)
{
	playerName = name;
	playerNumber = number;
	capitol = cap;
	numberOfTerritories = territories;
}
void Player::setPlayerStats(string name, int number, int cap, int territories);
{
	playerName = name;
	playerNumber = number;
	capitol = cap;
	numberOfTerritories = territories;
}
void Player::setPlayerName(string name)
{
	playerName = name;
}
void Player::setPlayerNumber(int number)
{
	playerNumber = number;
}
void Player::setCapitol(int cap)
{
	capitol = cap;
}
void Player::setNumOfTerritories(int territories)
{
	numberOfTerritories = territories;
}
string Player::getPlayerName()
{
	return playerName;
}
int Player::getPlayerNumber()
{
	return playerNumber;
}
int Player::getCapitol()
{
	return capitol;
}
int Player::getNumberOfTerritories()
{
	return numberOfTerritories;
}
