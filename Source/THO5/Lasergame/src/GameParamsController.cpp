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

GameParamsController::GameParamsController(KeypadController& kpC, InitGameController& initGameController, RunGameController& runGameController, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irEntity, unsigned int priority) : 
	rtos::task<>{ priority, "GameParamsController" },
	state{ STATE::WAITING_FOR_A },
	irEntity{ irEntity },
	playerInfo{ playerInfo },
	font{ },
	oledBoundary{ oledBoundary },
	oledStream{ oledBoundary.getBufferedLCD(), font },
	statusStream{ oledBoundary.getStatusMessageField(), font },
	playerIDStream{ oledBoundary.getPlayerNumberInputField(), font },
	confirmStream{ oledBoundary.getConfirmMessageField(), font },
	weaponIDStream{ oledBoundary.getFirePowerInputField(), font },
	kpC{kpC},
	msg("keypad char"),
	initGameController{initGameController},
	runGameController{runGameController},
	irMessageChannel{ this, "irMessageChannel" }
{
	oledBoundary.getStatusMessageField().setLocation({ 4 * 8, 1 * 8 });
	oledBoundary.getConfirmMessageField().setLocation({ 2 * 8, 5 * 8 });
	oledBoundary.getPlayerNumberInputField().setLocation({ 4 * 8, 4 * 8 });
	oledBoundary.getFirePowerInputField().setLocation({ 4 * 8, 4 * 8 });
}
	
void GameParamsController::handleMessageKey(char c) {
	msg.write(c);
}

void GameParamsController::receivedMsgstd(const std::array<char, 2>& msg)
{
	irMessageChannel.write({ msg[0], msg[1] });
}

void GameParamsController::main()
{
	irEntity.receive.setReceiveListener(this);	
	
	oledStream << "\f*--------------*";
	oledStream << "\n|   Welcome!   |";
	oledStream << "\n|   Press A    |";
	oledStream << "\n|              |";
	oledStream << "\n|              |";
	oledStream << "\n|              |";
	oledStream << "\n|              |";
	oledStream << "\n*--------------*";
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
	byte gameDuration = message.receivedWeaponID;
	HWLIB_TRACE << "timeCMD received: " << int(gameDuration);
	statusStream << "\fWaiting\nfor start";
	oledBoundary.flushParts();
	
	do {
		message = irMessageChannel.read();
	} while(!(message.receivedPlayerID == 0 && message.receivedWeaponID == 0));
	HWLIB_TRACE << "startCMD received";
	HWLIB_TRACE << "To runGame";
	runGameController.startGame(gameDuration);
}

void GameParamsController::consumeChar(char c) {
	
	if(state == STATE::WAITING_FOR_A && c == 'A')
	{
		statusStream << "\fEnter\nPlayerID";
		playerIDStream << "\f_";
		oledBoundary.flushParts();
		HWLIB_TRACE << "state = INPUTTING_PLAYER_ID";
		state = STATE::INPUTTING_PLAYER_ID;
	}
	else if(state == STATE::WAITING_FOR_B && c == 'B')
	{
		playerIDStream << "\f";
		confirmStream << "\f";
		weaponIDStream << "\f_";
		hwlib::cout << "playerID: " << int(playerInfo.getPlayerID()) << "\n";
		statusStream << "\fEnter\nWeaponID";
		oledBoundary.flushParts();
		HWLIB_TRACE << "state = INPUTTING_WEAPON_ID";
		state = STATE::INPUTTING_WEAPON_ID;
	}
}

void GameParamsController::consumeHashTag() {
	weaponIDStream << "\f";
	oledBoundary.flushParts();
	if(state == STATE::WAITING_FOR_HASHTAG)
	{
		HWLIB_TRACE << "# pressed";
		hwlib::cout << "WeaponDMG: " << int(playerInfo.getWeaponID()) << "\n";
		if(playerInfo.getPlayerID() > 0)
		{
			statusStream << "\fWaiting\nfor time.";
			confirmStream << "\f";
			oledBoundary.flushParts();
			HWLIB_TRACE << "state = WAITING_FOR_COMMANDS";
			state = STATE::WAITING_FOR_COMMANDS;
			waitForCommands();
		}
		else
		{
			hwlib::cout << "To initGame";
			initGameController.start();
		}
		suspend();
	}
}

void GameParamsController::consumeWildcard()
{}

void GameParamsController::consumeDigits(char c) {
	if(state == STATE::INPUTTING_PLAYER_ID || state == STATE::WAITING_FOR_B) {
		playerInfo.setPlayerID(c - '0');
		playerIDStream << "\f" << c;
		confirmStream << "\f\nB to Confirm";
		oledBoundary.flushParts();
		HWLIB_TRACE << "state = WAITING_FOR_B";
		state = STATE::WAITING_FOR_B;
	}
	else if(state == STATE::INPUTTING_WEAPON_ID || state == STATE::WAITING_FOR_HASHTAG) {
		playerInfo.setWeaponID(c - '0');
		weaponIDStream << "\f" << c;
		confirmStream << "\f\n# to Confirm";
		oledBoundary.flushParts();
		HWLIB_TRACE << "state = WAITING_FOR_HASHTAG";
		state = STATE::WAITING_FOR_HASHTAG;
	}
}