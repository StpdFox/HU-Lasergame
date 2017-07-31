#ifndef GAMEPARAMETERS_HPP
#define GAMEPARAMETERS_HPP

#include "hwlib.hpp"

class playerInformation{
private:
	char16_t compiledBits;
	int playerHealth = 100;
	byte playerID;
	byte weaponID;
	
public:
	playerInformation():
	compiledBits(0)
	{}
	
	// health is hier niet nodig? deze klasse wordt alleen gebruikt om data over te sturen.
	// Bevat alleen constante die na 1x setten niet meer veranderen? Dus playerID, weaponID.
	// in Player (rungamecontroller.hpp / cpp) zit het echte player object,
	// waar logica aan verbonden is over de implementatie binnen het spel.
	//int playerHealth = 100;
	
	char16_t getCompiledBits(){
		return compiledBits;
	}

	void setCompiledBits(char16_t value){
		this->compiledBits = value;
	}
	
	byte getPlayerID(){
		return playerID;
	}
	
	void setPlayerID(byte playerID){
		this->playerID = playerID;
	}
	
	byte getWeaponID(){
		return weaponID;
	}
	
	void setWeaponID(byte weaponID){
		this->weaponID = weaponID;
	}
	
	int getPlayerHealth(){
		return playerHealth;
	}
	void setPlayerHealth(int playerHealth){
		this->playerHealth = playerHealth;
	}
};

class gameInformation{
private:
	int gameTimeRemaining;
	bool gameRunning;
public:
	gameInformation(int gameTimeRemaining):
	gameTimeRemaining(gameTimeRemaining),
	gameRunning(0)
	{}

	int getGameTime(){
		return gameTimeRemaining;
	}
	bool getGameRunning(){
		return gameRunning;
	}

	void setGameTime(int time){
		gameTimeRemaining = time;
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