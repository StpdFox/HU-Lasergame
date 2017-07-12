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
		bool buttonSet = button.get();
		led.set(!buttonSet);
		if(!buttonSet)
		{
			receive.suspend();
			trans.enableFlag();
			sleep(1200*rtos::ms);
		};
		receive.resume();
	};
}*/
