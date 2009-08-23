#pragma once

#include <string>

class Player
{
private:
	string playerName;
	int playerNumber;
	int capitol;
	int numberOfTerritories;
	int numOfTroopsToReinforce;
	int territoriesOwned[NUM_OF_TERRITORIES];
	bool alive;
public:
	Player()
		:playerName("Fred"), playerNumber(-1), capitol(-1),  numberOfTerritories(0), alive(false), numOfTroopsToReinforce(0)
	{
		for(int i = 0; i < NUM_OF_TERRITORIES; ++i)
			territoriesOwned[i] = -1;
	}
	Player(string, int, int, int);
	void setPlayerStats(string, int, int, int, bool);
	void addTerritory(int terrToAdd);
	void loseTerritory(int terrToLose);
	bool ownTerritory(int terrToCheck);

	void setPlayerName(string newName) {playerName = newName;}
	void setPlayerNumber(int newNum) {playerNumber = newNum;}
	void setCapitol(int newCap) {capitol = newCap;}
	void setNumOfTerritories(int newNum) {numberOfTerritories = newNum;}
	void setAlive(bool isAlive) {alive = isAlive;}
	void setReinforce(int newTroops) {numOfTroopsToReinforce = newTroops;}
	void decrementReinforcements() {numOfTroopsToReinforce--;}
	string getPlayerName() {return playerName;}
	int getPlayerNumber() {return playerNumber;}
	int getCapitol() {return capitol;}
	int getNumberOfTerritories() {return numberOfTerritories;}
	int getReinforce() {return numOfTroopsToReinforce;}
	int getTerritory(int terrToGet) {return territoriesOwned[terrToGet];}
	bool getAlive() {return alive;}
};
Player::Player(string name, int number, int cap, int territories)
{
	playerName = name;
	playerNumber = number;
	capitol = cap;
	numberOfTerritories = territories;
	alive = true;
}
void Player::setPlayerStats(string name, int number, int cap, int territories, bool isAlive)
{
	playerName = name;
	playerNumber = number;
	capitol = cap;
	numberOfTerritories = territories;
	alive = isAlive;
}

void Player::addTerritory(int terrToAdd)
{
	territoriesOwned[numberOfTerritories] = terrToAdd;
	numberOfTerritories++;
}

void Player::loseTerritory(int terrToLose)
{
	bool removed = false;
	for(int i = 0; i < numberOfTerritories -1; ++i)
	{
		if(territoriesOwned[i] == terrToLose)
			removed = true;
		if(removed)
			territoriesOwned[i] = territoriesOwned[i + 1];
	}
	if(removed)
		numberOfTerritories--;
}

bool Player::ownTerritory(int terrToCheck)
{
	for(int i = 0; i < numberOfTerritories; ++i)
	{
		if(territoriesOwned[i] == terrToCheck)
			return true;
	}
	return false;
}