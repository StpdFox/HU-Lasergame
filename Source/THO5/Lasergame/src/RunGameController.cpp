#include "RunGameController.hpp"
RunGameController::RunGameController(KeypadController& kpC, ISound& sound) : 
task("initGame task"), 
kpC{kpC}, sound{sound}, 
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
	switch(c)	{
		case '#':
			// Speakercontroller set flag for sound
			sound.setSound(Sounds::HIT);
		break;
	}
}
