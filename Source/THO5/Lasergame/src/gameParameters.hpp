///	\file gameParameters.hpp
#ifndef GAMEPARAMETERS_HPP
#define GAMEPARAMETERS_HPP

#include "hwlib.hpp"

/**
 * @class playerInformation
 * @author Matthijs
 * @brief The only instance of this class is used to communicate the player information between tasks. This instance is created in main().
 */
class playerInformation {
public:
	playerInformation() :
		compiledBits{ 0 },
		playerID{ },
		weaponID{ },
		playerHealth{ 100 }
	{}
	
	char16_t getCompiledBits() const {
		return compiledBits;
	}

	void setCompiledBits(char16_t value) {
		this->compiledBits = value;
	}
	
	byte getPlayerID() const {
		return playerID;
	}
	
	void setPlayerID(byte playerID) {
		this->playerID = playerID;
	}
	
	byte getWeaponID() const {
		return weaponID;
	}
	
	void setWeaponID(byte weaponID) {
		this->weaponID = weaponID;
	}
	
	int getPlayerHealth() const {
		return playerHealth;
	}
	
	void setPlayerHealth(int playerHealth) {
		this->playerHealth = playerHealth;
	}
	
private:
	char16_t compiledBits;
	byte playerID;
	byte weaponID;
	int playerHealth;
};

#endif