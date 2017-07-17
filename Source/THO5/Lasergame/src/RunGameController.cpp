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

Player::Player(uint8_t playerId, RunGameController& parentController, int life) : parentController{parentController}, life{life}	{}

void Player::damagePlayer(uint8_t player_id, uint8_t damage)	{
	other_players[player_id].first = player_id;
	other_players[player_id].second += 1; // got hit another time
	if((life -= damage) < 0)	{
		life = 0; // game over
		parentController.sound.setSound(Sounds::END_GAME);
		// set timeout
	}
	else{
		parentController.sound.setSound(Sounds::HIT);
		// stop game
	}
}

bool Player::playerIsAlive()	{
		return playerisAlive;
}

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
	gameDurationMin = 10;
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
			player.damagePlayer((uint8_t)msg[0], (uint8_t)msg[1]);
			/*if(player.isAlive())	{
				// Set timeout
			}
			else	{
				// End game for this player
			}*/
			/*if(player.isAlive())	{
				sound.setSound(Sounds::HIT);
			}
			else {
				
			}*/
/*			if(damagePlayer((uint8_t)msg[0], (uint8_t)msg[1]))	{
				sound.setSound(Sounds::HIT);
				//Set Timeout (reset time of player, 
				//this way the player cannot be damaged multiple times in a row within the same time period).
				// Player took a hit
			}
			else{
				sound.setSound(Sounds::END_GAME);
				// Player is dead. RIP player
			}*/
		}
		else if(event == gameTimeSecondsClock)
		{
			int remainingTimeSec = gameDurationMin * 60 - (hwlib::now_us() - startOfGameTimestamp) / 1'000'000;
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
	
	hwlib::wait_ms(1000);

	irE.receive.resume();
	sound.setSound(Sounds::SHOOT);
	irE.led.set(false);
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

/*bool RunGameController::damagePlayer(uint8_t player_id, uint8_t damage)	{
	players[player_id].first = player_id;
	players[player_id].second += 1; // got hit another time
	if((life -= damage) < 0)	{
		life = 0; // game over
		return false;
	}
	return true;
}*/