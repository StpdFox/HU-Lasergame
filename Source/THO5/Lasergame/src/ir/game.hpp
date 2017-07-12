#ifndef GAME_HPP
#define GAME_HPP

/*#include "hwlib.hpp"
#include "rtos.hpp"
#include "transmitterController.hpp"
#include "receiverController.hpp"
#include "messageLogic.hpp"
#include "gameParameters.hpp"*/
/*typedef struct IREntity {
	//auto & button,auto & led,auto & playerInformation,auto & logic,auto & receiver
	hwlib::pin_in & button;
	hwlib::pin_out & led;
	transmitterController trans;
	messageLogic & logic;
	receiverController & receive;
	
	
	IREntity(auto & button,auto & led,auto & playerInformation,auto & logic,auto & receiver) :
	button{button},
	led{led},
	trans{playerInformation,1},
	logic{logic},
	receive{receiver} {}
} &irentity;*/
/*class gameController : public rtos::task<>{
private:
	irentity irE;
	rtos::channel<char16_t,10> receiverMessageChannel;
	hwlib::pin_in & button;
	hwlib::pin_out & led;
	transmitterController trans;
	messageLogic & logic;
	receiverController & receive;
	rtos::channel<char16_t,10> receiverMessageChannel;

	void main(void);
	
public:
<<<<<<< HEAD
	gameController(hwlib::pin_in & button,hwlib::pin_out & led,playerInformation & playerInformation,messageLogic & logic,receiverController & receiver);
=======
	gameController(irentity irE);
>>>>>>> 34791152ee1dfe183272b93c183bd80e0a103de9
	~gameController();


};*/
#endif