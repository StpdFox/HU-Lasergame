#include "transmitter.hpp"
#include "rtos.hpp"
#include "defines.h"

void transmitter::sendOne()
{
	sendPin.set(1);
	block_wait(1600 * rtos::us);
	sendPin.set(0);
	block_wait(800 * rtos::us);
}
void transmitter::sendZero()
{
	sendPin.set(1);
	block_wait(800 * rtos::us);
	sendPin.set(0);
	block_wait(1600 * rtos::us);
}
void transmitter::setZero()
{
	sendPin.set(0);
}
void transmitter::sendMessage(char16_t message)
{
	hwlib::cout.base(2);
	HWLIB_TRACE << message << "\n";
	for(int nMessage = 0; nMessage < 2; nMessage++)
	{
		for(int i = 15; i >= 0; i--)
		{
			if((message & (1 <<i)) !=0)
			{
				sendOne();
				//hwlib::cout << "1";
			}
			else
			{
				sendZero();
				//hwlib::cout << "0";
			}
		}
		
		sleep_non_block(3 * rtos::ms);
	}
}