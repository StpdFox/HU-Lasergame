#ifndef TRANSMIT_HPP
#define TRANSMIT_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "gameParameters.hpp"
#include "transmitter.hpp"
/***
 * 
 * ToDo:
 * Volgens mij kan de transmitterController geen andere informatie oversturen. 
 * De classess gameinformation en hit kunnen niet gebruikt worden op deze manier.
 * Een oplossing zou zijn om ze allemaal te laten inheriten van een base class IRmessage of iets dergelijks.
 * Een andere oplossing is door alleen een setmessage aan te maken hier en die zoekt dan uit wat voor object gemaakt moet worden.
 * Derde oplossing is door gewoon een ruwe char array op te slaan. Zelf prefereer ik de eerste.
 * 
 * Probeer het morgen uit te vogelen maar weet niet zeker of ik tijd heb. 
 * 
 * */
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