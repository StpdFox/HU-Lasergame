#ifndef INITGAME_HPP
#define INITGAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"
#include "KeypadController.hpp"
#include <array>
/// \author Ferdi Stoeltie
/// \brief Controller class that handles the initialize of the game settings
class InitGameController : public rtos::task<>, public KeypadListener  {
public:
    KeypadController& keypadController; // The owner
    rtos::mailbox<char> msg;
    KeypadListener* nextListener;
    InitGameController(KeypadController& kpC, KeypadListener* nextListener);
    void main();

    void handleMessageKey(char c);
private:
    bool incomingCommand = false;
    bool commandoIsValid = false;
    char commandCount = 0;
    std::array<char, 2> commandCode { {'0', '0' } };
    int COMMANDSIZE = 2; 
    // Parse the keypad data and execute code based on the key pressed
    void parseKeypad(char s);
    
    void initNewCommand();
    void validateCommand();
    void sendMessage();

    void sendStartMessage();
};
#endif