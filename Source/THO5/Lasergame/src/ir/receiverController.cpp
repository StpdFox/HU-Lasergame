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
			
			if(messageIsValid(message1))
			{
				HWLIB_TRACE << "message1 is valid.";
				rL->receivedMsgstd({ char((message1 & 0xFF00) >> 8), char(message1 & 0xFF) });
			}
			else if(messageIsValid(message2))
			{
				HWLIB_TRACE << "message1 is not valid but message2 is.";
				rL->receivedMsgstd({ char((message2 & 0xFF00) >> 8), char(message2 & 0xFF) });
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

bool receiverController::messageIsValid(uint16_t message)
{
	uint16_t weapon = (message >> 5) & 0x1F;
	uint16_t playerID = (message >> 10) & 0x1F;
	return (weapon ^ playerID) == (message & 0x1F);
}

//int receiverController::getStartBit()
//{
//	if(!rPin.get())
//	{
//		hwlib::wait_us(1200);
//		if(!rPin.get())
//		{
//			hwlib::wait_us(1200);
//			return 1;
//		}
//	}
//	return -1;
//}
//
//int receiverController::getBit()
//{
//	int begin = hwlib::now_us();
//	while(rPin.get())
//	{
//		hwlib::wait_us(400);
//		if(hwlib::now_us() - begin >= 4000)
//		{
//			return -1;
//		}
//	}
//	hwlib::wait_us(1200);
//	if(!rPin.get())
//	{
//		hwlib::wait_us(1200);
//		return 1;
//	}
//	else
//	{
//		hwlib::wait_us(1200);
//		return 0;
//	}
//	return -1;
//}
//
//char16_t receiverController::getMessage()
//{
//	char16_t bitstream = 0;
//	for(int i = 0; i < 15; i++)
//	{
//    	auto bit = getBit();
//        if((bit == 1) || (bit == 0))
//        {
//			bitstream = (bitstream | bit);
//        }
//        else if(bit == -1)
//        {
//        	return -1;
//        }
//        if(i < 14)
//        {
//        	bitstream = bitstream << 1;
//        }          
//    }
//    bitstream = bitstream | (1 << 15);
//    hwlib::cout.base(2);
//    hwlib::cout << "receiving : " << bitstream << "\n";
//    byte x = 5;
//    byte y = 5;
//    hwlib::cout << "byte x = " << (int)x << "\n byte y = " << (int)y << "\n";
//    logic.decode(bitstream,x,y);
//	/**
//	 * 
//	 * 
//	 * */
//	 std::array<char, 2> msg = {{x, y}};
//	rL->receivedMsgstd(msg);
//    hwlib::cout << "byte x = " << (int)x << "\n byte y = " << (int)y << "\n";
//    return bitstream;
//}

void receiverController::setReceiveListener(ReceiveListener* newRL)	{
	rL = newRL;
}