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
#include "KeypadListener.hpp"
#include "KeypadController.hpp"
#include "SpeakerController.hpp"
#include "transmitterController.hpp"
#include "receiverController.hpp"
#include "messageLogic.hpp"
#include "gameParameters.hpp"
#include "ReceiveListener.hpp"

class OLEDBoundary;

/**
 * @author Matthijs Vos
 * @date 25/07/2017
 * 
 * @brief This class is a container of all the boundary objects and related controllers that many controllers typically need. It prevents ultra long constructor headers.
 */
struct irentity {
	hwlib::pin_in & button;
	hwlib::pin_out & led;
	transmitterController& trans;
	messageLogic & logic;
	receiverController & receive;
	
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
class RunGameController : public rtos::task<>, public KeypadListener, public ReceiveListener, private KeyConsume {
private:
	// A reference to the keypad controller. This is required to register itself as a listener
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
	rtos::clock gameTimeSecondsClock;

	//get and set gameParameters
	playerInformation& playerInfo;
	
	void handleReceivedMessage(const std::array<char, 2>& msg);
	void consumeChar(char c);
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
	
	/**
	 * @brief This sets the start flag and writes the specified duration to an internal pool.
	 * The main function first waits for this flag to be set before doing anything else.
	 * Calling this function constitutes activating the task. The task will never wait again on this flag.
	 * @param durationMin The length of the game in minutes.
	 */
	void startGame(byte durationMin);
};

#endif //RUNGAMECONTROLLER_HPP
