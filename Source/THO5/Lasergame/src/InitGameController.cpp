///	\file InitgameController.cpp
/// The InitgameController file,
/// contains the InitgameController class implementation only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#include "InitGameController.hpp"
#include "KeypadListener.hpp"
#include "KeypadController.hpp"
#include "KeyConsumer.h"
#include "OLEDBoundary.hpp"
#include "ir/transmitterController.hpp"
#include "ir/messageLogic.hpp"
#include "RunGameController.hpp"

InitGameController::InitGameController(KeypadController& kpC, RunGameController* nextListener, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irEntity, unsigned int priority) : 
    task(priority, "initGame task"),
	state{ STATE::WAITING_FOR_C },
	irEntity{ irEntity },
	playerInfo{ playerInfo },
	oledBoundary{ oledBoundary },
	keypadController{kpC},
	msg("keypad char"),
	nextListener{nextListener}
{
	oledBoundary.getGameTimeField().setLocation({ 9 * 8, 2 * 8 });
}

void InitGameController::handleMessageKey(char c)  {
   msg.write(c);
}

void InitGameController::main()  {
	for(;;) {
		char c = msg.read();
		KeyConsumer::handleMessageKey(*this, c);
	}
}

void InitGameController::initNewCommand() {
	commandCode[0] = 0;
	commandCode[1] = 0;
	commandCount = 0;
	hwlib::window_ostream stream{ oledBoundary.getBufferedLCD(), font };
	stream << "\f\n\nCommand: __\nPress # when\nyou're done.";
	oledBoundary.flush();
}

bool InitGameController::validateCommand() {
	if(    commandCode[0] >= '0' && commandCode[0] <= '9' 
		&& commandCode[1] >= '0' && commandCode[1] <= '9')  {
		byte command = (commandCode[0] - '0') * 10 + (commandCode[1] - '0');
		return command >= 1 && command <= 15;
	}
	return false;
}

void InitGameController::sendMessage() {
	// Send the message over the air with apple update system
	HWLIB_TRACE << "InitGameController sendMessage";
	irEntity.receive.suspend();
	irEntity.trans.enableFlag();
	hwlib::wait_ms(1000);
	irEntity.receive.resume();
}

void InitGameController::sendStartMessage() {
	// Send the start message + start timer over the air with apple update system
	// Deregisters self and registers other keypadlistener
	HWLIB_TRACE << "InitGameController sendStartMessage";
	irEntity.receive.suspend();
	irEntity.trans.enableFlag();
	hwlib::wait_ms(1000);
	irEntity.receive.resume();
}
	
// Keypad Methods
void InitGameController::consumeChar(char c) {
	if(state == STATE::WAITING_FOR_C && c == 'C')
	{
		HWLIB_TRACE << "C pressed";
		initNewCommand();
		HWLIB_TRACE << "state = STATE::INPUTTING_CMD";
		state = STATE::INPUTTING_CMD;
	}
	else if(state == STATE::SENDING_START_CMD && c == 'C')
	{
		HWLIB_TRACE << "C pressed";
		//TODO stuur start signaalnaar RunGameController
		hwlib::cout << "To runGame";
		keypadController.registerNext(nextListener);
		nextListener->resume();
		suspend();
	}
}

void InitGameController::consumeHashTag() {
	// If the command has been validated, it can be executed
	if(state == STATE::WAITING_FOR_HASHTAG)
	{
		HWLIB_TRACE << "# pressed";
		if(validateCommand()) {
			HWLIB_TRACE << "command is valid";
			char16_t timeBits = irEntity.logic.encode(0, (commandCode[0] - '0') * 10 + (commandCode[1] - '0'));
			playerInfo.setCompiledBits(timeBits); //playerInformation is the object in the transmitterController
			hwlib::cout.base(2);
			HWLIB_TRACE << playerInfo.getCompiledBits() << "\n";
			HWLIB_TRACE << "state = STATE::SENDING_CMD";
			
			hwlib::window_ostream stream{ oledBoundary.getBufferedLCD(), font };
			stream << "\f\n\nPress # to send\nthe command.\n\nPress * when\nyou're done.";
			oledBoundary.flush();
			
			state = STATE::SENDING_CMD;
		} else {
			HWLIB_TRACE << "command is not valid";
			HWLIB_TRACE << "state = STATE::INPUTTING_CMD";
			state = STATE::INPUTTING_CMD;
			initNewCommand();
		}
	}
	else if(state == STATE::SENDING_CMD)
	{
		sendMessage();
	}
}

void InitGameController::consumeWildcard() {
	// Send a start message
	if(state == STATE::SENDING_CMD)
	{
		hwlib::window_ostream stream{ oledBoundary.getBufferedLCD(), font };
		stream << "\f\n\nPress * to send\nthe start command.\n\nPress C to start\nplaying.";
		oledBoundary.flush();
		
		char16_t timeBits = irEntity.logic.encode(0, 1);
		playerInfo.setCompiledBits(timeBits);
		HWLIB_TRACE << "state = STATE::SENDING_START_CMD";
		state = STATE::SENDING_START_CMD;
	}
	if(state == STATE::SENDING_START_CMD)
	{
		sendStartMessage();
	}
}

void InitGameController::consumeDigits(char c) {
	if(state == STATE::INPUTTING_CMD)
	{
		HWLIB_TRACE << c << " pressed";
		commandCode[commandCount++] = c;
		hwlib::window_ostream timeStream{ oledBoundary.getGameTimeField(), font };
		if(commandCount == 1)
		{
			timeStream << "\f" << c << "_";
		}
		else if(commandCount == 2)
		{
			timeStream << "\f" << commandCode[0] << c;
			HWLIB_TRACE << "state = STATE::WAITING_FOR_HASHTAG";
			state = STATE::WAITING_FOR_HASHTAG;
		}
		oledBoundary.flushParts();
	}
}