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
	oledBoundary.getPlayerNumberInputField().setLocation({ 12 * 8, 5 * 8 });
	oledBoundary.getFirePowerInputField().setLocation({ 13 * 8, 5 * 8 });
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
	stream << "\f\n\nPress A";
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

bool GameParamsController::validatePlayerIDInput() {
	if(    commandCode[0] >= '0' && commandCode[0] <= '9' 
		&& commandCode[1] >= '0' && commandCode[1] <= '9') {
		playerID = (commandCode[0] - '0')*10 + (commandCode[1] - '0');
		return playerID >= 1 && playerID <= 31;
	}
	return false;
}

bool GameParamsController::validateWeaponIDInput() {
	if(    commandCode[0] >= '0' && commandCode[0] <= '9' 
		&& commandCode[1] >= '0' && commandCode[1] <= '9') {
		weaponID = (commandCode[0] - '0')*10 + (commandCode[1] - '0');
		return weaponID >= 0 && weaponID <= 31;
	}
	return false;
}

void GameParamsController::initNewCommand()   {
	commandCode[0] = 0;
	commandCode[1] = 0;
	commandCount = 0;
}

void GameParamsController::waitForCommands()
{
	irMessage message{ irMessageChannel.read() };
	if(message.receivedPlayerID == 0 && message.receivedWeaponID > 0)
	{
		HWLIB_TRACE << "timeCMD received";
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
		initNewCommand();
		
		hwlib::window_ostream stream{ oledBoundary.getBufferedLCD(), font };
		stream << "\f\n\n\nEnter your\nplayer No.: __\nEnd with B";
		oledBoundary.flush();
		
		HWLIB_TRACE << "state = INPUTTING_PLAYER_ID";
		state = STATE::INPUTTING_PLAYER_ID;
	}
	else if(state == STATE::WAITING_FOR_B && c == 'B')
	{
		HWLIB_TRACE << "B pressed";
		if(!validatePlayerIDInput())
		{
			initNewCommand();
			hwlib::window_ostream playerIDStream{ oledBoundary.getPlayerNumberInputField(), font };
			playerIDStream << "\f__";
			oledBoundary.flushParts();
			HWLIB_TRACE << "state = INPUTTING_PLAYER_ID";
			state = STATE::INPUTTING_PLAYER_ID;
		}
		else
		{
			playerInfo.setPlayerID(commandCode[0], commandCode[1]);
			playerID = (commandCode[0] - '0')*10 + (commandCode[1] - '0');
			hwlib::cout << "playerID: " << playerID << "\n";
			
			initNewCommand();
			
			hwlib::window_ostream stream{ oledBoundary.getBufferedLCD(), font };
			stream << "\fWelcome\nplayer " << playerID << "\n\nEnter your\nweapon type: __\nEnd with #";
			oledBoundary.flush();
			
			HWLIB_TRACE << "state = INPUTTING_WEAPON_ID";
			state = STATE::INPUTTING_WEAPON_ID;
		}
	}
}

void GameParamsController::consumeHashTag() {
	if(state == STATE::WAITING_FOR_HASHTAG) {
		HWLIB_TRACE << "# pressed";
		if(!validateWeaponIDInput())
		{
			initNewCommand();
			hwlib::window_ostream weaponIDStream{ oledBoundary.getFirePowerInputField(), font };
			weaponIDStream << "\f__";
			oledBoundary.flushParts();
			HWLIB_TRACE << "state = INPUTTING_WEAPON_ID";
			state = STATE::INPUTTING_WEAPON_ID;
		}
		else
		{
			playerInfo.setWeaponID(commandCode[0], commandCode[1]);
			weaponID = (commandCode[0] - '0')*10 + (commandCode[1] - '0');
			hwlib::cout << "WeaponDMG: " << weaponID << "\n";
			
			HWLIB_TRACE << "state = WAITING_FOR_GAMETIME_CMD";
			state = STATE::WAITING_FOR_COMMANDS;
			if(playerID > 0)
			{
				waitForCommands();
			}
			else
			{
				hwlib::cout << "To initGame";
				kpC.registerNext(initGameListener);
				initGameListener->resume();
			}
		}
	}
}

void GameParamsController::consumeWildcard()
{}

void GameParamsController::consumeDigits(char c) {
	if(state == STATE::INPUTTING_PLAYER_ID) {
		commandCode[commandCount++] = c;
		hwlib::window_ostream playerIDStream{ oledBoundary.getPlayerNumberInputField(), font };
		if(commandCount == 1)
		{
			playerIDStream << "\f" << c << "_";
			commandCode[commandCount++] = c;
		}
		else if(commandCount == 2)
		{
			playerIDStream << "\f" << commandCode[0] << c;
			commandCode[commandCount++] = c;
			HWLIB_TRACE << "state = WAITING_FOR_B";
			state = STATE::WAITING_FOR_B;
		}
	}
	else if(state == STATE::INPUTTING_WEAPON_ID) {
		commandCode[commandCount++] = c;
		hwlib::window_ostream weaponIDStream{ oledBoundary.getFirePowerInputField(), font };
		if(commandCount == 1)
		{
			weaponIDStream << "\f" << c << "_";
			commandCode[commandCount++] = c;
		}
		else if(commandCount == 2)
		{
			weaponIDStream << "\f" << commandCode[0] << c;
			commandCode[commandCount++] = c;
			HWLIB_TRACE << "state = WAITING_FOR_HASHTAG";
			state = STATE::WAITING_FOR_HASHTAG;
		}
	}
	oledBoundary.flushParts();
}