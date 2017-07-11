#include "hwlib.hpp"
#include "rtos.hpp"
#include "transmitterController.hpp"
#include "receiverController.hpp"
#include "messageLogic.hpp"
#include "gameParameters.hpp"

#ifndef GAME_HPP
#define GAME_HPP

class gameController : public rtos::task<>{
private:
	hwlib::pin_in & button;
	hwlib::pin_out & led;
	transmitterController trans;
	messageLogic & logic;
	receiverController & receive;
	rtos::channel<char16_t,10> receiverMessageChannel;
	
	void main(void){
		hwlib::cout << "starting gamecontroller\n";
		for(;;){

			bool buttonSet = button.get();
			led.set(!buttonSet);
			if(!buttonSet){
				receive.suspend();
				trans.enableFlag();
				
				sleep(1000);
			};
			release();
			
		};
	};
public:
	gameController(hwlib::pin_in & button,hwlib::pin_out & led,playerInformation & playerInformation,messageLogic & logic,receiverController & receiver):
		task(6,"game"),
		button(button),
		led(led),
		trans(playerInformation),
		logic(logic),
		receive(receiver),
		receiverMessageChannel(this,"receiverMessage")
		{
		
		}

	void write(char16_t value){
        receiverMessageChannel.write(value);
    }
};
#endif