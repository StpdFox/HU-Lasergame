#include "transmitterController.hpp"

transmitterController::transmitterController(playerInformation & playerinformation,unsigned int priority ) :
 	task(priority, "sendTask"),
 	irTransmit(),
 	messageToSend(playerinformation),
 	sendMessageFlag(this, "sendMessageFlag")
 	{
 		
 	}

void transmitterController::main()
{
	for(;;)
	{
 		wait(sendMessageFlag);
 		sendMessage();
 	}
}

void transmitterController::sendMessage()
{
	auto message = messageToSend.getCompiledBits();
	
 	irTransmit.sendMessage(message);
}

void transmitterController::enableFlag()
{
	sendMessageFlag.set();
}