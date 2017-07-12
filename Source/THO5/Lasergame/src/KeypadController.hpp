/// \author Marianne Delmaar
/// \brief Rtos task that handles the input given by a keypad. This is then passed in to a keypadlistener
#ifndef KEYPAD_CONTROLLER_HPP
#define KEYPAD_CONTROLLER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include "KeypadListener.hpp"

class KeypadController : public rtos::task<>   {
private:
	int waitTime = 100; // 100 ms
    hwlib::keypad<16> &keypad;
	
public:
    KeypadListener* kp;
    
    KeypadController(hwlib::keypad< 16 >& keypad, unsigned int priority);
    void main();
    void registerNext(KeypadListener* nextListener);
};

#endif //KEYPAD_HANDLER_HPP