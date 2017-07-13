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
	byte playerID1;
	byte playerID2;
	byte weaponID1;
	byte weaponID2;
	
	int playerHealth = 100;
	
	char16_t getCompiledBits(){
		return compiledBits;
	}

	void setCompiledBits(char16_t value){
		compiledBits = value;
	}
	
	byte getPlayerID1(){
		return playerID1;
	}
	
	byte getPlayerID2(){
		return playerID2;
	}
	void setPlayerID(byte playerID1, byte playerID2){
		playerID1 = playerID1;
		playerID2 = playerID2;
	}
	
	byte getWeaponID1(){
		return weaponID2;
	}
	
	byte getWeaponID2(){
		return weaponID2;
	}
	void setWeaponID(byte weaponID1, byte weaponID2){
		weaponID1 = weaponID1;
		weaponID2 = weaponID2;
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