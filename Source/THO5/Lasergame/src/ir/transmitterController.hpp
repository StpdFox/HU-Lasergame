#include "hwlib.hpp"
#include "rtos.hpp"
#include "gameParameters.hpp"
#include "transmitter.hpp"

class transmitterController : public rtos::task<>{
private:
 	transmitter irTransmit;
 	playerInformation & messageToSend;
 	rtos::flag sendMessageFlag;

 	void main(){
 		for(;;){
 			wait(sendMessageFlag);
 			
 			sendMessage();
 		}
 	}

 	void sendMessage(){
 		auto message = messageToSend.getCompiledBits();
 		
 		irTransmit.sendMessage(message);
 	}

public:
	transmitterController(playerInformation & playerInformation):
 		task(3, "sendTask"),
 		irTransmit(),
 		messageToSend(playerInformation),
 		sendMessageFlag(this, "sendMessageFlag")
 		{}

 		void enableFlag(){
 			
 			sendMessageFlag.set();
 		}
 };