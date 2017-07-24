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

RunGameController::RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irE, unsigned int priority) :
	rtos::task<>{ priority, "RunGameController" },
	kpC{kpC}, sound{sound}, 
	oledBoundary{ oledBoundary },
	font{ },
	oledStream{ oledBoundary.getBufferedLCD(), font },
	gameTimeStream{ oledBoundary.getGameTimeField(), font },
	statusMessageStream{ oledBoundary.getStatusMessageField(),font},
	keypadMsgPool{ "keypadMsgPool" },
	irMsgPool{ "irMsgPool" },
	durationPool{ "durationPool" },
	startFlag{ this, "startFlag" },
	keypadFlag{this, "keypadInputFlag"},
	irMsgFlag{this, "irMsgFlag"},
	irE{ irE },
	gameTimeSecondsClock{ this, 1 * rtos::s, "gameTimeSecondsClock" },
	playerInfo{ playerInfo }
{
	
	
}

RunGameController::~RunGameController()
{
}

void RunGameController::main()
{

	wait(startFlag);
	oledBoundary.getGameTimeField().setLocation({ 10 * 8, 6 * 8 });
	oledBoundary.getStatusMessageField().setLocation({1 * 8, 1* 8});
	int gameDurationMin = durationPool.read();
	kpC.registerNext(this);
	
	HWLIB_TRACE << "start RunGameController!\n";
	
	//TODO hier moet ergens die getMessage van receiverController komen te staan om de hits te maken
	oledStream << "\f*--------------*";
	oledStream << "\n|       |      |";
	oledStream << "\n|       |ID: "<<(int)playerInfo.getPlayerID()<<" |";
	oledStream << "\n|       |------|";
	oledStream << "\n|       |DMG:"<<(int)playerInfo.getWeaponID()<<" |";
	oledStream << "\n|-------|------|";
	oledStream << "\n|HP:"<<playerInfo.getPlayerHealth()<<" |      |";
	oledStream << "\n*--------------*";
	oledBoundary.flush();

	
	int countdownSec = 20;
	doCountDown(countdownSec);
	irE.receive.setReceiveListener(this);
	playerInfo.setCompiledBits(irE.logic.encode(playerInfo.getPlayerID(), playerInfo.getWeaponID()));
	sound.setSound(Sounds::START_GAME);

	
	int startOfGameTimestamp = hwlib::now_us();
	while(true)
	{
		const rtos::event& event = wait();
		if(event == keypadFlag)	{
			KeyConsumer::handleMessageKey(*this, keypadMsgPool.read());
		}
		else if(event == irMsgFlag)	{
			HWLIB_TRACE << "Got hit";
			handleReceivedMessage(irMsgPool.read());
		}
		else if(event == gameTimeSecondsClock)
		{
			int remainingTimeSec = gameDurationMin * 60 - (hwlib::now_us() - startOfGameTimestamp) / 1'000'000;
			gameTimeStream << "\f" << remainingTimeSec / 60 << ":" << remainingTimeSec % 60;
			oledBoundary.flushParts();
			
			if(remainingTimeSec <= 0)
			{
				sound.setSound(Sounds::END_GAME);
				HWLIB_TRACE << "Game over!";
				suspend();
			}
		}
	}
}

void RunGameController::doCountDown(int seconds)
{
	int startOfGameTimestamp = hwlib::now_us(), remainingTimeSec = 1;
	while(remainingTimeSec > 0)
	{
		remainingTimeSec = seconds - (hwlib::now_us() - startOfGameTimestamp) / 1'000'000;
		gameTimeStream << "\f" << remainingTimeSec / 60 << ":" << remainingTimeSec % 60;
		oledBoundary.flushParts();
		//TODO beep geluid
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
	
		HWLIB_TRACE << "\n true ";
		//player hit
		statusMessageStream << "Hi!";
		oledBoundary.flushParts();
		sound.setSound(Sounds::HIT);
		playerInfo.setPlayerHealth(playerInfo.getPlayerHealth() - msg[1]);
		hwlib::cout << "Player health: " << playerInfo.getPlayerHealth() << "\n";
		sleep_non_block(1*rtos::s);
		statusMessageStream << "\f";
	
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
