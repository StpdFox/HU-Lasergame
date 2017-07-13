#include "receiverController.hpp"

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
	while(1)
	{
		poolReceiverTimer.set(400*rtos::us);
		wait(poolReceiverTimer);
		if(getStartBit() != -1)
		{
			
    		hwlib::cout.base(2);
   			
    		hwlib::cout <<getMessage() << "\n";    		
			
			sleep(1200*rtos::ms);
		}
	}
}

int receiverController::getStartBit()
{
	if(!rPin.get())
	{
		hwlib::wait_us(1200);
		if(!rPin.get())
		{
			hwlib::wait_us(1200);
			return 1;
		}
	}
	return -1;
}

int receiverController::getBit()
{
	int begin = hwlib::now_us();
	while(rPin.get())
	{
		sleep(100 * rtos::us);
		if(begin-hwlib::now_us() >= 4000)
		{
			return -1;
		}
	}
	sleep(1200*rtos::us);
	if(!rPin.get())
	{
		
		sleep(1200*rtos::us);
		return 1;
	}
	else
	{
		
		sleep(1200*rtos::us);
		return 0;
	}
	return -1;
}

char16_t receiverController::getMessage()
{
	char16_t bitstream = 0;
	
	for(int i = 0; i < 15; i++)
	{
    	auto bit = getBit();
        if((bit == 1) || (bit == 0))
        {
			bitstream = (bitstream | bit);
        }
        else if(bit == -1)
        {
        	return -1;
        }
        if(i < 14)
        {
        	bitstream = bitstream << 1;
        }          	
    }
    bitstream = bitstream | (1 << 15);
    return bitstream;
}