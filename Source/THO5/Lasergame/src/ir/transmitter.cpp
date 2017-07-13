#include "transmitter.hpp"

void transmitter::sendOne()
{
	sendPin.set(1);
	hwlib::wait_us(1600);
	sendPin.set(0);
	hwlib::wait_us(800);
}
void transmitter::sendZero()
{
	sendPin.set(1);
	hwlib::wait_us(800);
	sendPin.set(0);
	hwlib::wait_us(1600);
}
void transmitter::setZero()
{
	sendPin.set(0);
}
void transmitter::sendMessage(char16_t message)
{
	for(int nMessage = 0; nMessage < 2; nMessage++)
	{	
		
		
		for(int i = 15; i >= 0; i--)
		{
			if((message & (1 <<i)) !=0)
			{
				sendOne();
				hwlib::cout << "1";
			}
			else
			{
				sendZero();
				hwlib::cout << "0";
			}
		}
		
		hwlib::wait_ms(3);
	}
}