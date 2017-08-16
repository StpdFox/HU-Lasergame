/*#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "RunGameController.hpp"
#include "GameParamsController.hpp"
//class RunGameController;
class Player	{
private:
	friend class FormattedGameStats; // They are bff's!
	
public:
bool playerisAlive = true;
	playerInformation& playerInfo;
	RunGameController& parentController;
	
	int healthPoints;
	// Array van 10 elementen (max 10 spelers), waar de key-pair is: hoeveelheid damage, aantal schoten
	std::array<std::pair<uint8_t, uint8_t>, 10> other_players { { 
		std::pair<uint8_t,uint8_t>(0x00, 0), 
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0)} };
		
	Player(playerInformation& playerInfo, RunGameController& parentController);
	//Player(uint8_t playerId, RunGameController& parentController, int life = 100);
	
	void takeDamage(uint8_t, uint8_t);
	void doDamage();
	bool playerIsAlive();
};

#endif // PLAYER_HPP*/
