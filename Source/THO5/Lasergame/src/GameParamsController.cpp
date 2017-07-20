///	\file GameParamsController.cpp
/// The GameParamsController file,
/// contains the GameParamsController class implementation only. 
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 12-07-2017

#include "GameParamsController.hpp"
#include "OLEDBoundary.hpp"
#include "ReceiveListener.hpp"
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
	oledBoundary.getConfirmMessageField().setLocation({2 * 8, 6 * 8});
	oledBoundary.getGameTimeField().setLocation({ 7 * 8, 5 * 8 });
	oledBoundary.getPlayerNumberInputField().setLocation({ 4 * 8, 4 * 8 });
	oledBoundary.getFirePowerInputField().setLocation({ 4 * 8, 4 * 8 });
}

GameParamsController::~GameParamsController(){
}
	
void GameParamsController::handleMessageKey(char c) {
	msg.write(c);
}

void GameParamsController::receivedMsgstd(std::array<char, 2> msg)
{
	irMessageChannel.write({ msg[0], msg[1] });
}

void GameParamsController::main() {
	initGameListener->suspend(); //TODO verzin mechanisme waarbij initGame en RunGame niet dingen doen (zoals timers laten zien) als dat nog niet moet
	runGameListener->suspend();
	
	irEntity.receive.setReceiveListener(this);
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

void GameParamsController::waitForCommands()
{
	irMessage message;
	do {
		message = irMessageChannel.read();
	} while(!(message.receivedPlayerID == 0 && message.receivedWeaponID > 0));
	
	HWLIB_TRACE << "timeCMD received: " << int(message.receivedWeaponID);
	hwlib::window_ostream stream{ oledBoundary.getStatusMessageField(), font };
	stream << "\fWaiting for start";
	oledBoundary.flushParts();
	//TODO schrijf data naar de RunGameController
	
	do {
		message = irMessageChannel.read();
	} while(!(message.receivedPlayerID == 0 && message.receivedWeaponID == 0));
	HWLIB_TRACE << "startCMD received";
	//TODO stuur start signaal
	
	HWLIB_TRACE << "To runGame";
	kpC.registerNext(runGameListener);
	runGameListener->resume();
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
		hwlib::window_ostream pIDstream{oledBoundary.getPlayerNumberInputField(),font};
		pIDstream << "\f ";
		hwlib::window_ostream confirmStream{oledBoundary.getConfirmMessageField(), font};
		confirmStream << "\f        ";
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
	hwlib::window_ostream confirmStream{ oledBoundary.getConfirmMessageField(), font};
	hwlib::window_ostream weaponIDStream{ oledBoundary.getFirePowerInputField(), font };
	hwlib::window_ostream stream{ oledBoundary.getStatusMessageField(), font };

	weaponIDStream << "\f  ";
	oledBoundary.flushParts();
	if(state == STATE::WAITING_FOR_HASHTAG) {

		HWLIB_TRACE << "# pressed";
		playerInfo.setWeaponID(weaponID);
		hwlib::cout << "WeaponDMG: " << int(weaponID) << "\n";
		if(playerID > 0)
		{
			stream << "\fWaiting \nfor time.";
			confirmStream << "\f  ";
			oledBoundary.flushParts();
			HWLIB_TRACE << "state = WAITING_FOR_COMMANDS";
			state = STATE::WAITING_FOR_COMMANDS;
			waitForCommands();
		}
		else
		{
			stream << "\fYou are \nHost";
			confirmStream << "\f C to setup";
			oledBoundary.flushParts();
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
	hwlib::window_ostream confirmStream{ oledBoundary.getConfirmMessageField(), font};
	hwlib::window_ostream playerIDStream{ oledBoundary.getPlayerNumberInputField(), font };
	if(state == STATE::INPUTTING_PLAYER_ID || state == STATE::WAITING_FOR_B) {
		playerID = c - '0';
		HWLIB_TRACE << c - '0';
		
		playerIDStream << "\f" << c;
		
		confirmStream << "B to Confirm";
		oledBoundary.flushParts();
		HWLIB_TRACE << "state = WAITING_FOR_B";
		state = STATE::WAITING_FOR_B;
	}
	else if(state == STATE::INPUTTING_WEAPON_ID || state == STATE::WAITING_FOR_HASHTAG) {
		weaponID = c - '0';
		confirmStream << "\f";
		hwlib::window_ostream weaponIDStream{ oledBoundary.getFirePowerInputField(), font };
		weaponIDStream << "\f" << c;
		confirmStream << "\f# to Confirm";
		oledBoundary.flushParts();
		HWLIB_TRACE << "state = WAITING_FOR_HASHTAG";
		state = STATE::WAITING_FOR_HASHTAG;
	}
}