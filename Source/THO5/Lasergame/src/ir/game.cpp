/*#include "game.hpp"

gameController::gameController(irentity irE):
	task{5,"game"},
	irE{irE},
	receiverMessageChannel(this,"receiverMessage")
	{}
	
gameController::~gameController()
{}

void gameController::main()
{
	hwlib::cout << "starting gamecontroller\n";
	for(;;)
	{
		bool buttonSet = irE.button.get();
		irE.led.set(!buttonSet);
		if(!buttonSet)
		{
			irE.receive.suspend();
			irE.trans.enableFlag();
			sleep(1200*rtos::ms);
		};
		irE.receive.resume();
	};
}*/
