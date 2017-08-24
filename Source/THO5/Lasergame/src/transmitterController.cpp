#include "transmitterController.hpp"
#include "rtos.hpp"

transmitterController::transmitterController(unsigned int priority) :
 	task{ priority, "sendTask" },
 	irTransmit{},
 	messagePoolMutex{ "messagePoolMutex" },
	messagePoolFlag{ this, "messagePoolFlag" },
 	messagePool{ "messagePool" }
{ }

void transmitterController::main()
{
	for(;;)
	{
 		wait();
		messagePoolMutex.wait();
		char16_t message = messagePool.read();
		messagePoolMutex.signal();
		irTransmit.sendMessage(message);
 	}
}

void transmitterController::sendMessage(char16_t message)
{
	messagePoolMutex.wait();
	messagePool.write(message);
	messagePoolMutex.signal();
	messagePoolFlag.set();
}