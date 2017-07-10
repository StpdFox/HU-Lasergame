#ifndef RUNGAME_HPP
#define RUNGAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "keypadlistener.hpp"
#include "KeypadController.hpp"
#include "SpeakerController.hpp"

class OLEDBoundary;

/// \Author Ferdi Stoeltie
/// \brief Controller for the runnable game logic
class RunGameController : public rtos::task<>, public KeypadListener {
private:

   KeypadController& kpC;
   ISound& sound;
   
   OLEDBoundary& oledBoundary;
   rtos::flag registerFlag;
   rtos::clock gameTimeSecondsClock;
   int startOfGameTimestamp;
   int gameDurationMin;

public:
	
   
   RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, unsigned int priority );
   ~RunGameController();

   void main() override;
   void handleMessageKey(char c);
};

#endif //RUNGAMECONTROLLER_HPP
