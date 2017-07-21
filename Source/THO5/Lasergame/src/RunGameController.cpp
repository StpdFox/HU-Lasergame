///	\file RunGameController.cpp
/// The RunGameController implementation file,
/// contains the RunGameController class implementation only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#include "gameParameters.hpp"
#include "RunGameController.hpp"
#include "OLEDBoundary.hpp"

RunGameController::RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, irentity irE, unsigned int priority ) :
	rtos::task<>{ priority, "RunGameController" },
	kpC{kpC}, sound{sound}, 
	oledBoundary{ oledBoundary },
	font{ },
	oledStream{ oledBoundary.getBufferedLCD(), font },
	gameTimeStream{ oledBoundary.getGameTimeField(), font },
	keypadMsgPool{ "keypadMsgPool" },
	irMsgPool{ "irMsgPool" },
	durationPool{ "durationPool" },
	startFlag{ this, "startFlag" },
	keypadFlag{this, "keypadInputFlag"},
	irMsgFlag{this, "irMsgFlag"},
	irE{ irE },
	gameTimeSecondsClock{ this, 1 * rtos::s, "gameTimeSecondsClock" },
	playerInfo{ },
	startOfGameTimestamp{ 0 },
	gameDurationMin{ 0 }
{
	oledBoundary.getGameTimeField().setLocation({ 7 * 8, 6 * 8 });
}

RunGameController::~RunGameController()
{
}

void RunGameController::main()
{
	wait(startFlag);
	kpC.registerNext(this);
	HWLIB_TRACE << "start game!\n";
	
	//hier moet ergens die getMessage van receiverController komen te staan om de hits te maken
	oledStream << "\f";
	oledBoundary.flush();
	
	startOfGameTimestamp = hwlib::now_us();
	gameDurationMin = durationPool.read();
	while(true)
	{
		const rtos::event& event = wait();
		if(event == keypadFlag)	{
			KeyConsumer::handleMessageKey(*this, keypadMsgPool.read());
		}
		else if(event == irMsgFlag)	{
			handleReceivedMessage(irMsgPool.read());
		}
		else if(event == gameTimeSecondsClock)
		{
			int remainingTimeSec = gameDurationMin * 60 - (hwlib::now_us() - startOfGameTimestamp) / 1'000'000;
			gameTimeStream << "\f" << remainingTimeSec / 60 << ":" << remainingTimeSec % 60;
			oledBoundary.flushParts();
			
			if(remainingTimeSec <= 0)
			{
				HWLIB_TRACE << "Game over!";
				while(true) sleep(10);
			}
		}
	}
}

void RunGameController::handleMessageKey(char c)  {
	keypadMsgPool.write(c);
	keypadFlag.set();
}

void RunGameController::consumeChar(char c) {}
void RunGameController::consumeHashTag() {}
void RunGameController::consumeWildcard() {
	irE.led.set(true);
	irE.receive.suspend();
	irE.trans.enableFlag();
	
	sleep(1000 * rtos::ms);

	irE.receive.resume();
	sound.setSound(Sounds::SHOOT);
	irE.led.set(false);
}
void RunGameController::consumeDigits(char c) {}

void RunGameController::handleReceivedMessage(const std::array<char, 2>& msg)
{
	hwlib::cout << "byte01: " << (int)msg[0] << " | byte02: " << (int)msg[1] << " end of msg\n";
	if(msg[0] != 0){
		//player hit
		sound.setSound(Sounds::HIT);
		playerInfo.setPlayerHealth(playerInfo.getPlayerHealth() - msg[1]);
		hwlib::cout << "Player health: " << playerInfo.getPlayerHealth() << "\n";
	}
}

void RunGameController::receivedMsgstd(const std::array<char, 2>& msg)
{
	irMsgPool.write(msg);
	irMsgFlag.set();
}

void RunGameController::startGame(byte durationMin)
{
	durationPool.write(durationMin);
	startFlag.set();
}
