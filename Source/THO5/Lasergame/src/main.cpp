// simple IR signal detector
#include "hwlib.hpp"
#include "ir/transmitter.hpp"
#include "ir/gameParameters.hpp"
#include "ir/messageLogic.hpp"
#include "ir/receiverController.hpp"

#include "ir/game.hpp"
#include "test.hpp"
#include "InitGameController.hpp"
#include "KeypadController.hpp"
#include "RunGameController.hpp"
#include "SpeakerController.hpp"
#include "GameParamsController.hpp"
#include <array>

#include "rtos.hpp"
#include "OLEDBoundary.hpp"
#include "FormattedGameStats.hpp"
//#include "TestTask.hpp"

int main( void ){
   
   /***
    * 
	* 
	* ToDo:
	* Playerinformation has to be shared between multiple objects (tasks):
	* Own player information is set in initgamecontroller for host and gameparamscontroller for clients.
	* This is then passed to the RunGameController.
	* When a shot is being launched, 
	* the client player_id and weapon_id within playerInformation (within Player <-- current player) is sent over the IR.
	* 
	* For retrieval of other players shooting, the Playerinformation is being used by the receiver.
	* This is then passed to the RunGameController where the damage taken is taken care of by the Player object.
	* playerInformation contains data for over IR. Player contains logic for gameplay implementation.
	**/
   	
	// kill the watchdog
  	WDT->WDT_MR = WDT_MR_WDDIS;


	namespace target = hwlib::target;
   	//Wait for hell freezing over
	hwlib::wait_ms(1000);
	
   	//Set button and led pins
	auto button = hwlib::target::pin_in(hwlib::target::pins::d22);
	auto led = hwlib::target::pin_out(hwlib::target::pins::d24);
   
	//Set IR receiver pins
    auto vcc = hwlib::target::pin_out(hwlib::target::pins::d10);
    auto gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
    auto data = hwlib::target::pin_in(hwlib::target::pins::d8);

    //Set speaker pin
    auto lsp = target::pin_out( target::pins::d7);

    //Set keypad pins
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

	
	//Create a test message and encode it
	messageLogic messageLogic;
    char16_t compiledMessage = messageLogic.encode(1,1);
	
	//Set the playerInformation according to the message
    playerInformation playerInformation;
    playerInformation.setCompiledBits(compiledMessage);
	
	//Set receivercontroller
    auto receiver = receiverController(data,gnd,vcc,messageLogic,0);
	
	struct IREntity IRE(button,led,playerInformation,messageLogic,receiver);
	
	KeypadController kpC = KeypadController(keypad, 15);
	OLEDBoundary oledBoundary{ 10 };
	auto sC = SpeakerController(lsp, 14);
	auto rGC = RunGameController(kpC, sC, oledBoundary, IRE, playerInformation, 12);
	auto iGC = InitGameController(kpC, &rGC, oledBoundary, playerInformation, IRE, 13);
	auto gPC = GameParamsController(kpC, &iGC, &rGC, oledBoundary, playerInformation, IRE, 16);
	Player testPlayer(playerInformation, rGC);
	FormattedGameStats FGS;
	//std::string lotsofaids;
	FGS.getResultsXml(testPlayer);
	
/*	for(unsigned int i = 0; i < lotsofaids.size() - 1; i++)	{
		buffedasfuck[i] = lotsofaids.at(i);
	}*/
	
	kpC.registerNext(&gPC);
	
	// set IR receiver
	receiver.setReceiveListener(&rGC);
//   TestTask tt{ 2 };
//   tt.setOledBoundary(&oledBoundary);
   //RunGameController runGameController{ 2, oledBoundary };
	//hwlib::cout << buffedasfuck << hwlib::endl;
   rtos::run();
}

