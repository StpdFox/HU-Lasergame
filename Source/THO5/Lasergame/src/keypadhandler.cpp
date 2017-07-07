#include "keypadHandler.hpp"

KeypadHandler::KeypadHandler(hwlib::keypad< 16 >& keypad) : 
task("Keypadhandler task"), keypad{keypad} {

}
void KeypadHandler::main() {
    for(;;) {
        hwlib::wait_ms(waitTime);
        if(keypad.char_available()){
          auto keypadChar = keypad.getc();
          kp->handleMessageKey(keypadChar);
        }
    }
}
void KeypadHandler::registerNext(keypadListener* nextListener) {
    kp = nextListener;
}
