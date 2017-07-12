#ifndef GAME_HPP
#define GAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "transmitterController.hpp"
#include "receiverController.hpp"
#include "messageLogic.hpp"
#include "gameParameters.hpp"


class gameController : public rtos::task<>{
private:
	hwlib::pin_in & button;
	hwlib::pin_out & led;
	transmitterController trans;
	messageLogic & logic;
	receiverController & receive;
	rtos::channel<char16_t,10> receiverMessageChannel;

	void main(void);
	
public:
	gameController(hwlib::pin_in & button,hwlib::pin_out & led,playerInformation & playerInformation,messageLogic & logic,receiverController & receiver);
	~gameController();


};
#endif