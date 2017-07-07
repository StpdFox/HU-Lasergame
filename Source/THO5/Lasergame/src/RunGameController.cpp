#include "RunGameController.hpp"
RunGameController::RunGameController(KeypadHandler& kpH) : 
task("initGame task"), 
kpH{kpH}, 
registerFlag(this, "registerFlag")
{}

void RunGameController::main() {
    for(;;) {
        auto w = wait(registerFlag);
        if(w == registerFlag)   {
            kpH.registerNext(this);
        }
    }
}
void RunGameController::handleMessageKey(char c)  {
    hwlib::cout << c << " was written in testTask \n";
}
