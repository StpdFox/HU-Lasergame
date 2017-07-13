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

/// \author Ferdi Stoeltie
/// \brief Controller class that handles the initialize of the game settings
class InitGameController : public rtos::task<>, public KeypadListener, private KeyConsume  {
public:
    InitGameController(KeypadController& kpC, KeypadListener* nextListener, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irEntity, unsigned int priority);

    void handleMessageKey(char c);
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
    KeypadController& keypadController; // The owner
    rtos::mailbox<char> msg;
    KeypadListener* nextListener;
	
    void main() override;

    char commandCount = 0;
    std::array<char, 2> commandCode { { '0', '0' } };
    // Parse the keypad data and execute code based on the key pressed
    //void parseKeypad(char s);
    
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