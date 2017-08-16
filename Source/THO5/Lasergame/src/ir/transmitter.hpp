#include "hwlib.hpp"
#include "gameParameters.hpp"
#include "rtos.hpp"

#ifndef TRANSMITTER_HPP
#define TRANSMITTER_HPP
class transmitter{
private:
	hwlib::target::d2_36kHz sendPin;

public:
	void sendOne();
	void sendZero();
	void setZero();
	void sendMessage(char16_t message);
};
#endif