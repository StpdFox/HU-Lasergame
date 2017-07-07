/// \author Marianne Delmaar
/// \brief Rtos task that handles the input given by a keypad. This is then passed in to a keypadlistener
#ifndef KEYPAD_HANDLER_HPP
#define KEYPAD_HANDLER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include "keypadlistener.hpp"
class KeypadHandler : public rtos::task<>   {
public:
    int waitTime = 100; // 100 ms
    hwlib::keypad<16> &keypad;

    keypadListener* kp;
    
    KeypadHandler(hwlib::keypad< 16 >& keypad);
    void main();
    void registerNext(keypadListener* nextListener);
};

#endif //KEYPAD_HANDLER_HPP