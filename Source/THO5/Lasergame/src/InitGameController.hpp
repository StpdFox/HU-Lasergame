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
#include "KeypadController.hpp"
#include "KeyConsumer.h"
#include <array>
/// \author Ferdi Stoeltie
/// \author Peter Bonnema
/// \brief Controller class that handles the initialize of the game settings
class InitGameController : public rtos::task<>, public KeypadListener, private KeyConsume  {
public:
	
	/// \author Ferdi Stoeltie
	/// \brief InitGameController Constructor that initializes this task.
	/// \param KeypadController& kpC The KeypadController that is used to register the next listener to.
	/// \param KeypadListener* nextListener The next KeypadListener to register to the KeypadController when this task is finished.
	/// \param unsigned_int priority The priority of this RTOS task.
    InitGameController(KeypadController& kpC, KeypadListener* nextListener, unsigned int priority);
	
    
private:
	KeypadController& keypadController; // The owner
	rtos::mailbox<char> msg;
	KeypadListener* nextListener;
	void main();
    bool incomingCommand = false;
    bool commandoIsValid = false;
    char commandCount = 0;
    std::array<char, 2> commandCode { {'0', '0' } };
	
    int COMMANDSIZE = 2; 
    // Parse the keypad data and execute code based on the key pressed
    //void parseKeypad(char s);
    void handleMessageKey(char c);
	
    void initNewCommand();
    void validateCommand();
    void sendMessage();

    void sendStartMessage();
	
	// KeyConsumer methods
	void consumeChar(char c);
	void consumeHashTag();
	void consumeWildcard();
	void consumeDigits(char c);
};
#endif