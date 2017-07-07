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
 			 hwlib::cout << "sendmessageTest\n";
 			sendMessage();
 		}
 	}

 	void sendMessage(){
 		auto message = messageToSend.getCompiledBits();
 		hwlib::cout << message << "\n";
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