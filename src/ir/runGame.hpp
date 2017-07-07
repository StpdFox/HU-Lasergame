#include "hwlib.hpp"	
#include "rtos.hpp"
#include "gameParameters.hpp"
#include "transmitterController.hpp"
#include "transmitter.hpp"
#include "messageLogic.hpp"

#ifndef RUNGAME_HPP
#define RUNGAME_HPP

class runGame : public rtos::task<>{
private:
	messageLogic & messageLogic;
	playerInformation & playerInformation;
	gameParameters getGameParameters;
	rtos::flag gameOverFlag;
	rtos::flag buttonPressedFlag;
	transmitterController transmitterController;

	rtos::channel<char16_t,10> receiverInformationChannel;
	int hitsArrayPos = 0;
	hit * hits[10];
	byte playerID =0;
	byte weaponID =0;
	char healthArray[16] = "Health: 100"


	void main(void){
		bool registerGameParametersDone = 1;
		char key 0;

		//setup game parameters, currently set to true, gotta add functions

	int playTime = 0;
	while(1){
		playerID = 0;
		weaponID = 0;
		wait(receiverInformationChannel);
		auto bitstream = receiverInformationChannel.read();
		if(bitstream != -1){
			if(messageLogic.decode(bitstream,playerID,weaponID)){
				if(playerID ==0){
					playTime = (weaponID*60);
					hwlib::cout <<((int)weaponID*60) << "\n";
					break;
				}
				else{
					continue;
				}
			}
		}else{
			continue;
		}
	}

	int cooldownTime = 10;

	//wait for game to start 

	//update health

	while(1){
		auto event = wait(buttonPressedFlag + gameOverFlag + receiverInformationChannel);

		if(event == gameOverFlag){
			hwlib::cout <<"GAME OVER \n";
		}
		//fire button pressed
		else if(event == buttonPressedFlag){
		//suspend button control
		transmitterController.enableFlag();
		//play sounds
		//fancy ledshow
		hwlib::cout << "pewpew \n";
		sleep (100 * rtos::ms);
		//resume button control	
		}
		else if(event == receiverInformationChannel){
			auto information = receiverInformationChannel.read();
			if(messageLogic.decode(information,playerID,weaponID)){
				hit h(playerID,weaponID);
				hwlib::cout << "pID" << playerID << "\n" << "wID" << weaponID << "\n";
				hits[hitsArrayPos] = &h;
				hitsArrayPos++;

				//update lcd

				//play sound

				playerInformation.setHealth(playerInformation.getHealth() -10);
				auto health = playerInformation.getHealth();

				if(health > 0){
					//update health
				}else{
					//gameover
				}
				//update lcd


			}
		}
	}

	}
public :

runGame(messageLogic & messageLogic,playerInformation & playerInformation):
task(6,"runGame"),
messageLogic(messageLogic),
playerInformation(playerInformation)
getGameParameters(messageLogic,playerInformation).
gameOverFlag(this, "gameOverFlag"),
buttonPressedFlag(this,"buttonPressedFlag"),
transmitterController(playerInformation),
receiverInformationChannel(this, "receiverInformationChannel")
{}

void enableFlag(){
	buttonPressedFlag.set();

}
void write(char16_t value){
	receiverInformationChannel.write(value);

}
};
#endif