#include "hwlib.hpp"
#ifndef GAMEPARAMETERS_HPP
#define GAMEPARAMETERS_HPP

class playerInformation{
private:

	char16_t compiledBits;
public:
	playerInformation():
	compiledBits(0)
	{}
	byte playerID;
	byte weaponID;
	
	int playerHealth = 100;
	
	char16_t getCompiledBits(){
		return compiledBits;
	}

	void setCompiledBits(char16_t value){
		compiledBits = value;
	}
	
	byte getPlayerID(){
		return playerID;
	}
	
	void setPlayerID(byte playerID){
		playerID = playerID;
	}
	
	byte getWeaponID(){
		return weaponID;
	}
	
	void setWeaponID(byte weaponID){
		weaponID = weaponID;
	}
	
	int getPlayerHealth(){
		return playerHealth;
	}
	void setPlayerHealth(int playerHealth){
		playerHealth = playerHealth;
	}
};

class gameInformation{
private:
	int gameTimeRemaning;
	bool gameRunning;
public:
	gameInformation(int gameTimeRemaning):
	gameTimeRemaning(gameTimeRemaning),
	gameRunning(0)
	{}

	int getGameTime(){
		return gameTimeRemaning;
	}
	bool getGameRunning(){
		return gameRunning;
	}

	void setGameTime(int time){
		gameTimeRemaning = time;
	}
	void setGameRunning(bool state){
		gameRunning = state;
	}
};

class hit{
private:
	byte playerID;
	byte weaponID;
public:
	hit(byte playerID,byte weaponID):
	playerID(playerID),
	weaponID(weaponID)
	{}

	byte getPlayerID(){
		return playerID;
	}
	byte getWeaponID(){
		return weaponID;
	}
};

#endif