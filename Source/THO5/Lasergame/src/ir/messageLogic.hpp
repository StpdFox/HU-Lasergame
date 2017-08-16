///	\file messageLogic.hpp
/// The messageLogic file,
/// Contains the messageLogic class declaration.
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 14-07-2017

#ifndef MESSAGELOGIC_HPP
#define MESSAGELOGIC_HPP

#include "hwlib.hpp"
/// \author Matthijs Vos
/// \brief This class contains a simple implementation to encode and decode messages that are send over the IR (protocol).
class messageLogic{
public:
	/// \author Matthijs Vos
	/// \brief Encodes a starting message with playerID and weaponID to send over the IR (protocol).
	/// \param uint_fast8_t playerID ID of the player that is sending the message.
	/// \param uint_fast8_t weaponID weaponID of th weapon firing.
	/// \return char16_t encoded bitstream that can be send correctly over the IR.
	char16_t encode(uint8_t playerID, uint8_t weaponID);

	/// \author Matthijs Vos
	/// \brief Decodes a message that has been received by the IR receiver.
	/// \param char16_t receivingBits The bitstream that has been received and needs to be decoded.
	/// \param byte& receivedPlayerID The playerid of the player that the message has been sent from.
	/// \param byte& receivedWeaponID The weaponid of the weapon that the message has been sent from.
	/// \return bool Returns true if message has been decoded succesfully otherwise false.
	bool decode(char16_t receivingBits, byte & receivedPlayerID , byte & receivedWeaponID);
};
#endif