///	\file GameParamsController.hpp
/// The GameParamsController file,
/// contains the GameParamsController class decleration only. 
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 11-07-2017

#ifndef GameParams_HPP
#define GameParams_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadController.hpp"
#include "KeypadListener.hpp"
#include "gameParameters.hpp"
#include "RunGameController.hpp"
#include "InitGameController.hpp"

#include <array>

class OLEDBoundary;
class KeypadController;
class transmitterController;
class ReceiveListener;

/**
 * @author Matthijs Vos
 * @author Peter Bonnema
 * @author Marianne Delmaar
 * @author Ferdi Stoeltie
 * @date 25/07/2017
 * 
 * @brief This task enables the user to input their data which is their player id and a weapon id (which is just the weapon damage for now).
 * 
 * The task first waits until A is pressed. Then the user can input his player number.
 * After that the task waits for B and the player can input his fire power.
 * Then if the player id equals 0 it means the player is the game master and the InitGameController is switched on.
 * If the player isn't the game master the task will wait for a game time message from the game master via IR and subsequently a start message.
 * When it has received a start message the RunGameController is activated.
 */
class GameParamsController : public rtos::task<>, public KeypadListener, public ReceiveListener, private KeyConsume
{
public:
	/**
	 * @brief Constructs an instance with the specified controller and boundary object as well as a reference to the shared playerInformation instance, the shared irentity and a task priority.
	 * @param kpC
	 * @param initGameController
	 * @param runGameController
	 * @param oledBoundary
	 * @param playerInfo The shared playerInformation instance through which controllers communicate the player id and weapon id.
	 * @param irEntity
	 * @param priority The task priority. Lower is higher priority. See the documentation of the rtos.
	 */
	GameParamsController(KeypadController& kpC, InitGameController& initGameController, RunGameController& runGameController, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irEntity, unsigned int priority);
	
	/**
	 * @brief This method is called by ReceiverController upon receiving an IR message. Simply writes `msg` to an internal channel
	 * @param msg The message received. The first byte is the player id of the sender and the second byte is the weapon id (or other data in case of a special command).
	 */
	void receivedMsgstd(const std::array<char, 2>& msg) override;
	
	/**
	 * @brief This method is called by the KeyPadController when a key is pressed. It writes the c to an internal mailbox which is read in the main() function of the task in a loop.
	 * @param c The pressed key
	 */
	void handleMessageKey(char c);
	
private:
	enum STATE
	{
		WAITING_FOR_A,
		INPUTTING_PLAYER_ID,
		WAITING_FOR_B,
		INPUTTING_WEAPON_ID,
		WAITING_FOR_HASHTAG,
		WAITING_FOR_COMMANDS,
	} state;
	
	irentity& irEntity;
	playerInformation& playerInfo;
	hwlib::font_default_8x8 font;
	OLEDBoundary& oledBoundary;
	hwlib::window_ostream oledStream, statusStream, playerIDStream, confirmStream, weaponIDStream;
	KeypadController& kpC; // The owner
    rtos::mailbox<char> msg;
    InitGameController& initGameController;
	RunGameController& runGameController;
	
	typedef struct { byte receivedPlayerID; byte receivedWeaponID; } irMessage;
	rtos::channel<irMessage, 1> irMessageChannel;
	
	void consumeChar(char c);
	void consumeHashTag();
	void consumeWildcard();
	void consumeDigits(char c);
	
	void waitForCommands();
	void main();
};

#endif // GameParams_HPP
