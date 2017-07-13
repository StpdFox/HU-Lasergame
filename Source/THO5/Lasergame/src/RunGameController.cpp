///	\file RunGameController.cpp
/// The RunGameController implementation file,
/// contains the RunGameController class implementation only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#define GAMETIME_WIDTH 5
#define GAMETIME_HEIGHT 1
#include "gameParameters.hpp"
#include "RunGameController.hpp"
#include "OLEDBoundary.hpp"

RunGameController::RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, irentity irE, unsigned int priority ) :
	rtos::task<>{ priority, "RunGameController" },
	kpC{kpC}, sound{sound}, 
	oledBoundary{ oledBoundary },
	keypadFlag(this, "keypadInputFlag"),
	irMsgFlag(this, "irMsgFlag"),
	irE{irE},
	gameTimeSecondsClock{ this, 1 * rtos::s, "gameTimeSecondsClock" },
	receiverMessageChannel(this,"receiverMessage")
{
	oledBoundary.getGameTimeField().setLocation({ 7 * 8, 6 * 8 });
}

RunGameController::~RunGameController()
{
}

void RunGameController::main()
{
	//hier moet ergens die getMessage van receiverController komen te staan om de hits te maken
	hwlib::glcd_oled_buffered& lcd = oledBoundary.getBufferedLCD();
	auto f = hwlib::font_default_8x8();
	auto stream = hwlib::window_ostream{ lcd, f };
	stream << "\f";
	oledBoundary.flush();
	
	startOfGameTimestamp = hwlib::now_us();
	gameDurationMin = 1;
	hwlib::window_ostream gameTimeStream{ oledBoundary.getGameTimeField(), f };
	while(true)
	{	

		const rtos::event& event = wait();
		if(event == keypadFlag)	{
			KeyConsumer::handleMessageKey(*this, keypadMsgPool.read());
		}
		else if(event == irMsgFlag)	{
			hwlib::cout << "ir msg flag has been set!\n";
			std::array<char, 2> msg = irMsgPool.read();
			hwlib::cout << "byte01: " << msg[0] << " | byte02: " << msg[1] << " end of msg\n"; 
		}
		else if(event == gameTimeSecondsClock)
		{
			int remainingTimeSec = gameDurationMin * 60 - (hwlib::now_us() - startOfGameTimestamp) / 100'000'000;
			gameTimeStream << "\f" << remainingTimeSec / 60 << ":" << remainingTimeSec % 60;
			oledBoundary.flushParts();
			
			if(remainingTimeSec <= 0)
			{
				HWLIB_TRACE << "Game over!";
				while(true) sleep(1);
			}
		}
		else if(event == receiverMessageChannel){
			handleReceivedMessage(irE.receive.getMessage());
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
	sound.setSound(Sounds::SHOOT);
	irE.led.set(false);
	sleep(1200*rtos::ms);
	irE.receive.resume();
}
void RunGameController::consumeDigits(char c) {}

void RunGameController::handleReceivedMessage(auto msg){
	
	byte enemyPlayerID = 0;
    byte enemyWeapon = 0;
    hwlib::cout << "byte x = " << (int)enemyPlayerID << "\n byte y = " << (int)enemyWeapon << "\n";
    if(irE.logic.decode(msg,enemyPlayerID,enemyWeapon) == 1){
		 hwlib::cout << "byte x = " << (int)enemyPlayerID << "\n byte y = " << (int)enemyWeapon << "\n";
		if(enemyPlayerID != 0){
			//player hit
			sound.setSound(Sounds::HIT);
			playerInfo.setPlayerHealth(playerInfo.getPlayerHealth() - enemyWeapon);
			hwlib::cout << "Player health: " << playerInfo.getPlayerHealth() << "\n";
		}
	}
}