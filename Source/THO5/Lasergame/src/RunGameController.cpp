///	\file RunGameController.cpp
/// The RunGameController implementation file,
/// contains the RunGameController class implementation only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

/*<<<<<<< HEAD
#define GAMETIME_WIDTH 5

#define GAMETIME_HEIGHT 1

=======*/
#include "gameParameters.hpp"
//>>>>>>> master
#include "RunGameController.hpp"
#include "OLEDBoundary.hpp"
#include <cstdlib>
Player::Player(playerInformation& playerInfo, RunGameController& parentController) : playerInfo{playerInfo}, parentController{parentController}{
	//playerInfo.getPlayerHealth();
	healthPoints = playerInfo.getPlayerHealth();
}
//Player::Player(uint8_t playerId, RunGameController& parentController, int healthPoints) : parentController{parentController}, healthPoints{healthPoints}	{}
void Player::doDamage()	{
		// send over ir to do damage
		//parentController.irE.trans.sendMessage(playerInfo.getPlayerID(), playerInfo.getWeaponID());
}
void Player::takeDamage(byte player_id, byte damage_id)	{
	other_players[(uint8_t)player_id].first = (uint8_t)damage_id;
	other_players[(uint8_t)player_id].second += 1; // got hit another time
	if((healthPoints -= damage_id) < 0)	{
		healthPoints = 0; // game over
		parentController.sound.setSound(Sounds::END_GAME);
		parentController.shutDownGame();
		// shutdown game
	}
	else{
		parentController.sound.setSound(Sounds::HIT);
		// stop game
	}
}

/*<<<<<<< HEAD
bool Player::playerIsAlive()	{
	return playerisAlive;
}*/

//void Player::getResultsXml()	{
	//data_struct dt;
	//dt.size = 100;
	//toByteBuffer();
	/*createMessage(MESSAGE_TYPES::PD, dt);
	resetBuffer();
	fillByteArray("<player_game_info>\n");*/
	//fillByteArray("\t<player_id> ");
	//fillBufferWithUInt(playerInfo.playerID + 104);
	//fillByteArray(" </player_id>\n");
	
	//fillByteArray("\t<player_healthpoints>");
	//fillBufferWithUInt(healthPoints);
	//fillByteArray("</player_healthpoints>\n");
	
	//"\t<other_players amount=\"" << other_players.size() << "\">\n"
	//fillByteArray("\t<other_players amount=\"");
	//fillBufferWithUInt(other_players.size());
	//fillByteArray("\">\n");
	
	//hwlib::cout << "\t<player_healthpoints>" << healthPoints << "</player_healthpoints>\n";
	//printByteArray();
	/*hwlib::cout << "<player_game_info>\n";
	//hwlib::wait_ms(1000);
	hwlib::cout << "\t<player_id>" << playerInfo.playerID << "</player_id>\n" << "\t<player_healthpoints>";
	hwlib::cout << "\t<player_healthpoints>" << healthPoints << "</player_healthpoints>\n";
	hwlib::cout << "\t<other_players amount=\"" << other_players.size() << "\">\n";
	for(unsigned int i = 0; i < other_players.size() - 1; i++)	{
		hwlib::cout << "\t\t<player>\n";
		hwlib::cout << "\t\t\t<player_id>" << i << "</player_id>\n";
		hwlib::cout << "\t\t\t<weapon_id>" << other_players[i].first << "</weapon_id>\n";
		hwlib::cout << "\t\t\t<player_damage>" << other_players[i].second << "</player_damage>\n";
		hwlib::cout << "\t\t</player>\n";
	}
	hwlib::cout << "\t</other_players>\n";
	hwlib::cout << "</player_game_info>\n" << hwlib::endl;*/
//}

/*RunGameController::RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, irentity irE, playerInformation& playerInfo, unsigned int priority ) :
=======*/
RunGameController::RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irE, unsigned int priority) :
//>>>>>>> master
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
	playerInfo{playerInfo},
	gameTimeSecondsClock{ this, 1 * rtos::s, "gameTimeSecondsClock" },
//<<<<<<< HEAD
	player{playerInfo, *this}
/*	receiverMessageChannel(this,"receiverMessage")*/
//=======
/*	playerInfo{ playerInfo }
>>>>>>> master*/
{
	oledBoundary.getGameTimeField().setLocation({ 7 * 8, 6 * 8 });
}

void RunGameController::main()
/*<<<<<<< HEAD
{	
	//hier moet ergens die getMessage van receiverController komen te staan om de hits te maken
	hwlib::glcd_oled_buffered& lcd = oledBoundary.getBufferedLCD();
	auto f = hwlib::font_default_8x8();
	auto stream = hwlib::window_ostream{ lcd, f };
	stream << "\f";
=======*/
{
	wait(startFlag);
	int gameDurationMin = durationPool.read();
	kpC.registerNext(this);
	//irE.receive.setReceiveListener(this);
	playerInfo.setCompiledBits(irE.logic.encode(playerInfo.getPlayerID(), playerInfo.getWeaponID()));
	HWLIB_TRACE << "start RunGameController!\n";
	
	//TODO hier moet ergens die getMessage van receiverController komen te staan om de hits te maken
	oledStream << "\f";
//>>>>>>> master
	oledBoundary.flush();
	
	int countdownSec = 20;
	doCountDown(countdownSec);
	sound.setSound(Sounds::START_GAME);
	
	int startOfGameTimestamp = hwlib::now_us();
	while(true)
	{
		const rtos::event& event = wait();
		if(event == keypadFlag)	{
			KeyConsumer::handleMessageKey(*this, keypadMsgPool.read());
		}
		else if(event == irMsgFlag)	{
/*<<<<<<< HEAD
			hwlib::cout << "ir msg flag has been set!\n";
			std::array<char, 2> msg = irMsgPool.read();
			
			hwlib::cout << "byte01: " << msg[0] << " | byte02: " << msg[1] << " end of msg\n"; 
			player.takeDamage((uint8_t)msg[0], (uint8_t)msg[1]);
=======*/
			handleReceivedMessage(irMsgPool.read());
//>>>>>>> master
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
/*<<<<<<< HEAD
				//while(true) sleep(1);
				char c = '0';
				hwlib::cin >> c;
				if(c == 'r')	{
					player.getResultsXml();
				}
				while(true) sleep(1);
=======*/
				suspend();
//>>>>>>> master
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
void RunGameController::consumeHashTag() {
/*		if(!previousPressHashCode)	{
			previousPressHashCode = true;
		}
		else	{
			previousPressHashCode = false;
			// send data over usb to computer
			player.getResultsXml();
		}*/
}
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
/*<<<<<<< HEAD
void RunGameController::shutDownGame()	{
	gameDurationMin = 0;
}
void RunGameController::writeGameResults()	{
	hwlib::cout << "writing data brb...\t ";
	sleep(100);
	hwlib::cout << "tjuuk...";
	sleep(100);
	hwlib::cout << "tjuuk...";
	sleep(50);
	hwlib::cout << "prrp...";
	sleep(500);
	hwlib::cout << "ping!";
	sleep(500);
	hwlib::cout << "done!" << hwlib::endl;
}*/
//=======

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
//>>>>>>> master
