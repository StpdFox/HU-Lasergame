#ifndef TRANSMIT_HPP
#define TRANSMIT_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "gameParameters.hpp"
#include "transmitter.hpp"

class transmitterController : public rtos::task<>{
private:
 	transmitter irTransmit;
 	playerInformation & messageToSend;
 	rtos::flag sendMessageFlag;

 	void main();

 	void sendMessage();

public:
	transmitterController(playerInformation & playerInformation,unsigned int priority);

 	void enableFlag();
 };
 #endif