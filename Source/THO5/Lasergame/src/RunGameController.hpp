///	\file RunGameController.hpp
/// The RunGameController file,
/// contains the RunGameController class declaration only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#ifndef RUNGAME_HPP
#define RUNGAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "keypadlistener.hpp"
#include "KeypadController.hpp"
#include "SpeakerController.hpp"
#include "transmitterController.hpp"
#include "receiverController.hpp"
#include "messageLogic.hpp"
#include "gameParameters.hpp"
#include "ReceiveListener.hpp"
#include "GameParameters.hpp"
#include <array>
#include <utility>

class OLEDBoundary;
class RunGameController;
class Player	{
private:
	bool playerisAlive = true;
	playerInformation& playerInfo;
	RunGameController& parentController;
	
	int healthPoints;
	// Array van 10 elementen (max 10 spelers), waar de key-pair is: hoeveelheid damage, aantal schoten
	std::array<std::pair<uint8_t, uint8_t>, 10> other_players { { 
		std::pair<uint8_t,uint8_t>(0x00, 0), 
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0)} };
		
public:
	Player(playerInformation& playerInfo, RunGameController& parentController);
	//Player(uint8_t playerId, RunGameController& parentController, int life = 100);
	
	void takeDamage(uint8_t, uint8_t);
	void doDamage();
	bool playerIsAlive();
};
typedef struct IREntity {
	//auto & button,auto & led,auto & playerInformation,auto & logic,auto & receiver
	hwlib::pin_in & button;
	hwlib::pin_out & led;
	transmitterController trans;
	messageLogic & logic;
	receiverController & receive;
	IREntity(auto & button,auto & led,auto & playerInformation,auto & logic,auto & receiver) :
	button{button},
	led{led},
	trans{playerInformation,1},
	logic{logic},
	receive{receiver} {}

} &irentity;
 /// \author Matthijs Vos
/// \Author Ferdi Stoeltie
/// \brief Controller for the runnable game logic
/// \date 11-07-2017
class RunGameController : public rtos::task<>, public KeypadListener, public ReceiveListener, private KeyConsume {
private:
	friend class Player;
	// A reference to the keypad controller. This is required to register itself as a listener
   KeypadController& kpC;
   
   // A reference to a sound interface. ISound.setSound is required to set an active sound.
   ISound& sound;
   
   // The OLEDBoundary hardware interface object.
   OLEDBoundary& oledBoundary;
   
   // RTOS
   rtos::pool<char> keypadMsgPool;
   //std::array<char, 2> commandCode { {'0', '0' } };
   rtos::pool<std::array<char, 2>> irMsgPool;
   rtos::flag keypadFlag;
   rtos::flag irMsgFlag;
   irentity irE;
   rtos::clock gameTimeSecondsClock;
   
	//get and set gameParameters
	playerInformation playerInfo;
	
   // Primitive data types
   int startOfGameTimestamp;
   int gameDurationMin;
   
	//int life = 100;
	//Player player{0x00, *this};
	Player& player;
	//bool damagePlayer(uint8_t, uint8_t);
public:

	/// \author Matthijs Vos
   /// \author Peter Bonnema 
   /// \author Marianne Delmaar
   /// \author Ferdi Stoeltie
   /// \brief Constructor that needs to be called to initialize the \c RunGameController \c instance.
   /// \param[in] kpC A reference tot the KeypadController.
   /// \param[in] sound A handle to the ISound interface.
   /// \param[in] oledBoundary a reference to the oledBoundary object.
   /// \param Priority of this rtos::task.
   RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, irentity irE, playerInformation& playerInfo , unsigned int priority );
   ~RunGameController();
	
   rtos::channel<char16_t,10> receiverMessageChannel;
   void main() override;
   /// \author Matthijs Vos
   /// \author Ferdi Stoeltie
   /// \brief Method that is inherited from interface \c KeypadListener \c.
   /// 			In here the key that is being pressed by the KeypadController will be handled.
   void handleMessageKey(char c);
   
   	void consumeChar(char c);
	void consumeHashTag();
	void consumeWildcard();
	void consumeDigits(char c);
	
	void handleReceivedMessage(auto msg);
	
	void shutDownGame();
	void writeGameResults();
	void receivedMsgstd(std::array<char, 2> msg) {
		irMsgPool.write(msg);
		irMsgFlag.set();
	}
};

#endif //RUNGAMECONTROLLER_HPP
