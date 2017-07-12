/*#include "game.hpp"

<<<<<<< HEAD
gameController::gameController(hwlib::pin_in & button,hwlib::pin_out & led,playerInformation & playerInformation,messageLogic & logic,receiverController & receiver):
	task{3,"game"},
	button{button},
	led{led},
	trans{playerInformation,1},
	logic{logic},
	receive{receiver},
=======
gameController::gameController(irentity irE):
	task{5,"game"},
	irE{irE},
>>>>>>> 34791152ee1dfe183272b93c183bd80e0a103de9
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
