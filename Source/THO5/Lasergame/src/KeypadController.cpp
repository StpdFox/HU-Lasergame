#include "KeypadController.hpp"

KeypadController::KeypadController(hwlib::keypad< 16 >& keypad) : 
task("KeypadController task"), keypad{keypad} {

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
void KeypadController::registerNext(keypadListener* nextListener) {
    kp = nextListener;
}
