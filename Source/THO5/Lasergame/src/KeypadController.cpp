///	\file KeypadController.cpp
/// The KeypadController file,
/// contains the KeypadController class implementation only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#include "KeypadController.hpp"

KeypadController::KeypadController(hwlib::keypad< 16 >& keypad, unsigned int priority) : 
task(priority, "KeypadController task"), keypad{keypad} {

}
void KeypadController::main() {
    for(;;) {
        hwlib::wait_ms(waitTime);
        if(keypad.char_available()){
          auto keypadChar = keypad.getc();
          kp->handleMessageKey(keypadChar);
        }
    }
}
void KeypadController::registerNext(KeypadListener* nextListener) {
    kp = nextListener;
	//hwlib::cout << "Registering next listener\n\n" << hwlib::endl;
	//hwlib::cout.flush();
}
