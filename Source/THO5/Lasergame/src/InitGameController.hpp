///	\file InitgameController.hpp
/// The InitgameController file,
/// contains the InitgameController class declaration only.
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#ifndef INITGAME_HPP
#define INITGAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"
#include "gameParameters.hpp"
#include "RunGameController.hpp"
#include <array>

class OLEDBoundary;
class KeypadController;
class transmitterController;

/**
 * @author Ferdi Stoeltie
 * @author Peter Bonnema
 * @author Matthijs Vos
 * @date 25/07/2017
 * 
 * @brief This class enables the game master to input commands (the time command is the only command for now) and send them to other players. It also sends a start command afterwards.
 * 
 * The task first waits for C to be pressed and then lets the user input a command number in the range [1..15].
 * The user has to confirm the command with a press on #.
 * Then the user can send the command to one player at a time by repeatedly pressing # and * when he is done.
 * After that the user can send the start command to a player at a time by repeatedly pressing #.
 * Lastly, pressing C activates the RunGameController.
 */
class InitGameController : public rtos::task<>, public KeypadListener, private KeyConsume  {
public:
    /**
     * @brief Constructs an instance with the specified controllers and boundaries along with a reference to the playerInformation instance, the irentity instance and a priority.
     * @param kpC
     * @param runGameController
     * @param oledBoundary
	 * @param playerInfo The shared playerInformation instance through which controllers communicate the player id and weapon id.
	 * @param irEntity
	 * @param priority The task priority. Lower is higher priority. See the documentation of the rtos.
     */
    InitGameController(KeypadController& kpC, RunGameController& runGameController, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irEntity, unsigned int priority); 
 
	/**
	 * @brief This method is called by the KeyPadController when a key is pressed.
	 * It writes the c to an internal mailbox which is read in the main() function of the task in a loop.
	 * @param c The pressed key
	 */
    void handleMessageKey(char c);
	
	/**
	 * @brief This sets the start flag. The main function first waits for this flag to be set before doing anything else. Calling this function constitutes activating the task. The task will never wait again on this flag.
	 */
	void start();
	
private:
	enum STATE
	{
		WAITING_FOR_C,
		INPUTTING_CMD,
		WAITING_FOR_HASHTAG,
		SENDING_CMD,
		SENDING_START_CMD,
	} state;

	irentity& irEntity;
	playerInformation& playerInfo;
	hwlib::font_default_8x8 font;
	OLEDBoundary& oledBoundary;
	hwlib::window_ostream statusStream, confirmStream, timeStream;
    KeypadController& keypadController; // The owner
    rtos::mailbox<char> msg;
    RunGameController& runGameController;
	rtos::flag startFlag;

    void main() override;

    char commandCount = 0;
    std::array<char, 2> commandCode { { '0', '0' } };
	byte gameDurationMin;

    void initNewCommand();
    bool validateCommand();
    void sendMessage();
    void sendStartMessage();

	// KeyConsumer methods
	void consumeChar(char c);
	void consumeHashTag();
	void consumeWildcard();
	void consumeDigits(char c);
};
#endif
