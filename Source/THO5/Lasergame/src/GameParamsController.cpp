///	\file GameParamsController.cpp
/// The GameParamsController file,
/// contains the GameParamsController class implementation only. 
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 12-07-2017

#include "GameParamsController.hpp"
#include "OLEDBoundary.hpp"
#include "ir/transmitterController.hpp"
#include "ir/messageLogic.hpp"

GameParamsController::GameParamsController(KeypadController& kpC, InitGameController* initGameListener, RunGameController* runGameListener, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irEntity, unsigned int priority) : 
	rtos::task<>{ priority, "GameParamsController" },
	state{ STATE::WAITING_FOR_A },
	irEntity{ irEntity },
	playerInfo{ playerInfo },
	oledBoundary{ oledBoundary },
	kpC{kpC},
	msg("keypad char"),
	initGameListener{initGameListener},
	runGameListener{runGameListener},
	irMessageChannel{ this, "irMessageChannel" }
{
	oledBoundary.getStatusMessageField().setLocation({4 *8  , 1 * 8 });
	oledBoundary.getGameTimeField().setLocation({ 7 * 8, 5 * 8 });
	oledBoundary.getPlayerNumberInputField().setLocation({ 4 * 8, 4 * 8 });
	oledBoundary.getFirePowerInputField().setLocation({ 13 * 8, 4 * 8 });
}

GameParamsController::~GameParamsController(){
}
	
void GameParamsController::handleMessageKey(char c) {
	msg.write(c);
}

void GameParamsController::main() {
	initGameListener->suspend(); //TODO verzin mechanisme waarbij initGame en RunGame niet dingen doen (zoals timers laten zien) als dat nog niet moet
	runGameListener->suspend();
	
	hwlib::window_ostream stream{ oledBoundary.getBufferedLCD(), font };	
	
	stream << "\f*--------------*";
	stream << "\n|   Welcome!   |";
	stream << "\n|   Press A    |";
	stream << "\n|              |";
	stream << "\n|              |";
	stream << "\n|              |";
	stream << "\n|              |";
	stream << "\n*--------------*";
	oledBoundary.flush();
	

	for(;;) {
		char c = msg.read();
		KeyConsumer::handleMessageKey(*this, c);
	}
}

void GameParamsController::handleNewMessage(byte & receivedPlayerID , byte & receivedWeaponID)
{
	irMessageChannel.write({ receivedPlayerID, receivedWeaponID });
}

void GameParamsController::waitForCommands()
{
	irMessage message{ irMessageChannel.read() };

	if(message.receivedPlayerID == 0 && message.receivedWeaponID > 0)
	{
		HWLIB_TRACE << "timeCMD received";
		hwlib::window_ostream stream{ oledBoundary.getBufferedLCD(), font };
		stream << "\f\n\nWaiting for the\nstart signal.";
		oledBoundary.flush();
		//TODO schrijf data naar de RunGameController
	}
	
	message = irMessageChannel.read();
	if(message.receivedPlayerID == 0 && message.receivedWeaponID == 0)
	{
		HWLIB_TRACE << "startCMD received";
		//TODO stuur start signaal
		
		hwlib::cout << "To runGame";
		kpC.registerNext(runGameListener);
		runGameListener->resume();
	}
}

void GameParamsController::consumeChar(char c) {
	
	if(state == STATE::WAITING_FOR_A && c == 'A')
	{
		HWLIB_TRACE << "A pressed";
		hwlib::window_ostream stream{ oledBoundary.getStatusMessageField(), font };
		
		stream << "\fEnter\nPlayerID";
		
		oledBoundary.flushParts();
		
		HWLIB_TRACE << "state = INPUTTING_PLAYER_ID";
		state = STATE::INPUTTING_PLAYER_ID;
	}
	else if(state == STATE::WAITING_FOR_B && c == 'B')
	{
		HWLIB_TRACE << "B pressed";
		playerInfo.setPlayerID(playerID);
		hwlib::cout << "playerID: " << int(playerID) << "\n";
		hwlib::window_ostream stream{ oledBoundary.getStatusMessageField(), font };
		
		stream << "\fEnter\nWeaponID";
		oledBoundary.flushParts();
		
		HWLIB_TRACE << "state = INPUTTING_WEAPON_ID";
		state = STATE::INPUTTING_WEAPON_ID;
	}
}

void GameParamsController::consumeHashTag() {
	if(state == STATE::WAITING_FOR_HASHTAG) {
		HWLIB_TRACE << "# pressed";
		playerInfo.setWeaponID(weaponID);
		hwlib::cout << "WeaponDMG: " << int(weaponID) << "\n";
		if(playerID > 0)
		{
			hwlib::window_ostream stream{ oledBoundary.getBufferedLCD(), font };
			stream << "\f\n\nWaiting for the\ngame time.";
			oledBoundary.flush();
			
			HWLIB_TRACE << "state = WAITING_FOR_GAMETIME_CMD";
			state = STATE::WAITING_FOR_COMMANDS;
			
			waitForCommands();
		}
		else
		{
			hwlib::window_ostream stream{ oledBoundary.getBufferedLCD(), font };
			stream << "\f\n\nPress C.";
			oledBoundary.flush();
			
			hwlib::cout << "To initGame";
			kpC.registerNext(initGameListener);
			initGameListener->resume();
			suspend();
		}
	}
}

void GameParamsController::consumeWildcard()
{}

void GameParamsController::consumeDigits(char c) {
	if(state == STATE::INPUTTING_PLAYER_ID) {
		playerID = c - '0';
		HWLIB_TRACE << c - '0';
		hwlib::window_ostream playerIDStream{ oledBoundary.getPlayerNumberInputField(), font };
		playerIDStream << "\f" << c;
		oledBoundary.flushParts();
		HWLIB_TRACE << "state = WAITING_FOR_B";
		state = STATE::WAITING_FOR_B;
	}
	else if(state == STATE::INPUTTING_WEAPON_ID) {
		weaponID = c - '0';
		hwlib::window_ostream weaponIDStream{ oledBoundary.getFirePowerInputField(), font };
		weaponIDStream << "\f" << c;
		oledBoundary.flushParts();
		HWLIB_TRACE << "state = WAITING_FOR_HASHTAG";
		state = STATE::WAITING_FOR_HASHTAG;
	}
}