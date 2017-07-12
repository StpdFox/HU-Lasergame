#ifndef MESSAGELOGIC_HPP
#define MESSAGELOGIC_HPP

#include "hwlib.hpp"
class messageLogic{
public:
	

	char16_t encode(uint_fast8_t playerID, uint_fast8_t weaponID);

	bool decode(char16_t receivingBits, byte & receivedPlayerID , byte & receivedWeaponID);
};
#endif