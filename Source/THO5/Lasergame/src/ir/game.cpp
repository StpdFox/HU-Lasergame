#include "game.hpp"

gameController::gameController(hwlib::pin_in & button,hwlib::pin_out & led,playerInformation & playerInformation,messageLogic & logic,receiverController & receiver):
	task{3,"game"},
	button{button},
	led{led},
	trans{playerInformation,1},
	logic{logic},
	receive{receiver},
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
}
