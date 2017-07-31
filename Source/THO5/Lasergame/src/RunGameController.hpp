///	\file RunGameController.hpp
/// The RunGameController file,
/// contains the RunGameController class declaration only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#ifndef RUNGAME_HPP
#define RUNGAME_HPP
#include "gameParameters.hpp"
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
#include "Player.hpp"
#include "MessageHandler.hpp"

class OLEDBoundary;
class RunGameController;

class data_struct {
public:
	unsigned int size = 0;
	// should be only method to have access to byte buffer from data struct objects, can use friends?
	virtual void toByteBuffer() = 0;
	
};
class Player : data_struct	{
private:
	friend class FormattedGameStats; // They are bff's!
	
	void toByteBuffer() override {
		MessageHandler& mh = MessageHandler::getInstance();
		
		//ToDo make fillkeypair instead of fillbytearray and fillbufferwithuint each and every time... Done?
		const unsigned int pID =  playerInfo.getPlayerID();
		const unsigned int pHealth = healthPoints;
		
		mh.writeKeyValuePair("Player_ID", pID);
		mh.writeKeyValuePair("Player_Health", pHealth);
		for(int i = 0; i < 10; i++)	{
			const unsigned int ii = i;
			const unsigned int other_player_key = other_players[i].first;
			const unsigned int other_player_hits = other_players[i].second;
			mh.writeKeyValuePair("Other_Player_ID", ii);
			mh.writeKeyValuePair("Player_WeaponType", other_player_key);
			mh.writeKeyValuePair("Player_Hits", other_player_hits);
		}
		mh.fillheader(MESSAGE_TYPES::PD);
		mh.setEnd();
		mh.printByteArray();
		
		mh.clearBuffer(); // Should be renamed to clear?
	}
public:
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
		
	Player(playerInformation& playerInfo, RunGameController& parentController);
	//Player(uint8_t playerId, RunGameController& parentController, int life = 100);
	
	void getResultsXml();

	void takeDamage(uint8_t, uint8_t);
	void doDamage();
	bool playerIsAlive();
};
typedef struct IREntity {
	
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
   
	Player player;
	bool previousPressHashCode = true;
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
