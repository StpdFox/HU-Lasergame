///	\file receiverController.cpp
/// The receiverController file,
/// contains the receiverController class implementation only. 
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 14-07-2017


#include "receiverController.hpp"
#include "defines.h"

receiverController::receiverController(hwlib::pin_in & rPin, hwlib::pin_out & gnd, hwlib::pin_out & vcc, messageLogic & logic,unsigned int priority):
	task(priority,"receiverController"),
	poolReceiverTimer(this,"poolReceiverTimer"),
	rPin(rPin),
	gnd(gnd),
	vcc(vcc),
	logic(logic)
{
	gnd.set(0);
	vcc.set(1);
}
	
void receiverController::main()
{
	//test met andere hw. test met oude versie rtos. isoleer blinky in apart project.
	while(true)
	{
		sleep_non_block(400 * rtos::us);
		if(readBit() == 1)
		{
			uint16_t message1 = readMessage();
			if(message1 == uint16_t(~0))
			{
				HWLIB_TRACE << "message1 invalid!" << hwlib::bin << message1;
				continue;
			}
			
			//wait for 2e start bit
			block_wait(3000 * rtos::us);
			if(readBit(1000, 200) != 1)
			{
				HWLIB_TRACE << "2e startbit invalid!";
				continue;
			}
			
			uint16_t message2 = readMessage();
			if(message2 == uint16_t(~0))
			{
				HWLIB_TRACE << "message2 invalid!" << hwlib::bin << message2;
				continue;
			}
			
			HWLIB_TRACE << hwlib::bin << message1 << " " << hwlib::bin << message2;
			
			byte playerID, weaponID;
			if(logic.decode(message1, playerID, weaponID))
			{
				HWLIB_TRACE << "message1 is valid.";
				rL->receivedMsgstd({ playerID, weaponID });
			}
			else if(logic.decode(message2, playerID, weaponID))
			{
				HWLIB_TRACE << "message1 is not valid but message2 is.";
				rL->receivedMsgstd({ playerID, weaponID });
			}
			else
			{
				HWLIB_TRACE << "both messages aren't valid!";
			}
		}
	}
}

uint16_t receiverController::readMessage()
{
	uint16_t message = 0x8000;
	for(int i = 14; i >= 0; i--)
	{
		uint16_t bit = readBit(4000, 400);
		if(bit == uint16_t(~0))
		{
			HWLIB_TRACE << "bit " << i << " invalid!";
			return ~0;
		}
		message |= bit << i;
	}
	return message;
}

uint16_t receiverController::readBit(unsigned int maxDelayUs, unsigned int intervalUs)
{
	auto start = hwlib::now_us();
	while(rPin.get())
	{
		if(hwlib::now_us() - start >= maxDelayUs)
			return ~0; //return max value
		block_wait(intervalUs * rtos::us);
	}
	
	block_wait(800 * rtos::us);
	uint16_t bit = !rPin.get();
	block_wait(1600 * rtos::us);
	return bit;
}

void receiverController::setReceiveListener(ReceiveListener* newRL)	{
	rL = newRL;
}