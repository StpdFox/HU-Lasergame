#ifndef RUNGAME_HPP
#define RUNGAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "keypadlistener.hpp"
#include "keypadhandler.hpp"
/// \Author Ferdi Stoeltie
/// \brief Controller for the runnable game logic
class RunGameController : public rtos::task<>, public keypadListener {
public:
    KeypadHandler& kpH;
    rtos::flag registerFlag;
    RunGameController(KeypadHandler& kpH);

    void main();
    void handleMessageKey(char c);
};

#endif