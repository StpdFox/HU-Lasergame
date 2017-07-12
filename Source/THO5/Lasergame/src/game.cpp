#include "game.hpp"

gameController::gameController(auto & button,auto & led,auto & playerInformation,auto & logic,auto & receiver):
	task{5,"game"},
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
