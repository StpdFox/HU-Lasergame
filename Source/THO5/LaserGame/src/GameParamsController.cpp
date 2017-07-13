///	\file GameParamsController.cpp
/// The GameParamsController file,
/// contains the GameParamsController class implementation only. 
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 12-07-2017

#include "GameParamsController.hpp"

GameParamsController::GameParamsController(KeypadController& kpC, InitGameController* initGameListener, RunGameController* runGameListener, unsigned int priority) : 
rtos::task<>{ priority, "GameParamsController" }, kpC{kpC} , msg("keypad char"), initGameListener{initGameListener}, runGameListener{runGameListener}{
	
}

GameParamsController::~GameParamsController(){
}
	
void GameParamsController::handleMessageKey(char c)  {
	msg.write(c);
}

void GameParamsController::validateCommand()  {
	if(commandCode[0] >= '0' && commandCode[0] <= '9' 
		&& commandCode[1] >= '0' && commandCode[1] <= '9')  {
		if(id){
			playerID1 = commandCode[0];
			playerID2 = commandCode[1];
			playerInfo.setPlayerID(playerID1, playerID2);
			playerID = (commandCode[0] - '0')*10 + (commandCode[1] - '0');
			hwlib::cout << "playerID: " << playerID << "\n";
			id = false;
			initNewCommand();
		}
		else{
			weaponID1 = commandCode[0];
			weaponID2 = commandCode[1];
			playerInfo.setWeaponID(weaponID1, weaponID2);
			weaponID = (commandCode[0] - '0')*10 + (commandCode[1] - '0');
			hwlib::cout << "WeaponDMG: " << weaponID<< "\n";
			id = true;
			initNewCommand();
		}
	}
}

void GameParamsController::initNewCommand()   {
	commandCode[0] = 0;
	commandCode[1] = 0;
	commandCount = 0;
}

void GameParamsController::main()	{
	initGameListener->suspend();
	runGameListener->suspend();
	for(;;) {
		char c = msg.read();
		KeyConsumer::handleMessageKey(*this, c);
		//parseKeypad(s);
		
		if(playerID == 1 && id){
			hwlib::cout << "To initGame";
			kpC.registerNext(initGameListener);
			initGameListener->resume();
		}
		else if(playerID > 1 && id){ //if (IRmessageStart.received)
			hwlib::cout << "To runGame";
			kpC.registerNext(runGameListener);
			runGameListener->resume();
		}
	}
}

void GameParamsController::consumeChar(char c) {	}
void GameParamsController::consumeHashTag() {
	validateCommand();
}
void GameParamsController::consumeWildcard() {}
void GameParamsController::consumeDigits(char c) {
	if(commandCount <= COMMANDSIZE -1)    {
		commandCode[commandCount++] = c;
	}
}