#include "RunGameController.hpp"
RunGameController::RunGameController(KeypadController& kpC) : 
task("initGame task"), 
kpC{kpC}, 
registerFlag(this, "registerFlag")
{}

void RunGameController::main() {
    for(;;) {
        auto w = wait(registerFlag);
        if(w == registerFlag)   {
            kpC.registerNext(this);
        }
    }
}
void RunGameController::handleMessageKey(char c)  {
    hwlib::cout << c << " was written in testTask \n";
}
