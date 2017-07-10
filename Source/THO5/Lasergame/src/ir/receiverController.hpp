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
	void main(void){
		while(1){
			poolReceiverTimer.set(400*rtos::us);
			wait(poolReceiverTimer);
			// hwlib::cout << "reveicer\n";
			if(getStartBit() != -1){
				getMessage();
				sleep(400*rtos::ms);

			}
		}
	}
public:
	receiverController(hwlib::pin_in & rPin, hwlib::pin_out & gnd, hwlib::pin_out & vcc, messageLogic & logic ):
	task(0,"receiverController"),
	poolReceiverTimer(this,"poolReceiverTimer"),
	rPin(rPin),
	gnd(gnd),
	vcc(vcc),
	logic(logic)
	{
		gnd.set(0);
		vcc.set(1);
	}

	int getStartBit(){
		if(!rPin.get()){
			hwlib::wait_us(1100);
			if(!rPin.get()){
				hwlib:cout<<  1;
				hwlib::wait_us(700);
				return 1;
			}else{
				hwlib::wait_us(700);
				return 0;
			}
		}
		return -1;
	}

	int getBit(){
		int begin = hwlib::now_us();
		while(rPin.get()){
			sleep(100 * rtos::us);
			if(begin-hwlib::now_us() >= 4000){
				return -1;
			}
		}
            sleep(1100*rtos::us);
            if(!rPin.get()){
                sleep(700*rtos::us);
               

                return 1;
            }else{
                sleep(700*rtos::us);
               

                return 0;
            }
        return -1;
    }
    char16_t getMessage(){
        char16_t bitstream = 0;
        bitstream = bitstream << 1;
        for(int i = 0; i < 15; i++){
            auto bit = getBit();
            if((bit == 1) || (bit == 0)){
            bitstream = (bitstream | bit);
            }else if(bit == -1){
                return -1;
            }
            if(i < 14){
                bitstream = bitstream << 1;
            }          
        }
        bitstream = bitstream | (1 << 15);
       hwlib::cout.base(2);
       hwlib::cout << bitstream << "\n";
       return bitstream;
    }
};
#endif