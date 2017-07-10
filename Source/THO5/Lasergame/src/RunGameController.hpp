#ifndef RUNGAME_HPP
#define RUNGAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "keypadlistener.hpp"
#include "KeypadController.hpp"
/// \Author Ferdi Stoeltie
/// \brief Controller for the runnable game logic
class RunGameController : public rtos::task<>, public KeypadListener {
public:
    KeypadController& kpC;
    rtos::flag registerFlag;
    RunGameController(KeypadController& kpC);

    void main();
    void handleMessageKey(char c);
};

#endif