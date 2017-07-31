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
#include "KeypadListener.hpp"
#include "KeypadController.hpp"
#include "SpeakerController.hpp"
#include "transmitterController.hpp"
#include "receiverController.hpp"
#include "messageLogic.hpp"
#include "gameParameters.hpp"
//#include "ReceiveListener.hpp"
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

//<<<<<<< HEAD
	void takeDamage(uint8_t, uint8_t);
	void doDamage();
	bool playerIsAlive();
};
//typedef struct IREntity {
	
//=======
/**
 * @author Matthijs Vos
 * @date 25/07/2017
 * 
 * @brief This class is a container of all the boundary objects and related controllers that many controllers typically need. It prevents ultra long constructor headers.
 */
struct irentity {
//>>>>>>> master
	hwlib::pin_in & button;
	hwlib::pin_out & led;
	transmitterController& trans;
	messageLogic & logic;
	receiverController & receive;
//<<<<<<< HEAD
/*	IREntity(auto & button,auto & led,auto & playerInformation,auto & logic,auto & receiver) :
	button{button},
	led{led},
	trans{playerInformation,1},
	logic{logic},
	receive{receiver} {}*/

//} &irentity;

/// \author Matthijs Vos
/// \Author Ferdi Stoeltie
/// \brief Controller for the runnable game logic
/// \date 11-07-2017
//=======
	
	/**
	 * @brief Constructs an irentity with references to all the boundaries specified.
	 * @param button
	 * @param led
	 * @param trans
	 * @param logic For decoding ir messages
	 * @param receiver
	 */
	irentity(auto & button,auto & led,auto & trans,auto & logic,auto & receiver) :
		button{button},
		led{led},
		trans{trans},
		logic{logic},
		receive{receiver} {}
};

/**
 * @author Matthijs Vos
 * @author Peter Bonnema
 * @author Ferdi Stoeltie
 * @date 25/07/2017
 * 
 * @brief This task controls the game.
 * 
 * It reacts to being hit by another player, reacting to keyboard input to shoot and it keeps track of the remaining game time and health.
 * When either the game time is up or the player has no health left it goes into a game over state where it waits for a command from a computer
 * via USB to write all the registered hits to the computer.
 */
//>>>>>>> master
class RunGameController : public rtos::task<>, public KeypadListener, public ReceiveListener, private KeyConsume {
private:
	friend class Player;
	// A reference to the keypad controller. This is required to register itself as a listener
//<<<<<<< HEAD
   /*KeypadController& kpC;
   
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
public:*/

	/// \author Matthijs Vos
   /// \author Peter Bonnema 
   /// \author Marianne Delmaar
   /// \author Ferdi Stoeltie
   /// \brief Constructor that needs to be called to initialize the \c RunGameController \c instance.
   /// \param[in] kpC A reference tot the KeypadController.
   /// \param[in] sound A handle to the ISound interface.
   /// \param[in] oledBoundary a reference to the oledBoundary object.
   /// \param Priority of this rtos::task.
/*   RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, irentity irE, playerInformation& playerInfo , unsigned int priority );
   ~RunGameController();
	
   rtos::channel<char16_t,10> receiverMessageChannel;
   void main() override;
   /// \author Matthijs Vos
   /// \author Ferdi Stoeltie
   /// \brief Method that is inherited from interface \c KeypadListener \c.
   /// 			In here the key that is being pressed by the KeypadController will be handled.
   void handleMessageKey(char c);
   
   	void consumeChar(char c);
=======*/
	
	KeypadController& kpC;

	// A reference to a sound interface. ISound.setSound is required to set an active sound.
	ISound& sound;

	// The OLEDBoundary hardware interface object.
	OLEDBoundary& oledBoundary;
	hwlib::font_default_8x8 font;
	hwlib::window_ostream oledStream, gameTimeStream;

	// RTOS
	rtos::pool<char> keypadMsgPool;
	rtos::pool<std::array<char, 2>> irMsgPool;
	rtos::pool<byte> durationPool;
	rtos::flag startFlag, keypadFlag, irMsgFlag;
	irentity& irE;
	playerInformation& playerInfo;
	rtos::clock gameTimeSecondsClock;
	Player player;
	//get and set gameParameters
	
	
	void handleReceivedMessage(const std::array<char, 2>& msg);
	void consumeChar(char c);
//>>>>>>> master
	void consumeHashTag();
	void consumeWildcard();
	void consumeDigits(char c);
	void doCountDown(int seconds);
	void main() override;
   
public:
	/**
	 * @brief Constructs an instance with the specified controllers and boundaries along with a reference to the playerInformation instance, the irentity instance and a priority.
     * @param kpC
	 * @param sound
	 * @param oledBoundary
	 * @param playerInfo The shared playerInformation instance through which controllers communicate the player id and weapon id.
	 * @param irE
	 * @param priority The task priority. Lower is higher priority. See the documentation of the rtos.
     */
	RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irE, unsigned int priority);

	/**
	 * @brief This method is called by the KeyPadController when a key is pressed.
	 * It writes the c to an internal pool and sets a flag signalling to the main function that a key was pressed.
	 * @param c The pressed key
	 */
	void handleMessageKey(char c);

	/**
	 * @brief This method is called by ReceiverController upon receiving an IR message. Simply writes `msg` to an internal pool and sets a flag that a message has been received.
	 * @param msg The message received. The first byte is the player id of the sender and the second byte is the weapon id (or other data in case of a special command).
	 */
	void receivedMsgstd(const std::array<char, 2>& msg) override;
	
/*<<<<<<< HEAD
	void handleReceivedMessage(auto msg);
	
	void shutDownGame();
	void writeGameResults();
	void receivedMsgstd(std::array<char, 2> msg) {
		irMsgPool.write(msg);
		irMsgFlag.set();
	}
=======*/

	void shutDownGame();
	void writeGameResults();
	
	/**
	 * @brief This sets the start flag and writes the specified duration to an internal pool.
	 * The main function first waits for this flag to be set before doing anything else.
	 * Calling this function constitutes activating the task. The task will never wait again on this flag.
	 * @param durationMin The length of the game in minutes.
	 */
	void startGame(byte durationMin);
//>>>>>>> master
};

#endif //RUNGAMECONTROLLER_HPP
