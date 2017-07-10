#ifndef RUNGAME_HPP
#define RUNGAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "keypadlistener.hpp"
#include "KeypadController.hpp"
#include "SpeakerController.hpp"
/// \Author Ferdi Stoeltie
/// \brief Controller for the runnable game logic
class RunGameController : public rtos::task<>, public KeypadListener {
public:
	
    KeypadController& kpC;
	ISound& sound;
    rtos::flag registerFlag;
    RunGameController(KeypadController& kpC, ISound& sound);

    void main();
    void handleMessageKey(char c);
};

#endif