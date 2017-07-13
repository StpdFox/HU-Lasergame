#include "hwlib.hpp"
#include "rtos.hpp"
#include "messageLogic.hpp"

#ifndef RECEIVERCONTROLLER_HPP
#define RECEIVERCONTROLLER_HPP

class receiverController : public rtos::task<>{
private:
	rtos::timer poolReceiverTimer;
	hwlib::pin_in & rPin;
	hwlib::pin_out & gnd;
	hwlib::pin_out & vcc;
	messageLogic & logic;
	
	void main(void) override;
	
public:
	receiverController(hwlib::pin_in & rPin, hwlib::pin_out & gnd, hwlib::pin_out & vcc, messageLogic & logic,unsigned int priority );

	int getStartBit();

	int getBit();

    char16_t getMessage();
};
#endif