// simple IR signal detector

#include "hwlib.hpp"
#include "ir/transmitterController.hpp"
#include "ir/transmitter.hpp"
#include "ir/gameParameters.hpp"
#include "ir/messageLogic.hpp"
#include "ir/receiverController.hpp"

#include "InitGameController.hpp"
#include "keypadhandler.hpp"
#include "RunGameController.hpp"
#include <array>

#include "rtos.hpp"

int main( void ){
   
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms(1000);

   namespace target = hwlib::target;
   
	auto out0 = target::pin_oc( target::pins::a0 );
	auto out1 = target::pin_oc( target::pins::a1 );
	auto out2 = target::pin_oc( target::pins::a2 );
	auto out3 = target::pin_oc( target::pins::a3 );

	auto in0  = target::pin_in( target::pins::a4 );
	auto in1  = target::pin_in( target::pins::a5 );
	auto in2  = target::pin_in( target::pins::a6 );
	auto in3  = target::pin_in( target::pins::a7 );

	auto out_port = hwlib::port_oc_from_pins( out0, out1, out2, out3 );
	auto in_port  = hwlib::port_in_from_pins( in0,  in1,  in2,  in3  );
	auto matrix   = hwlib::matrix_of_switches( out_port, in_port );
	auto keypad   = hwlib::keypad< 16 >( matrix, "123A456B789C*0#D" );

	KeypadHandler kpH = KeypadHandler(keypad);
	auto rGC = RunGameController(kpH);
	auto iGC = InitGameController(kpH, &rGC);
	kpH.registerNext(&iGC);
	
   /* hwlib::target::pin_out vcc = hwlib::target::pin_out(hwlib::target::pins::d10);
    hwlib::target::pin_out gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
    hwlib::target::pin_in data = hwlib::target::pin_in(hwlib::target::pins::d8);

    auto receiver = receiverController(data,gnd,vcc);
 
   
   messageLogic messageLogic;
   playerInformation playerInformation;
  
   transmitterController transmitterController(playerInformation);
   transmitterController.enableFlag();*/

   rtos::run();
}

