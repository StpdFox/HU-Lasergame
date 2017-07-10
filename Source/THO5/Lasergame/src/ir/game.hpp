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
	rtos::channel<char16_t,10> receiverMessageChannel;
	
	void main(void){


		hwlib::cout << "starting gamecontroller\n";
		for(;;){
			if(! button.get()){
				led.set(! button.get());
				trans.enableFlag();
				sleep(500);
			}else{
				led.set(! button.get());
			};
		};
	};
public:
	gameController(hwlib::pin_in & button,hwlib::pin_out & led,playerInformation & playerInformation,messageLogic & logic):
		task(6,"game"),
		button(button),
		led(led),
		trans(playerInformation),
		logic(logic),
		receiverMessageChannel(this,"receiverMessage")
		{
		
		}

	void write(char16_t value){
        receiverMessageChannel.write(value);
    }
};
#endif