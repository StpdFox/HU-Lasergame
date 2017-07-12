// simple IR signal detector
#include "hwlib.hpp"
#include "ir/transmitter.hpp"
#include "ir/gameParameters.hpp"
#include "ir/messageLogic.hpp"
#include "ir/receiverController.hpp"

#include "game.hpp"

#include "InitGameController.hpp"
#include "KeypadController.hpp"
#include "RunGameController.hpp"
#include "SpeakerController.hpp"
#include <array>

#include "rtos.hpp"
#include "OLEDBoundary.hpp"
//#include "TestTask.hpp"

int main( void ){
   
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms(1000);

   hwlib::target::pin_in button = hwlib::target::pin_in(hwlib::target::pins::d22);
   hwlib::target::pin_out led = hwlib::target::pin_out(hwlib::target::pins::d24);



   namespace target = hwlib::target;

    hwlib::target::pin_out vcc = hwlib::target::pin_out(hwlib::target::pins::d10);
    hwlib::target::pin_out gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
    hwlib::target::pin_in data = hwlib::target::pin_in(hwlib::target::pins::d8);


    messageLogic messageLogic;
    playerInformation playerInformation;
    char16_t compiledMessage = messageLogic.encode(1,1);
    playerInformation.setCompiledBits(compiledMessage);
    auto receiver = receiverController(data,gnd,vcc,messageLogic);
    

    auto game = gameController(button,led,playerInformation,messageLogic,receiver);

   
   namespace target = hwlib::target;
   auto lsp = target::pin_out( target::pins::d7);

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

	KeypadController kpC = KeypadController(keypad, 5);
	OLEDBoundary oledBoundary{ 1 };
	auto sC = SpeakerController(lsp, 4);
	auto rGC = RunGameController(kpC, sC, oledBoundary, 2);
	auto iGC = InitGameController(kpC, &rGC, 3);
	kpC.registerNext(&iGC);
	
   
//   TestTask tt{ 2 };
//   tt.setOledBoundary(&oledBoundary);
   //RunGameController runGameController{ 2, oledBoundary };

   rtos::run();
}

