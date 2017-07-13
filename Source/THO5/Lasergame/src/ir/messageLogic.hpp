#ifndef MESSAGELOGIC_HPP
#define MESSAGELOGIC_HPP

#include "hwlib.hpp"
/// \author Matthijs Vos
/// \brief This class contains a simple implementation to encode and decode messages that are send over the IR (protocol).
///
class messageLogic{
public:
	

	char16_t encode(uint_fast8_t playerID, uint_fast8_t weaponID);

	bool decode(char16_t receivingBits, byte & receivedPlayerID , byte & receivedWeaponID);
};
#endif