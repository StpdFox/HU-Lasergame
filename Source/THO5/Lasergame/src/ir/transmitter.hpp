#include "hwlib.hpp"
#include "gameParameters.hpp"
#include "rtos.hpp"

#ifndef TRANSMITTER_HPP
#define TRANSMITTER_HPP
class transmitter{
private:
	hwlib::target::d2_36kHz sendPin;

public:
	void sendOne(){

		sendPin.set(1);
		hwlib::wait_us(1600);
		sendPin.set(0);
		hwlib::wait_us(800);
	}
	void sendZero(){
		sendPin.set(1);
		hwlib::wait_us(800);
		sendPin.set(0);
		hwlib::wait_us(1600);
	}
	void setZero(){
		sendPin.set(0);
	}
	void sendMessage(char16_t message){
		for(int nMessage = 0; nMessage < 2; nMessage++){
			for(int i = 15; i >= 0; i--){
				if((message & (1 <<i)) !=0){
					sendOne();
				}
				else{
					sendZero();
				}
			}
			setZero();
			hwlib::wait_ms(3);
		}
	}
};
#endif